/*
	TODO:	Optimize current consuption
			see https://docs.espressif.com/projects/esp-idf/en/latest/api-reference/system/sleep_modes.html#

 */

#include <WiFi.h>
#include <DNSServer.h>
#include <ESPmDNS.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <U8x8lib.h>
#include "rom/rtc.h"  
#include <esp_wifi.h>
#include <esp_bt.h>
#include <esp_bt_main.h>
#include <esp_spi_flash.h>

#include "driver/mcpwm.h"
#include "soc/mcpwm_reg.h"
#include "soc/mcpwm_struct.h"

#include "clock.h"
#include "index.h"
#include "ctrl.h"

#define BUZZER_PIN      26
#define BUTTON1_PIN     27
#define BUTTON2_PIN     22

#define STATE_DEFAULT                0
#define STATE_WIFI_INIT              1
#define STATE_WIFI_AP_INIT           2
#define STATE_WIFI_ACTIVE            3
#define STATE_WIFI_CLIENT_INIT       4
#define STATE_WIFI_CLIENT_CONNECTING 5
#define STATE_WIFI_CLIENT_CONNECTED  6

#define MOT_DRV_PIN        26
#define MOT_DIR_PIN        25
#define MOT_PWM_PIN        5

#define TOP_SWITCH_PIN     13
#define BOTTOM_SWITCH_PIN  34
#define STOP_SWITCH_PIN    14
#define UP_BUTTON_PIN      27
#define DOWN_BUTTON_PIN    35


#define TOP_SWITCH_BIT     0
#define BOTTOM_SWITCH_BIT  1
#define STOP_SWITCH_BIT    2
#define UP_BUTTON_BIT      3
#define DOWN_BUTTON_BIT    4

// OLED
U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);
//U8X8_SSD1306_128X64_NONAME_4W_SW_SPI u8x8(/* clock=*/ 15, /* data=*/ 4, /* cs=*/ 12, /* dc=*/ 14, /* reset=*/ 23);

// Net
const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 4, 1);
DNSServer dnsServer;
const char *myHostname = "chicken";
static int server_initialized = 0;

// NTP
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;

// APP
static uint32_t state = 0;
static String ssid = "";
static String pw = "";
static uint32_t buttons = 0;

// Control
static int ctrl = 0;

WebServer server(80);

ClockClass rtc(0);


void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

boolean isIp(String str) {
  for (int i = 0; i < str.length(); i++) {
    int c = str.charAt(i);
    if (c != '.' && (c < '0' || c > '9')) {
      return false;
    }
  }
  return true;
}

String toStringIp(IPAddress ip) {
  String res = "";
  for (int i = 0; i < 3; i++) {
    res += String((ip >> (8 * i)) & 0xFF) + ".";
  }
  res += String(((ip >> 8 * 3)) & 0xFF);
  return res;
}

boolean captivePortal() {
  if (!isIp(server.hostHeader()) && server.hostHeader() != (String(myHostname)+".local")) {
    Serial.print("Request redirected to captive portal");
    server.sendHeader("Location", String("http://") + toStringIp(server.client().localIP()), true);
    server.send ( 302, "text/plain", ""); // Empty content inhibits Content-length header so we have to close the socket ourselves.
    server.client().stop(); // Stop is needed because we sent no content length
    return true;
  }
  return false;
}

void handleNotFound() 
{
  if (captivePortal()) { // If caprive portal redirect instead of displaying the error page.
    return;
  }
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for ( uint8_t i = 0; i < server.args(); i++ ) {
    message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
  }
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.send ( 404, "text/plain", message );
}

void handleRoot()
{  
  if(state != STATE_WIFI_CLIENT_CONNECTED) {
    String s = MAIN_page;
    server.send(200, "text/html", s);
  } else{
    String s = CTRL_page;
    server.send(200, "text/html", s);
  }
}

void handleGetDate()
{ 
  String s = rtc.toString() + " Reboot Count:" 
    + 0;
  server.send(200, "text/plane", s);
}

void handleSetWifi()
{ 
  Serial.println("setWifi");
  ssid = server.arg("ssid");
  pw = server.arg("pw");

  Serial.println(ssid);
  Serial.println(pw);

  state = STATE_WIFI_CLIENT_INIT;
}

void handleSetCtrl(void)
{  
  ctrl = server.arg("data").toInt();

  char buf[50];

  sprintf(buf, "setCtrl: %d", ctrl);
  
  Serial.println(buf);
}

