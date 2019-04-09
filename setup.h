const char SETUP_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<style>
[type="date"] {
  background:#fff url('calendar_2.png')  97% 50% no-repeat ;
}
[type="date"]::-webkit-inner-spin-button {
  display: none;
}
[type="date"]::-webkit-calendar-picker-indicator {
  opacity: 0;
}
/*
[type="time"] {
  background:#fff url('calendar_2.png')  97% 50% no-repeat ;
}
[type="time"]::-webkit-inner-spin-button {
  display: none;
}
[type="time"]::-webkit-time-picker-indicator {
  opacity: 0;
}
*/
/* custom styles */
body {
  padding: 4em;
  background: #e5e5e5;
  font-family:verdana;
}
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
table, th, td {
  border: none;
}
th, td {
  padding: 5px;
}
th {
  text-align: left;
}

button{
    border:0;
    border-radius:0.3rem;
    background-color:#1fa3ec;
    color:#fff;
    line-height:2.4rem;
    font-size:1.2rem;
    width:100%;
}.q{float: right;width: 64px;text-align: right;}

.mobile-container {
  max-width: 480px;
  margin: auto;
  background-color: #555;
  height: 500px;
  color: white;
  border-radius: 10px;
  padding:10px;
}

</style>
<body onLoad = "init()">

<div id = "mobile-container" class="">

<h1>Chicken Door Setup</h1>
<table>
  <tr>
    <th>SSID</th>
  </tr>
  <tr>
    <td><select name="ssid" id = "ssid" style="width: 100%"><option>None</option></select></td>
  </tr>
  <tr>
    <th>Password</th>
  </tr>
  <tr>
    <td><input type="password" name="pw" id="pw" value=""></td>
  </tr>
  <tr>
    <td><button type="submit" onclick="setWifi()">Submit</button></td>
  </tr>


  <tr>
    <th>Max Travel Duration</th>
  </tr>
  <tr>
    <td><input name="maxTravelDuration" id="maxTravelDuration" value=""></td>
  </tr>
  <tr>
    <th>Max Stop Duration</th>
  </tr>
  <tr>
    <td><input name="maxStopDuration" id="maxStopDuration" value=""></td>
  </tr>
  <tr>
    <td><button type="submit" onclick="setCtrlSettings()">Submit</button></td>
  </tr>
  <tr>
    <td><button type="submit" onclick="resetSettings()">Reset</button></td>
  </tr>

</table>

</div>

<script>

function init() {
  var element = document.getElementById("mobile-container");
  
  if(!isMobileDevice()) {
    element.classList.add("mobile-container");
  }
  
  fillApList();
  getSettings();
}

function isMobileDevice() {
    return (typeof window.orientation !== "undefined") || (navigator.userAgent.indexOf('IEMobile') !== -1);
};

function sendData(data) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("LEDState").innerHTML =
      this.responseText;
    }
  };
  xhttp.open("GET", "setData?data=" + data, true);
  xhttp.send();
}

function setWifi() {
  var s = "ssid=" + document.getElementById("ssid").value;
  s += "&pw=" + document.getElementById("pw").value;

  var xhttp = new XMLHttpRequest();
  xhttp.open("POST", "setWifi", true);
  xhttp.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');
  xhttp.send(s);
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
  var max_travel_duration = parseInt(r[1]);
  var stop_duration = parseInt(r[2]);

  document.getElementById("maxTravelDuration").value = max_travel_duration;
  document.getElementById("maxStopDuration").value = stop_duration;
}

function setCtrlSettings() {
  var s = "maxTravelDuration=" + document.getElementById("maxTravelDuration").value;
  s += "&maxStopDuration=" + document.getElementById("maxStopDuration").value;

  var xhttp = new XMLHttpRequest();
  xhttp.open("POST", "setCtrlSettings", true);
  xhttp.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');
  xhttp.send(s);
}

function resetSettings() {
  var s = "";

  var xhttp = new XMLHttpRequest();
  xhttp.open("POST", "resetSettings", true);
  xhttp.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');
  xhttp.send(s);
}

function fillApList() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      var date = new Date();
      document.getElementById("ssid").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "getApList", true);
  xhttp.send();
}

</script>
</body>
</html>
)=====";
