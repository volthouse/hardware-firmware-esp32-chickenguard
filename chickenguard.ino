
#include <WiFi.h>
#include <DNSServer.h>
#include <ESPmDNS.h>
#include <WebServer.h>
#include <U8x8lib.h>

#include "EEPROM.h"
#include "driver/mcpwm.h"
#include "soc/mcpwm_reg.h"
#include "soc/mcpwm_struct.h"

#include "ctrl.h"
#include "setup.h"
#include "dcf.h"
#include "sunriseset.h"

#define ARRAYSIZE(x) (sizeof x / sizeof x[0])

#define STATE_DEFAULT                0
#define STATE_WIFI_INIT              1
#define STATE_WIFI_AP_INIT           2
#define STATE_WIFI_ACTIVE            3
#define STATE_WIFI_CLIENT_INIT       4
#define STATE_WIFI_CLIENT_CONNECTING 5
#define STATE_WIFI_CLIENT_CONNECTED  6

#define DOOR_STATE_OPEN   1
#define DOOR_STATE_CLOSED 2
#define DOOR_STATE_TRAVEL 3

#define CTRL_STOP 0
#define CTRL_CLOSE 1
#define CTRL_OPEN  2

#define MOT_DRV_PIN        26
#define MOT_DIR_PIN        25
#define MOT_PWM_PIN        27

#define MOT_DIR_OPEN       0
#define MOT_DIR_CLOSE      1

#define MOT_CTRL_INIT      0
#define MOT_CTRL_STOP      1
#define MOT_CTRL_IDLE      2
#define MOT_CTRL_OPEN      3
#define MOT_CTRL_CLOSE     4

#define TOP_SWITCH_PIN     13
#define BOTTOM_SWITCH_PIN  14
#define STOP_SWITCH_PIN    39

#define UP_BUTTON_PIN      5
#define DOWN_BUTTON_PIN    17
#define STOP_BUTTON_PIN    38

#define TOP_SWITCH_BIT     0
#define BOTTOM_SWITCH_BIT  1
#define STOP_SWITCH_BIT    2
#define UP_BUTTON_BIT      3
#define DOWN_BUTTON_BIT    4
#define STOP_BUTTON_BIT    5


#define MAGIC_NO 0x4855484E

// OLED
U8X8_SSD1306_128X64_NONAME_SW_I2C m_u8x8(/* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);

//U8X8_SSD1306_128X64_NONAME_4W_SW_SPI m_u8x8(/* clock=*/ 15, /* data=*/ 4, /* cs=*/ 12, /* dc=*/ 14, /* reset=*/ 23);

// Net
const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 4, 1);
DNSServer m_dns_server;
const char *myHostname = "chicken";
static int m_server_initialized = 0;
WebServer m_server(80);

// NTP
const char* m_ntp_server = "pool.ntp.org";
const long  m_gmt_offset_sec = 3600;
const int m_daylight_offset_sec = 3600;

// APP
static uint32_t m_app_state = 0;

// Control
static int m_ctrl = 0;
static int m_door_state = 0;
static int m_buttons = 0;
static int m_time_ctrl = 0;
static int m_test_enabled = 1;

// Settings
#pragma pack(1)
struct {
  unsigned int magicNo;
  char ssid[100];
  char pw[100];
  int maxTravelTime;
} settings;
#pragma pop

void init_settings(void)
{
  if (!EEPROM.begin(sizeof(settings))) {
    Serial.println("failed to initialise EEPROM");
  }

  EEPROM.readBytes(0, &settings, sizeof(settings));

  if(MAGIC_NO != settings.magicNo) {
    memset(&settings, 0, sizeof(settings));
    settings.magicNo = MAGIC_NO;
    settings.maxTravelTime = 1000; 
    Serial.println("settings cleared");
  } else {
    Serial.println(settings.ssid);
    Serial.println(settings.pw);
  }
}

void save_settings(void)
{
  EEPROM.writeBytes(0, &settings, sizeof(settings));
  EEPROM.commit();
}

