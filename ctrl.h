const char CTRL_page[] PROGMEM = R"=====(
<style>

label {
  display: block;
}

input {
  border: 1px solid #c4c4c4;
  border-radius: 5px;
  background-color: #fff;
  padding: 3px 5px;
  box-shadow: inset 0 3px 6px rgba(0,0,0,0.1);
  width: 190px;
}

button{
    border:0;
    border-radius:0.3rem;
    background-color:#1fa3ec;
    color:#fff;
    line-height:2.4rem;
    font-size:5.2rem;
    width:100%;
    height: 100%;
}

.statusframe{
    border:0;
    border-radius:0.3rem;
    background-color:grey;
    color:#fff;
    width:150%;
}

.statusText{
    color:#fff;    
    font-size:3.2rem;  
    text-align: center;
    font-family: Trebuchet, Arial, sans-serif;
    text-align: center;
    vertical-align: top;
}

.flipswitch {
  position: relative;
  width: 100%;
  -webkit-user-select:none;
  -moz-user-select:none;
  -ms-user-select: none;
}
.flipswitch input[type=checkbox] {
  display: none;
}
.flipswitch-label {
  display: block;
  overflow: hidden;
  cursor: pointer;
  border: 1px solid #999999;
  border-radius: 6px;
  height:50px;
}
.flipswitch-inner {
  width: 200%;
  margin-left: -100%;
  -webkit-transition: margin 0.3s ease-in 0s;
  -moz-transition: margin 0.3s ease-in 0s;
  -ms-transition: margin 0.3s ease-in 0s;
  -o-transition: margin 0.3s ease-in 0s;
  transition: margin 0.3s ease-in 0s;
}
.flipswitch-inner:before, .flipswitch-inner:after {
  float: left;
  width: 50%;
  height: 50px;
  padding: 0;
  line-height: 40px;
  font-size: 18px;
  color: white;
  font-family: Trebuchet, Arial, sans-serif;
  font-weight: bold;
  -moz-box-sizing: border-box;
  -webkit-box-sizing: border-box;
  box-sizing: border-box;
}
.flipswitch-inner:before {
  content: "Auto";
  padding-left: 12px;
  background-color: #1fa3ec;
  color: #FFFFFF;
}
.flipswitch-inner:after {
  content: "Manuell";
  padding-right: 12px;
  background-color: #EBEBEB;
  color: #888888;
  text-align: right;
}
.flipswitch-switch {
  width: 31px;
  margin: 6.5px;
  background: #FFFFFF;
  border: 1px solid #999999;
  border-radius: 6px;
  position: absolute;
  top: 0;
  bottom: 0;
  right: 76%;
  -webkit-transition: all 0.3s ease-in 0s;
  -moz-transition: all 0.3s ease-in 0s;
  -ms-transition: all 0.3s ease-in 0s;
  -o-transition: all 0.3s ease-in 0s;
  transition: all 0.3s ease-in 0s;
}
.flipswitch-cb:checked + .flipswitch-label .flipswitch-inner {
  margin-left: 0;
}
.flipswitch-cb:checked + .flipswitch-label .flipswitch-switch {
  right: 0;
}

html, body, #wrapper {
  height:100%;
  margin: 0;
  padding: 0;
  border: none;
  text-align: center;
}
#wrapper {
  margin: 0 auto;
  text-align: left;
  vertical-align: middle;
  width: 100%px;
}

</style>

<!DOCTYPE html>
<html>
<body onLoad = "init()">

<div id = "mobile-container" class="">
  <table id="wrapper" >
    <tr>
      <td rowspan = "4" class="statusFrame"><div id = "state" align="center">
  
  
<!--  <table class="statusText">
  <tr><th style="color: black;">MODUS</th></tr>
  <tr><td>Zeitsteuerung</td></tr>
  <tr><th style="color: black;">ZUSTAND</th></tr>
  <tr><td>Auf</td></tr>
  <tr><th style="color: black;">ZEITSTEUERUNG</th></tr>
  <tr><td> Auf: 06:38 Uhr</td></tr>
  <tr><td> Zu : 18:42 Uhr</td></tr>
  </table>    -->
       
  
      </div></td>
      <td><button type="submit" id="CTRL1" onclick="setCtrl(2)">AUF</button></td>
    </tr>
    <tr>
      <td><button type="submit" id="CTRL2" onclick="setCtrl(1)">ZU</button></td>
    </tr>
    <tr>
      <td><button type="submit" id="CTRL0" onclick="setCtrl(0)" style="background: #FF0000">STOP</button></td>
    </tr>
    <tr>
      <td>
        <div class="flipswitch"><div class="flipswitch">
          <input type="checkbox" name="flipswitch" class="flipswitch-cb" id="fs" onclick="checkDateTimeCtrl()" unchecked>
          <label class="flipswitch-label" for="fs">
              <div class="flipswitch-inner"></div>
              <div class="flipswitch-switch"></div>
          </label>
        </div>
      </td>
    </tr>
  </table>