void init_server(void)
{
    if(!server_initialized) {
      dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
      dnsServer.start(DNS_PORT, "*", apIP);      
           
      server.on("/", handleRoot);  
      server.on("/setWifi", handleSetWifi);
      server.on("/getDate", handleGetDate);
      server.on("/setCtrl", handleSetCtrl);
      server.on("/generate_204", handleRoot);  //Android captive portal. Maybe not needed. Might be handled by notFound handler.
      server.on("/fwlink", handleRoot);  //Microsoft captive portal. Maybe not needed. Might be handled by notFound handler.
      server.onNotFound ( handleNotFound );    
      server.begin();
      server_initialized = true;
      Serial.println("HTTP server started");
    }
}

void do_wifi(void)
{
  switch(state) {
    case STATE_WIFI_INIT:
      break;
    case STATE_WIFI_AP_INIT:      
      WiFi.mode(WIFI_AP);
      WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));  
      WiFi.softAP("Chicken-Guard");
      delay(500);  
      Serial.print("AP IP address: ");
      Serial.println(WiFi.softAPIP());
      init_server();
      state = STATE_WIFI_ACTIVE;
      break;

    case STATE_WIFI_CLIENT_INIT:      
      WiFi.disconnect();
      WiFi.mode(WIFI_STA);
      WiFi.begin(ssid.c_str(), pw.c_str());
      init_server();
      state = STATE_WIFI_CLIENT_CONNECTING;
      break;

    case STATE_WIFI_CLIENT_CONNECTING:
      if (WiFi.status() != WL_CONNECTED) {                
          Serial.print(".");
      } else {
          Serial.println("CONNECTED");
          Serial.println("");
          Serial.print("Connected to ");
          Serial.print("IP address: ");
          Serial.println(WiFi.localIP());

          // Setup MDNS responder
          if (!MDNS.begin(myHostname)) {
            Serial.println("Error setting up MDNS responder!");
          } else {
            Serial.println("mDNS responder started");
            // Add service to MDNS-SD
            MDNS.addService("http", "tcp", 80);
          }
                   
          state = STATE_WIFI_CLIENT_CONNECTED;
      }
      break;
      

    case STATE_WIFI_CLIENT_CONNECTED:
    case STATE_WIFI_ACTIVE:
      dnsServer.processNextRequest();
      server.handleClient();
      break;
  }  
}

void do_buttons(void)
{
  static uint32_t timeout = 0;
  
  if((rtc.tickMs() - timeout) > 50) {
    if(digitalRead(TOP_SWITCH_PIN) == 0){
      buttons |= 1 << TOP_SWITCH_BIT;
    } else {
      buttons &= ~(1 << TOP_SWITCH_BIT);
    }
    if(digitalRead(BOTTOM_SWITCH_PIN) == 0){
      buttons |= 1 << BOTTOM_SWITCH_BIT;
    } else {
      buttons &= ~(1 << BOTTOM_SWITCH_BIT);
    }
    if(digitalRead(STOP_SWITCH_PIN) == 1){
      buttons |= 1 << STOP_SWITCH_BIT;
    } else {
      buttons &= ~(1 << STOP_SWITCH_BIT);
    }
    if(digitalRead(UP_BUTTON_PIN) == 1){
      buttons |= 1 << UP_BUTTON_BIT;
    } else {
      buttons &= ~(1 << UP_BUTTON_BIT);
    }
    if(digitalRead(DOWN_BUTTON_PIN) == 1){
      buttons |= 1 << DOWN_BUTTON_BIT;
    } else {
      buttons &= ~(1 << DOWN_BUTTON_BIT);
    }
    timeout = rtc.tickMs();      
  }
/*
  String s = "";

  if(buttons & (1 << TOP_SWITCH_BIT)) {
    s += "TOP_SWITCH ";  
  }
  if(buttons & (1 << BOTTOM_SWITCH_BIT)) {
    s += "BOTTOM_SWITCH ";  
  }
  if(buttons & (1 << STOP_SWITCH_BIT)) {
    s += "STOP_SWITCH ";  
  }
  if(buttons & (1 << UP_BUTTON_BIT)) {
    s += "UP_BUTTON ";  
  }
  if(buttons & (1 << DOWN_BUTTON_BIT)) {
    s += "DOWN_BUTTON ";  
  }

  Serial.println(s);
*/  
}

void do_display(void)
{
  switch(state) {
    case STATE_DEFAULT:
    case STATE_WIFI_CLIENT_CONNECTED:
      u8x8.drawString(0, 0, "Chicken Guard");
      break; 
    case STATE_WIFI_AP_INIT:
      u8x8.drawString(0, 0, "Wifi Setup  ");
      break;
  }

  switch(state) {
    case STATE_DEFAULT:   
    case STATE_WIFI_CLIENT_CONNECTED:
    case STATE_WIFI_ACTIVE: {      
      if(rtc.tick()) {
        u8x8.drawString(0, 2, rtc.timeToCStr());
        char buf[20];
        sprintf(buf, "State: %d", state);
        u8x8.drawString(0, 4, buf);
      }   
      break;
    }    
  }
}