void init_server(void)
{
    if(!m_server_initialized) {
      m_dns_server.setErrorReplyCode(DNSReplyCode::NoError);
      m_dns_server.start(DNS_PORT, "*", apIP);

      m_server.on("/", handleRoot);
      m_server.on("/setWifi", handleSetWifi);
      m_server.on("/getApList", handleGetApList);
      m_server.on("/getState", handleGetState);
      m_server.on("/getNextDateTimeCtrl", handleGetNextDateTimeCtrl);      
      m_server.on("/setCtrl", handleSetCtrl);
      m_server.on("/generate_204", handleRoot);  //Android captive portal. Maybe not needed. Might be handled by notFound handler.
      m_server.on("/fwlink", handleRoot);  //Microsoft captive portal. Maybe not needed. Might be handled by notFound handler.
      m_server.onNotFound ( handleNotFound );
      m_server.begin();
      m_server_initialized = true;
      Serial.println("HTTP m_server started");
    }
}

void printLocalTime(void)
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  Serial.println(timeinfo.tm_yday);
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

boolean captivePortal(void) {
  if (!isIp(m_server.hostHeader()) && m_server.hostHeader() != (String(myHostname)+".local")) {
    Serial.print("Request redirected to captive portal");
    m_server.sendHeader("Location", String("http://") + toStringIp(m_server.client().localIP()), true);
    m_server.send ( 302, "text/plain", ""); // Empty content inhibits Content-length header so we have to close the socket ourselves.
    m_server.client().stop(); // Stop is needed because we sent no content length
    return true;
  }
  return false;
}

void handleNotFound(void)
{
  if (captivePortal()) { // If caprive portal redirect instead of displaying the error page.
    return;
  }
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += m_server.uri();
  message += "\nMethod: ";
  message += ( m_server.method() == HTTP_GET ) ? "GET" : "POST";
  message += "\nArguments: ";
  message += m_server.args();
  message += "\n";

  for ( uint8_t i = 0; i < m_server.args(); i++ ) {
    message += " " + m_server.argName ( i ) + ": " + m_server.arg ( i ) + "\n";
  }
  m_server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  m_server.sendHeader("Pragma", "no-cache");
  m_server.sendHeader("Expires", "-1");
  m_server.send ( 404, "text/plain", message );
}

void handleRoot(void)
{
  if(m_app_state != STATE_WIFI_CLIENT_CONNECTED) {
    String s = SETUP_page;
    m_server.send(200, "text/html", s);
  } else{
    String s = CTRL_page;
    m_server.send(200, "text/html", s);
  }
}

void handleGetState(void)
{
  String s;

  if(DOOR_STATE_OPEN == m_door_state) {
    s = "Auf";
  } else if(DOOR_STATE_CLOSED == m_door_state) {
    s = "ZU";
  } else if(DOOR_STATE_TRAVEL == m_door_state) {
    s = "Fährt";
  } else {
    s = "?";
  }
  m_server.send(200, "text/plane", s);
}

void handleGetNextDateTimeCtrl(void)
{
  
  const sunriseset_t *sunriseset = NULL;
  time_t ltime;
  time(&ltime);
  tm * ptm = localtime(&ltime);

  int day = ptm->tm_yday;

  if (day < 0 || day > ARRAYSIZE(g_sun_rise_set)) {
      day = 364 / 2;
  }

  sunriseset = &g_sun_rise_set[day];
  int t = (ptm->tm_hour * 3600) + (ptm->tm_min * 60);

  char buf[100];
  
  sprintf(buf, "Auf: %02d:%02d  Zu: %02d:%02d", 
    sunriseset->rise / 3600, (sunriseset->rise / 60) % 60, 
    sunriseset->set / 3600, (sunriseset->set / 60) % 60
  );        
  
  m_server.send(200, "text/plane", buf);
  Serial.println(buf);
}

void handleGetApList(void)
{
  String s;

  int n = WiFi.scanNetworks();

  if (n == 0) {
    s = "";
  } else {
    for (int i = 0; i < n; ++i) {
      s += "<option>" + WiFi.SSID(i) + "</option>";
    }
  }
  m_server.send(200, "text/plane", s);
}

void handleSetWifi(void)
{
  Serial.println("setWifi");
  String ssid = m_server.arg("ssid");
  String pw = m_server.arg("pw");

  //Serial.println(ssid);
  //Serial.println(pw);

  strncpy(settings.ssid, ssid.c_str(), sizeof(settings.ssid));
  strncpy(settings.pw, pw.c_str(), sizeof(settings.pw));
  save_settings();

  m_app_state = STATE_WIFI_CLIENT_INIT;
}