</div>

<script>

function init() {
  var element = document.getElementById("mobile-container");
/*
  if(!isMobileDevice()) {
      element.classList.add("mobile-container");
  }
*/
  getSettings();
}

function isMobileDevice() {
    return (typeof window.orientation !== "undefined") || (navigator.userAgent.indexOf('IEMobile') !== -1);
};

function setCtrl(data) {
  var s = "data=" + data;
  var xhttp = new XMLHttpRequest();
  xhttp.open("POST", "setCtrl", true);
  xhttp.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');
  xhttp.send(s);
}

function checkDateTimeCtrl() {
  var checkbox = document.getElementById('fs');
  if (checkbox.checked != true) {
    setCtrl(100);   // Manuell
    //alert(100);
  } else {
    setCtrl(101);   // Zeitsteuerung
    //alert(101);
  }
}

setInterval(function() {

  getState();
  //test();

}, 500); //500mSeconds update rate

function getState() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      updateState(this.responseText);
    }
  };
  xhttp.open("GET", "getState", true);
  xhttp.send();
}

function updateState(s) {
  // control mode, door state, open time, close time
  //var s = "0;2;06:00;18:00";

  var r = s.split(";");
  var ctrl_mode_str = "";
  var ctrl_mode = parseInt(r[0]);
  var door_state_str = "";
  var door_state = parseInt(r[1]);

  if(ctrl_mode == 0) {
    ctrl_mode_str = "Manuell";
  } else if(ctrl_mode == 1) {
    ctrl_mode_str = "Zeitsteuerung";
  }

  if(door_state == 1) {
    door_state_str = "Auf";
  } else if(door_state == 2) {
    door_state_str = "Zu";
  } else if(door_state == 3) {
    door_state_str = "F&auml;hrt";
  } else {
    door_state_str = "Steht";
  }

  var h = "";
  /*
  <table class="statusText">
  <tr><th style="color: black;">Modus</th></tr>
  <tr><td>Zeitsteuerung</td></tr>
  <tr><th style="color: black;">Zustand</th></tr>
  <tr><td>Auf</td></tr>
  <tr><th style="color: black;">Zeitsteuerung</th></tr>
  <tr><td> Auf: 06:38 Uhr</td></tr>
  <tr><td> Zu : 18:42 Uhr</td></tr>
</table> 
  */
  
  h += "<table class='statusText'>";
  h += "<tr><th style='color: black;'>MODUS</th></tr>";
  h += "<tr><td>" + ctrl_mode_str + "</td></tr>";
  h += "<tr><th style='color: black;'>ZUSTAND</th></tr>";
  h += "<tr><td>" + door_state_str + "</td></tr>";
  if(ctrl_mode == 1) {
    h += "<tr><th style='color: black;'>ZEITSTEUERUNG</th></tr>";
    h += "<tr><td> Auf:" + r[2] + " Uhr</td></tr>";
    h += "<tr><td> Zu :" + r[3] + " Uhr</td>";
  }  
  h += "</tr></table>";

  document.getElementById("state").innerHTML = h;
}

function getNextDateTimeCtrl() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("state").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "getNextDateTimeCtrl", true);
  xhttp.send();
}

function getSettings() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      updateSettings(this.responseText);
    }
  };
  xhttp.open("GET", "getSettings", true);
  xhttp.send();
}

function updateSettings(s) {
  // // ctrlMode; maxTravelDuration; stopDuration

  var r = s.split(";");
  var ctrl_mode = parseInt(r[0]);

  document.getElementById("fs").checked = (ctrl_mode == 1);
}

function test() {
  var h ="";
  h += "<table class='statusText'>";
  h += "<tr><th style='color: black;'>Modus</th></tr>";
  h += "<tr><td>Zeitsteuerung</td></tr>";
  h += "<tr><th style='color: black;'>Zustand</th></tr>";
  h += "<tr><td>AUF</td></tr>";
  if(1) {
  h += "<tr><th style='color: black;'>Zeitsteuerung</th></tr>";
  h += "<tr><td> Auf: 06:38 Uhr</td></tr>";
  h += "<tr><td> Zu : 18:34 Uhr</td>";
  }  
  h += "</tr></table>";

  document.getElementById("state").innerHTML = h;

}

</script>
</body>
</html>
</html>
)=====";