void do_ntp()
{
    static int ntp_state = 0;
    static uint32_t timeout = 0;

    switch(state) {
      case STATE_WIFI_CLIENT_CONNECTED:
        if(ntp_state == 0)
          ntp_state = 1;
        break;
      default:
        ntp_state = 0;      
    }
    
    switch(ntp_state) {        
        case 1: {
            configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
            printLocalTime();         
            ntp_state++;
            break;
        }
        case 2: {
            if((rtc.tickMs() - timeout) > 1000) {
              //printLocalTime();
              timeout = rtc.tickMs();
            }
            break;
        }        
    }    
}

void do_ctrl(void)
{
  static int mot_state = 0;
  static uint32_t timeout = 0;
  static int last_ctrl = 0;
  static int d = 1;
  static int duty = 0;
  static uint32_t last_buttons = 0;

  if(ctrl != last_ctrl) {
    mot_state = 1;
    last_ctrl = ctrl;
  }


  if(buttons != last_buttons) {
    if(buttons & (1 << UP_BUTTON_BIT)) {
      ctrl = 1;
    } else if(buttons & (1 << DOWN_BUTTON_BIT)) {
      ctrl = 2;
    } else {
      ctrl = 0;
    }
    last_buttons = buttons;
  }
  
  if( (buttons & (1 << TOP_SWITCH_BIT)) || (buttons & (1 << BOTTOM_SWITCH_BIT)) ) {
    mot_state = 1;
    ctrl = 0;
    last_ctrl = 0;
  }

  switch(mot_state) {
    case 0: // INIT
      mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, MOT_PWM_PIN);    //Set GPIO as PWM0A, to which Motor is connected

      mcpwm_config_t pwm_config;
      pwm_config.frequency = 20000;    //frequency Hz
      pwm_config.cmpr_a = 0;    //duty cycle of PWMxA = 0
      pwm_config.cmpr_b = 0;    //duty cycle of PWMxb = 0
      pwm_config.counter_mode = MCPWM_UP_COUNTER;
      pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
      mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config); //Configure PWM0A & PWM0B with above settings

      Serial.println("Mot init");
      mot_state = 1;
      break;

    case 1: // STOP
      if((rtc.tickMs() - timeout) > 1) {
        if(duty > 0) {
          duty--;
          mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, duty);
          //Serial.println("Mot down");
        } else {
          if(ctrl == 1) {
            digitalWrite(MOT_DIR_PIN, 0);
            d = 1;
            //Serial.println("Mot dir left");
            mot_state = 100;
          } else if(ctrl == 2) {
            d = 1; 
            digitalWrite(MOT_DIR_PIN, 1);
            mot_state = 100;
            //Serial.println("Mot dir right");
          }          
          
        }        
        timeout = rtc.tickMs();
      }
      break;
   
    case 100:
      if((rtc.tickMs() - timeout) > 30) {
        //Serial.println("Mot active");
        mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, duty);       

        if(d == 1 && duty < 100) {
          duty++;
          if(duty == 100) {
            d = 0;
          }          
        } else if(d == 0 && duty > 20) {
          duty--;
          if(duty == 20) {
            d = 1;
          }
        }
        timeout = rtc.tickMs();
      }
      break;
  }    
}

void setup(void)
{
  pinMode(MOT_DRV_PIN, OUTPUT);
  digitalWrite(MOT_DRV_PIN, 1);

  pinMode(MOT_PWM_PIN, OUTPUT);
  digitalWrite(MOT_PWM_PIN, 0);
  
  pinMode(MOT_DIR_PIN, OUTPUT);
  digitalWrite(MOT_DIR_PIN, 0);

  pinMode(TOP_SWITCH_PIN, INPUT);
  pinMode(BOTTOM_SWITCH_PIN, INPUT);
  pinMode(STOP_SWITCH_PIN, INPUT);
  pinMode(UP_BUTTON_PIN, INPUT);
  pinMode(DOWN_BUTTON_PIN, INPUT);
  
  Serial.begin(115200);
  Serial.println("");

/*
  Serial.setDebugOutput(true);
  esp_log_level_set("*", ESP_LOG_VERBOSE);
  ESP_LOGD("EXAMPLE", "This doesn't show");
  
  log_v("Verbose");
  log_d("Debug");
  log_i("Info");
  log_w("Warning"); 
  log_e("Error");
*/
  pinMode(BUTTON1_PIN, INPUT);
  pinMode(BUTTON2_PIN, INPUT);
   
  u8x8.begin();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.clear();

 

  state = STATE_WIFI_CLIENT_INIT;
}

void loop(void)
{
  do_buttons();
  do_ctrl();
  do_ntp();
  do_display(); 
  do_wifi();  
}