void handleSetCtrl(void)
{

  m_test_enabled = 0;
  m_ctrl = m_server.arg("data").toInt();

  char buf[50];
  sprintf(buf, "setCtrl: %d", m_ctrl);

  Serial.println(buf);
}

void do_wifi(void)
{
  switch(m_app_state) {
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
      m_app_state = STATE_WIFI_ACTIVE;
      break;

    case STATE_WIFI_CLIENT_INIT:
      WiFi.disconnect();
      WiFi.mode(WIFI_STA);
      WiFi.begin(settings.ssid, settings.pw);
      init_server();
      m_app_state = STATE_WIFI_CLIENT_CONNECTING;
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

          m_app_state = STATE_WIFI_CLIENT_CONNECTED;
      }
      break;


    case STATE_WIFI_CLIENT_CONNECTED:
    case STATE_WIFI_ACTIVE:
      m_dns_server.processNextRequest();
      m_server.handleClient();
      break;
  }
}

void do_buttons(void)
{
  static unsigned long timeout = 0;

  if((millis() - timeout) > 50) {    
    if(digitalRead(TOP_SWITCH_PIN) == 0){
      m_buttons |= 1 << TOP_SWITCH_BIT;
    } else {
      m_buttons &= ~(1 << TOP_SWITCH_BIT);
    }
    if(digitalRead(BOTTOM_SWITCH_PIN) == 0){
      m_buttons |= 1 << BOTTOM_SWITCH_BIT;
    } else {
      m_buttons &= ~(1 << BOTTOM_SWITCH_BIT);
    }
    if(digitalRead(STOP_SWITCH_PIN) == 0){
      m_buttons |= 1 << STOP_SWITCH_BIT;
    } else {
      m_buttons &= ~(1 << STOP_SWITCH_BIT);
    }
    
    if(digitalRead(UP_BUTTON_PIN) == 0){
      m_buttons |= 1 << UP_BUTTON_BIT;
    } else {
      m_buttons &= ~(1 << UP_BUTTON_BIT);
    }
    if(digitalRead(DOWN_BUTTON_PIN) == 0){
      m_buttons |= 1 << DOWN_BUTTON_BIT;
    } else {
      m_buttons &= ~(1 << DOWN_BUTTON_BIT);
    }
    if(digitalRead(STOP_BUTTON_PIN) == 0){
      m_buttons |= 1 << STOP_BUTTON_BIT;
    } else {
      m_buttons &= ~(1 << STOP_BUTTON_BIT);
    }
    timeout = millis();
  }
/*
  String s = "";

  if(m_buttons & (1 << TOP_SWITCH_BIT)) {
    s += "TOP_SWITCH ";
  }
  if(m_buttons & (1 << BOTTOM_SWITCH_BIT)) {
    s += "BOTTOM_SWITCH ";
  }
  if(m_buttons & (1 << STOP_SWITCH_BIT)) {
    s += "STOP_SWITCH ";
  }
  if(m_buttons & (1 << UP_BUTTON_BIT)) {
    s += "UP_BUTTON ";
  }
  if(m_buttons & (1 << DOWN_BUTTON_BIT)) {
    s += "DOWN_BUTTON ";
  }
  if(m_buttons & (1 << STOP_BUTTON_BIT)) {
    s += "STOP_BUTTON ";
  }

  Serial.println(s);
*/
}

void do_display(void)
{
  static unsigned long timeout = 0;

  switch(m_app_state) {
    case STATE_DEFAULT:
    case STATE_WIFI_CLIENT_CONNECTED:
      m_u8x8.drawString(0, 0, "Chicken Guard");
      break;
    case STATE_WIFI_AP_INIT:
      m_u8x8.drawString(0, 0, "Wifi Setup  ");
      break;
  }

  switch(m_app_state) {
    case STATE_DEFAULT:
    case STATE_WIFI_CLIENT_CONNECTED:
    case STATE_WIFI_ACTIVE: {
      if((millis() - timeout) > 1000) {
        char buf[30];
        time_t ltime;
        time(&ltime);
        tm * ptm = localtime(&ltime);
        strftime(buf, 32, "%H:%M:%S", ptm);
        m_u8x8.drawString(0, 2, buf);
        sprintf(buf, "State: %d  ", m_app_state);
        m_u8x8.drawString(0, 4, buf);
        timeout = millis();
      }
      break;
    }
  }
}

