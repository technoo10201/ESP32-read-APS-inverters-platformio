#ifndef AA_CONSOLE_H
#define AA_CONSOLE_H

#include <AsyncWebSocket.h>

//<link rel="icon" type="image/x-icon" href="/favicon.ico" />

const char CONSOLE_HTML[] PROGMEM = R"=====(
<!DOCTYPE html><html><head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
<title>ESP-ECU</title>
<meta name="viewport" content="width=device-width, initial-scale=1">

<link rel="stylesheet" type="text/css" href="/STYLESHEET_HOME">
<script>
function helpfunctie() {
document.getElementById("help").style.display = "block";
}
function sl() {  
document.getElementById("help").style.display = "none";
}

</script>

<style>
 tr {height:16px !important;
 font-size:15px !important;
 } 
 li a:hover {
   background-color: #333 !important;
}
#help {
  background-color: #ffffff; 
  border: solid 2px; 
  display:none; 
  padding:4px;
  width:94vw;
}
</style>
</head>
<body>
  <div id='help'>
  <span class='close' onclick='sl();'>&times;</span><h3>CONSOLE COMMANDS</h3>
  <b>10;ZBT=message: </b> send a zigbee message (e.g. 2710).<br><br>
  <b>10;DELETE=filename: </b> delete a file.<br><br>
  <b>10;INV_REBOOT: </b> reboot an unresponsive inverter<br><br>
  <b>10;HEALTH: </b> healthcheck zigbee hw/system<br><br>
  <b>10;POLL=x: </b> poll inverter #x<br><br>
  <b>10;INIT_N: </b> start the zigbee coordinator<br><br>
  <b>10;DIAG: </b> more Debug messages in console<br><br>
  <b>10;EDIT=0-AABB: </b> mark an inverter as paired<br><br>
  <b>10;ERASE: </b> delete all inverter files<br><br>
  <b>10;FILES: </b> show filesystem<br><br>
  <b>10;TESTMQTT: </b>sends a mqtt testmessage<br><br>  
  <b>10;CLEAR: </b> clear console window<br><br> 
  </div>

<div id='msect'>
<ul>
<li id='fright'><a href='/MENU' onclick='confirmExit()' class='close'>&times;</span></a>
<li><a href='#' onclick='helpfunctie()'>help</a>
<li><a><input type="text" placeholder="type here" id="tiep"></a>
</ul>
</div>  
<br>  
<div id='msect'>
  <div class='divstijl' style='height:84vh; border:1px solid; padding-left:10px;'>
  <table id='tekstveld'></table>
  </div>
 </div>

<script>
  var field = document.getElementById('tekstveld');
  var gateway = `${(window.location.protocol == "https:"?"wss":"ws")}://${window.location.hostname}/ws`;
  var websocket;
  var inputField = document.getElementById('tiep');

  window.onbeforeunload = confirmExit;
  function confirmExit()
  {
      alert("close the console?");
      ws.close();  
  }  
  
  window.addEventListener('load', onLoad);
  function initWebSocket() {
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen    = onOpen;
    websocket.onclose   = onClose;
    websocket.onmessage = onMessage; // <-- add this line
  }
  function onOpen(event) {
    console.log('Connection opened');
    field.insertAdjacentHTML('beforeend', "<tr><td>* * connection opened * *");
    inputField.focus();
    }
  function onClose(event) {
    console.log('Connection closed');
    field.insertAdjacentHTML('beforeend', "<tr><td>* * connection closed * *");
    //setTimeout(initWebSocket, 2000);
  }
  function onMessage(event) {
    //var message = event.data;
    field.insertAdjacentHTML('beforeend', "<tr><td>" + event.data );
    if (field.rows.length > 20) {
    var rtm = field.rows.length - 20;
    for (let x=0; x<rtm; x++) { field.deleteRow(0); }
  }
    if (event.data == "clearWindow") { 
    for (let i = 0; i < 22; i++) {
        field.deleteRow(0); }
    }
   }
 
  function onLoad(event) {
    initWebSocket();
    sendEvent();
  }

  function sendEvent() {
    inputField.addEventListener('keyup', function(happen) {
    if (happen.keyCode === 13) {
       happen.preventDefault();
       sendData();
       }   
    });
  }  
  function sendData(){
  var data = inputField.value; 
  websocket.send(data, 1);
  inputField.value = "";
  }

function disConnect() {
  alert("close the console");
  ws.close(); 
}
</script>
</body>
</html>
)=====";


void handleWebSocketMessage(void *arg, uint8_t *data, size_t len);
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
void initWebSocket();

#endif