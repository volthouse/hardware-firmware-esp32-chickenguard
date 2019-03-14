const char CTRL_page[] PROGMEM = R"=====(
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
    height: 100%;
}

label{
    border:0;
    border-radius:0.3rem;
    background-color:#1fa3ec;
    color:#fff;
    line-height:2.4rem;
    font-size:1.2rem;
    width:100%; 
    height:100%;
    text-align: center;    
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

.mobile-container {
  max-width: 480px;
  margin: auto;
  background-color: #555;
  height: 500px;
  color: white;
  border-radius: 10px;
  padding:10px;
}

.item-header {
  grid-area: header;
  align-self: center;
}
.item-state {
background-color: #555;
border-radius: 10px;
  grid-area: state;  
}
.item-up {
  grid-area: up;
}
.item-down {
  grid-area: down;
}
.item-stop {
  grid-area: stop;
}

.container {
  display: grid;
  grid-template-columns: auto;
  grid-template-rows: auto;
  grid-gap: 15px 10px;  
  grid-template-areas:
    "state up"
    "state down"
    "state stop";
  width: 90vw;
  height: 90vh;
  align: stretch;
}

.arrows-down {
  position: relative;
  /**/
  
  top: 50%;
  left: 50%;
  /**/
  width: 30px;
  height: 30px;
  transform: translate(-30%, -30%);
}
.arrows-down:before {
  content: '';
  position: absolute;
  width: 100%;
  height: 100%;
  border-left: 26.66667px solid rgba(255, 0, 0, 0.7);
  border-bottom: 26.66667px solid rgba(255, 0, 0, 0.7);
  transform: translate(26.66667px, 106.66667px) rotate(-45deg);
  animation: arrows-down 3s linear infinite;
}
.arrows-down:after {
  content: '';
  position: absolute;
  width: 100%;
  height: 100%;
  border-left: 26.66667px solid rgba(255, 0, 0, 0.7);
  border-bottom: 26.66667px solid rgba(255, 0, 0, 0.7);
  transform: translate(53.33333px, 0px) rotate(-45deg);
  animation: arrows-down 3s linear infinite -1.5s;
}

@keyframes arrows-down {
  0% {
    border-left: 26.66667px solid transparent;
    border-bottom: 26.66667px solid transparent;
    transform: translate(-13.33333px, -53.33333px) rotate(-45deg);
  }
  10%, 90% {
    border-left: 26.66667px solid transparent;
    border-bottom: 26.66667px solid transparent;
  }
  50% {
    border-left: 26.66667px solid rgba(255, 0, 0, 0.7);
    border-bottom: 26.66667px solid rgba(255, 0, 0, 0.7);
    transform: translate(-13.33333px, 0px) rotate(-45deg);
  }
  100% {
    border-left: 26.66667px solid transparent;
    border-bottom: 26.66667px solid transparent;
    transform: translate(-13.33333px, 53.33333px) rotate(-45deg);
  }
}

.arrows-up {
  position: relative;
  /**/
  
  top: 30%;
  left: 50%;
  /**/
  width: 30px;
  height: 30px;
  transform: translate(-30%, -30%);
}
.arrows-up:before {
  content: '';
  position: absolute;
  width: 100%;
  height: 100%;
  border-left: 26.66667px solid rgba(255, 0, 0, 0.7);
  border-bottom: 26.66667px solid rgba(255, 0, 0, 0.7);
  transform: translate(26.66667px, 106.66667px) rotate(-45deg);
  animation: arrows-up 3s linear infinite;
}
.arrows-up:after {
  content: '';
  position: absolute;
  width: 100%;
  height: 100%;
  border-left: 26.66667px solid rgba(255, 0, 0, 0.7);
  border-bottom: 26.66667px solid rgba(255, 0, 0, 0.7);
  transform: translate(53.33333px, 0px) rotate(-45deg);
  animation: arrows-up 3s linear infinite -1.5s;
}

@keyframes arrows-up {
  100% {
    border-left: 26.66667px solid transparent;
    border-bottom: 26.66667px solid transparent;
    transform: translate(-13.33333px, -53.33333px) rotate(135deg);
  }
  50% {
    border-left: 26.66667px solid transparent;
    border-bottom: 26.66667px solid transparent;
  }
  10% {
    border-left: 26.66667px solid rgba(255, 0, 0, 0.7);
    border-bottom: 26.66667px solid rgba(255, 0, 0, 0.7);
    transform: translate(-13.33333px, 0px) rotate(135deg);
  }
  0% {
    border-left: 26.66667px solid transparent;
    border-bottom: 26.66667px solid transparent;
    transform: translate(-13.33333px, 53.33333px) rotate(135deg);
  }
}

.verticalhorizontal {  
    text-align: center;    
    position: relative;
    top: 50%;
    transform: translateY(-50%);
}

</style>
<body>
<!-- <div class="mobile-container"> -->

  
<div class="container">
  <div class="item-state" id = "state"><div class="arrows-down"></div></div>
  <div class="item-up"><button type="submit" id="CTRL1" onclick="setCtrl(1)">AUF</button></div>
  <div class="item-down"><button type="submit" id="CTRL2" onclick="setCtrl(2)">ZU</button></div>  
  <div class="item-stop"><button type="submit" id="CTRL0" onclick="setCtrl(0)" style="background: #FF0000">STOP</button></div>
</div>

</div>
<script>
pentitle="SCSS Arrow Animation";
function setCtrl(data) {
  var s = "data=" + data;
  var xhttp = new XMLHttpRequest();
  xhttp.open("POST", "setCtrl", true);
  xhttp.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');
  xhttp.send(s); 
}
var state = 0;
setInterval(function() {
    // Call a function repetatively
    // getState();
    switch(state) {
        case 0:            
            document.getElementById("state").innerHTML = '<div class="verticalhorizontal"><img src="closed.png"/></div>';
            break;
        case 1:
            document.getElementById("state").innerHTML = '<div class="verticalhorizontal"><img src="open.png"/></div>';
            break;
        case 2:
            document.getElementById("state").innerHTML = '<div class="arrows-up"></div>';
            break;
        case 3:
            document.getElementById("state").innerHTML = '<div class="arrows-down"></div>';
            break;

    }
    state = (state + 1) % 4;
}, 3000); //1000mSeconds update rate

function getState() {  
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) { 
      document.getElementById("state").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "getState", true);
  xhttp.send();
}
</script>
</body>
</html>
)=====";