void do_ntp(void)
{
    static int ntp_state = 0;
    static unsigned long timeout = 0;

    switch(m_app_state) {
      case STATE_WIFI_CLIENT_CONNECTED:
        if(ntp_state == 0)
          ntp_state = 1;
        break;
      default:
        ntp_state = 0;
    }

    switch(ntp_state) {
      case 1: {
        configTime(m_gmt_offset_sec, m_daylight_offset_sec, m_ntp_server);
        printLocalTime();
        ntp_state++;
        break;
      }
      case 2: {
        if((millis() - timeout) > 1000) {
          //printLocalTime();
          timeout = millis();
        }
        break;
      }
    }
}

void do_ctrl(void)
{
  static unsigned long timeout = 0;
  static unsigned long travel_start_time = 0;
  static int mot_ctrl = 0;
  static int last_ctrl = 0;
  static int duty = 0;
  static int last_buttons = 0;
  static int last_time_ctrl = 0;

  if(m_ctrl != last_ctrl) {
    mot_ctrl = MOT_CTRL_STOP;
    last_ctrl = m_ctrl;
  }

  // date time control sun rise or set
  if(m_time_ctrl != last_time_ctrl) {
    m_ctrl = m_time_ctrl;
    Serial.println("Time ctrl........................................");
    last_time_ctrl = m_time_ctrl;
  }

  // button control
  if(m_buttons != last_buttons) {
    if(m_buttons & (1 << UP_BUTTON_BIT)) {
      m_ctrl = CTRL_OPEN;
    } else if(m_buttons & (1 << DOWN_BUTTON_BIT)) {
      m_ctrl = CTRL_CLOSE;
    } else if(m_buttons & (1 << STOP_BUTTON_BIT)) {
      m_ctrl = CTRL_STOP;
    }

    last_buttons = m_buttons;
  }

  // limit switch check
  if(m_ctrl == CTRL_OPEN) {
    if(m_buttons & (1 << TOP_SWITCH_BIT)) {
      m_ctrl = CTRL_STOP;
      mot_ctrl = MOT_CTRL_STOP;
      last_ctrl = 0;
      m_door_state = DOOR_STATE_OPEN;
    }
  }

  // limit switch check
  if(m_ctrl == CTRL_CLOSE) {
    if(m_buttons & (1 << BOTTOM_SWITCH_BIT)) {
      m_ctrl = CTRL_STOP;
      mot_ctrl = MOT_CTRL_STOP;
      last_ctrl = 0;
      m_door_state = DOOR_STATE_CLOSED;
    }
  }
  
  // stop switch check
  if(m_buttons & (1 << STOP_SWITCH_BIT)) {
    m_ctrl = CTRL_STOP;
    mot_ctrl = MOT_CTRL_STOP;
    last_ctrl = 0;
  }
/*
  // travel time check
  if(m_ctrl == CTRL_STOP) {
    travel_start_time = 0;
  }
  
  if(travel_start_time) {
    if((millis() - travel_start_time) > settings.maxTravelTime) {
      m_ctrl = CTRL_STOP;
      mot_ctrl = MOT_CTRL_STOP;
      last_ctrl = 0;
    }
  }
*/
  switch(mot_ctrl) {
    case MOT_CTRL_INIT:
      mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, MOT_PWM_PIN);    //Set GPIO as PWM0A, to which Motor is connected

      mcpwm_config_t pwm_config;
      pwm_config.frequency = 20000;    //frequency Hz
      pwm_config.cmpr_a = 0;    //duty cycle of PWMxA = 0
      pwm_config.cmpr_b = 0;    //duty cycle of PWMxb = 0
      pwm_config.counter_mode = MCPWM_UP_COUNTER;
      pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
      mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config); //Configure PWM0A & PWM0B with above settings
      mot_ctrl = MOT_CTRL_STOP;
      break;

    case MOT_CTRL_STOP:
      if((millis() - timeout) > 1) {
        if(duty > 0) {
          duty -= 5;
          if(duty < 0) {
            duty = 0;            
          }
          mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, duty);
        }
        timeout = millis();
      }
      if(duty == 0) {
        mot_ctrl = MOT_CTRL_IDLE;
      }
      break;

    case MOT_CTRL_IDLE:
        if(m_ctrl == CTRL_OPEN) {
          digitalWrite(MOT_DIR_PIN, MOT_DIR_OPEN);          
          mot_ctrl = MOT_CTRL_OPEN;
          travel_start_time = millis();
          m_door_state = DOOR_STATE_TRAVEL;
        } else if(m_ctrl == CTRL_CLOSE) {
          digitalWrite(MOT_DIR_PIN, MOT_DIR_CLOSE);          
          mot_ctrl = MOT_CTRL_CLOSE;
          travel_start_time = millis();
          m_door_state = DOOR_STATE_TRAVEL;
        }
      break;

    case MOT_CTRL_OPEN:
    case MOT_CTRL_CLOSE:
      if((millis() - timeout) > 30) {
        mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, duty);
        if(duty < 80) {
          duty++;
        }
        timeout = millis();
      }
      break;
  }
}

