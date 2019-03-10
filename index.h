const char MAIN_page[] PROGMEM = R"=====(
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
  height: 40px;
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
  content: "Manual";
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
</style>
<body onLoad = "fillApList()">

<h1>Chicken Guard Setup</h1>
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
</table>
<script>

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

setInterval(function() {
    // Call a function repetatively
    // getDate();
    
    if(document.getElementById("dateFlipswitch").checked) {
        var date = new Date();
        var currentDate = date.toISOString().slice(0,10);
        var currentTime = date.toTimeString().split(' ')[0];        
        document.getElementById("nowdate").value = currentDate;
        document.getElementById("nowtime").value = currentTime;
    }
}, 1000); //1000mSeconds update rate

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
