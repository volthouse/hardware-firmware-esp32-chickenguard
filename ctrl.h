const char CTRL_page[] PROGMEM = R"=====(
<style>


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

.statusframe {
    border:0;
    border-radius:0.3rem;
    background-color:grey;
    color:#fff;   
}

.statusHeaderText {   
    font-size:1.5rem;  
    text-align: center;
    font-family: Trebuchet, Arial, sans-serif;    
}

.statusText {   
    font-size:2.5rem;  
    text-align: center;
    font-family: Trebuchet, Arial, sans-serif;    
}

.onoffswitch {
    position: relative; width: 100%;
    -webkit-user-select:none; -moz-user-select:none; -ms-user-select: none;    
}
.onoffswitch-checkbox {
    display: none;
}
.onoffswitch-label {
    display: block; overflow: hidden; cursor: pointer;
    border: 1px solid #999999; border-radius: 5px;margin:2px
}
.onoffswitch-inner {
    display: block; width: 200%; margin-left: -100%;
    transition: margin 0.3s ease-in 0s;
}
.onoffswitch-inner:before, .onoffswitch-inner:after {
    display: block; float: left; width: 50%; height: 50px; padding: 0; line-height: 50px;
    font-size: 14px; color: white; font-family: Trebuchet, Arial, sans-serif; font-weight: bold;
    box-sizing: border-box;
}
.onoffswitch-inner:before {
    content: "AUTO";
    padding-left: 10px;
    background-color: #34A7C1; color: #FFFFFF;
}
.onoffswitch-inner:after {
    content: "MANUELL";
    padding-right: 10px;
    background-color: #EEEEEE; color: #999999;
    text-align: right;
}
.onoffswitch-switch {
    display: block; width: 21px; margin: 14.5px;
    background: #FFFFFF;
    position: absolute; top: 0; bottom: 0;
    right: 90%;
    border: 2px solid #999999; border-radius: 5px;
    transition: all 0.3s ease-in 0s; 
}
.onoffswitch-checkbox:checked + .onoffswitch-label .onoffswitch-inner {
    margin-left: 0;
}
.onoffswitch-checkbox:checked + .onoffswitch-label .onoffswitch-switch {
    right: 0px; 
}

.wrapper { 
  text-align: left;
  vertical-align: middle;
  width: 100%;
  height:70%;
}

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

<!DOCTYPE html>
<html>
<body onLoad = "init()">

<div id = "mobile-container" class="">

  <table class="wrapper">      
    <tr>      
      <td><button type="submit" id="CTRL1" onclick="setCtrl(2)">AUF</button></td>
    <tr>      
      <td><button type="submit" id="CTRL2" onclick="setCtrl(1)">ZU</button></td>
    </tr>
    <tr>              
      <td><button type="submit" id="CTRL0" onclick="setCtrl(0)" style="background: #FF0000">STOP</button></td>
    </tr>
      <table id="statusTable" width="100%">
    
        <!-- <table width="100%">
          <td class="statusFrame">
            <div class="statusHeaderText">Zustand</div>
            <div id="stateText" class="statusText">Zu</div>
          </td>
          <td class="statusFrame"><div id="modeText" class="statusText">Auf</br>ZU</div></td>
        </table> -->        
        
          <td class="statusFrame">
            <div class="statusHeaderText">Zustand</div>
            <div id="stateText" class="statusText">Zu</div>
          </td>
          <td class="statusFrame"><div id="modeText" class="statusText">Auf</br>ZU</div></td>
      </table>
    <tr>      
      <td>
        <div class="onoffswitch">
            <input type="checkbox" name="onoffswitch" class="onoffswitch-checkbox" id="ctrlSwitch" onclick="checkDateTimeCtrl()" unchecked>
            <label class="onoffswitch-label" for="ctrlSwitch">
                <span class="onoffswitch-inner"></span>
                <span class="onoffswitch-switch"></span>
            </label>
        </div>
      </td>
    </tr>
  </table>

</div>
</body>
</html>

<script>
function init() {
  var element = document.getElementById("mobile-container");

  if(!isMobileDevice()) {
      element.classList.add("mobile-container");
  }
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
  var checkbox = document.getElementById('ctrlSwitch');
  if (checkbox.checked != true) {
    setCtrl(100);   // Manuell
    //alert(100);
  } else {
    setCtrl(101);   // Zeitsteuerung
    //alert(101);
  }
}

setInterval(function() {

  //getState();
  test();

}, 1000); //update rate ms

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
    ctrl_mode_str = "";
  } else if(ctrl_mode == 1) {
    ctrl_mode_str = "Auf: "  + r[2] + " Uhr</br>" + "Zu: " + r[3] + " Uhr";
  }

  if(door_state == 1) {
    door_state_str = "AUF";
  } else if(door_state == 2) {
    door_state_str = "ZU";
  } else if(door_state == 3) {
    door_state_str = "F&auml;hrt";
  } else {
    door_state_str = "Steht";
  }


  var timeCtrlTable =   
      "<tr>" +
        "<td class='statusFrame'>" +
          "<div class='statusHeaderText'>Zustand</div>" +
          "<div id='stateText' class='statusText'></div>" +
        "</td>" + 
        "<td class='statusFrame'><div id='modeText' class='statusText'></div></td>" +
      "</tr>";
    

  var manCtrlTable =   
      "<tr>" +
        "<td class='statusFrame'>" +
          "<div class='statusHeaderText'>Zustand</div>" +
          "<div id='stateText' class='statusText'></div>" +
        "</td>"        
      "</tr>";
  
  if(1) {
    document.getElementById("statusTable").innerHTML = timeCtrlTable;
    document.getElementById("stateText").innerHTML = door_state_str;
    document.getElementById("modeText").innerHTML = ctrl_mode_str;
  } else {
    document.getElementById("statusTable").innerHTML = manCtrlTable;
    document.getElementById("stateText").innerHTML = door_state_str;
  }
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

  document.getElementById("ctrlSwitch").checked = (ctrl_mode == 1);
}

function test() {
  var s = "1;2;06:00;18:00";
  updateState(s);
}

</script>
</body>
</html>
</html>
)=====";