void do_sunriseset_ctrl(void)
{
    const sunriseset_t *sunriseset = NULL;
    time_t ltime;
    time(&ltime);
    tm * ptm = localtime(&ltime);

    int day = ptm->tm_yday;

    if (day < 0 || day > ARRAYSIZE(g_sun_rise_set)) {
        day = 364 / 2;
    }

    sunriseset = &g_sun_rise_set[day];

    int t = (ptm->tm_hour * 3600) + (ptm->tm_min * 60);

    if (t >= sunriseset->rise && t < sunriseset->set) {
        //if(m_door_state == DOOR_STATE_CLOSED) {
          m_time_ctrl = CTRL_OPEN;
        //}
    }
    if (t >= sunriseset->set) {
      //if(m_door_state == DOOR_STATE_OPEN) {
        m_time_ctrl = CTRL_CLOSE;
      //}
    }
/*
Serial.println("Sun");
Serial.println(t);
Serial.println(sunriseset->rise);
Serial.println(m_time_ctrl);
*/
}

void do_time_test(void)
{

  static int lock = 0;

  if(lock == 0) {
//    do_ntp();
    time_t ltime;
    time(&ltime);
    tm * ptm = localtime(&ltime);
    
    
    ptm->tm_year = 2019 - 1900;
    ptm->tm_mon = 3 - 1;
    ptm->tm_mday = 15;
    ptm->tm_hour = 6;
    ptm->tm_min = 38;
    ptm->tm_sec = 40;
    
    time_t t = mktime(ptm);    
    struct timeval now = { .tv_sec = t };        
    settimeofday(&now, NULL);
    
    printLocalTime();    
    lock++;
  } else if(lock == 1 && m_door_state == DOOR_STATE_OPEN) {
    time_t ltime;
    time(&ltime);
    tm * ptm = localtime(&ltime);
    
    
    ptm->tm_year = 2019 - 1900;
    ptm->tm_mon = 3 - 1;
    ptm->tm_mday = 15;
    ptm->tm_hour = 18;
    ptm->tm_min = 27;
    ptm->tm_sec = 40;
    
    time_t t = mktime(ptm);    
    struct timeval now = { .tv_sec = t };        
    settimeofday(&now, NULL);
    
    printLocalTime();    
    lock++;
  } else if(lock == 2 && m_door_state == DOOR_STATE_CLOSED) {
    lock = 0;
  }
}

void do_test(void)
{
/*
  static unsigned long timeout = 0;

  if((m_test_enabled & (millis() - timeout) > 4000)) {
    if(m_door_state == DOOR_STATE_CLOSED) {
      float distance = get_distance();
      m_ctrl = 1;
      Serial.println(distance);
    } else if(m_door_state == DOOR_STATE_OPEN) {
      float distance = get_distance();
      m_ctrl = 2;
      Serial.println(distance);
    }
    timeout = millis();
  }
*/
}

void setup(void)
{
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
  pinMode(STOP_BUTTON_PIN, INPUT);

  Serial.begin(115200);
  Serial.println("");

  m_u8x8.begin();
  m_u8x8.setFont(u8x8_font_chroma48medium8_r);
  m_u8x8.clear();
  m_u8x8.drawString(0, 0, "Starting...");

  dcf_setup();

  init_settings();

  if(strlen(settings.ssid) > 0 && strlen(settings.pw) > 0) {
    m_app_state = STATE_WIFI_CLIENT_INIT;
  } else {
    m_app_state = STATE_WIFI_AP_INIT;
  }
}

void loop(void)
{
  do_sunriseset_ctrl();
  do_test();
  do_buttons();
  do_ctrl();
  //do_ntp();
  do_dcf_decoding();
  //do_time_test();
  do_display();
  do_wifi();
}
