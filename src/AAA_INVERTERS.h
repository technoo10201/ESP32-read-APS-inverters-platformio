#ifndef AAA_INVERTERS_H
#define AAA_INVERTERS_H

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>


const char INVCONFIG_START[] PROGMEM = R"=====(
<!DOCTYPE html><html><head><meta charset='utf-8'>
<title>ESP32-ECU</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="icon" type="image/x-icon" href="/favicon.ico" />
<link rel="stylesheet" type="text/css" href="/STYLESHEET">
<script type='text/javascript'>

function helpfunctie(){
document.getElementById("help").style.display = "block";
}
function sl(){  
document.getElementById("help").style.display = "none";
}
function showSubmit(){
document.getElementById("sub").style.display = "block";
}
function submitFunction(a){
document.getElementById("bo").innerHTML="<br>wait...<br>saving<br>this<br>inverter"; 
document.getElementById("bo").style.display="block"; 
document.getElementById('formulier').submit();
setTimeout(function(){window.location.href=a;}, 3000 ); 
}
</script>
<style>
.cap {
  font-weight:bold; 
  Background-color:lightgreen;
 }
div.overlay {
  display: block;
  width: 100%;
  height: 100%;
  background-color: rgba(0,0,0,0.7);
  z-index: 0;
  text-align: center;
  vertical-align: middle;
  line-height: 300px;
}
</style>
</head>
<body onload='%LOADBAG%'>

<div id='msect'>
  <div id="help">
  <span class='close' onclick='sl();'>&times;</span><h3>INVERTER SETTINGS HELP</h3>
  <b>you can add max 9 inverters.</b><br><br>
  <b>name</b><br>
  Enter a meaningfull name or the position of the inverter.
  <br><br><b>inverter serialnr:</b><br>
  You can find the serialnr stickered on the inverter.
  <br><br><b>panels:</b><br>
  Check which panels are connected.
  <br><br><b>pair:</b><br>
  link your inverter to this ECU in order to make the zigbee communication possible.<br>
  When paired you will see the obtained inverter ID in the status field.
  You can find more information in the log or information page. 
  <br><br>
  </div>
</div>

<center>

<div id='msect'>
<ul>
<li><a href='/MENU'>done</a></li>
<li><a href='/INV?welke=0' style='display:%none'0%>inv. 0</a></li>
<li><a href='/INV?welke=1' style='display:%none'1%>inv. 1</a></li>
<li><a href='/INV?welke=2' style='display:%none'2%>inv. 2</a></li>
<li><a href='/INV?welke=3' style='display:%none'3%>inv. 3</a></li>
<li><a href='/INV?welke=4' style='display:%none'4%>inv. 4</a></li>
<li><a href='/INV?welke=5' style='display:%none'5%>inv. 5</a></li>
<li><a href='/INV?welke=6' style='display:%none'6%>inv. 6</a></li>
<li><a href='/INV?welke=7' style='display:%none'7%>inv. 7</a></li>
<li><a href='/INV?welke=8' style='display:%none'8%>inv. 8</a></li>
<li><a href='/INV?welke=99' style='display:%none'99%>add</a></li>
</ul>
</div>

<div id='msect'>
<div id='bo'></div>
  <center><div class='divstijl' style='height:64vh;'>
<form id='formulier' method='get' action='inverterconfig' oninput='showSubmit()' onsubmit="return confirm('sure to save this inverter?')">
  
    %<FORMPAGE>%
    <br>
  </div>
</div>
<div id='msect'>
  <form id='formular' method='get' action='/INV_DEL'></form>
  <ul>
  <div id='pairknop' style='display:%none'p% >
    <li ><a class='groen' href='/PAIR' onclick="return confirm('Are you sure you want to pair this inverter?')">pair</a></li>
    <li><a href='#' onclick='delFunction("/SW=BACK")'>delete</a></li>
  </div>
    <li><a href='#' onclick='helpfunctie()'>help</a></li>
  
    <li id='sub'><a href='#' onclick='submitFunction("/SW=BACK")'>save</a></li>
  
</div>  
</ul>
<br>
  
</div>
<script type="text/javascript" src="INVSCRIPT"></script>
</body></html>
 )=====";

 const char INVERTER_GENERAL[] PROGMEM = R"=====(
<div id='inverter0' style='display:block'>
    <table><tr><td style='width:160px;'><h4>INVERTER {nr}</h4>
    <td style='width:70px;'><h4>STATUS:</h4><td>
    <input style='width:100px;' class='inp3' value='unpaired' readonly></tr></table>
        
    <br>
    <table style="background-color: lightgreen; padding:10px;">
    
    <tr><td class="cap" style="width:100px;">SERIALNR<td><input class='inp4' id='iv' name='iv' minlength='12' maxlength='12' required value='000000'></input>
    <tr><td class="cap">TYPE<td><select name='invt' class='sb1' id='sel' onchange='myFunction()'>
    <option value='0' invtype_0>YC600</option>
    <option value='2' invtype_2>DS3</option>
    <option value='1' invtype_1>QS1</option></select>
    </tr>
    <tr><td class="cap" >NAME<td class="cap" ><input class='inp4' id='il' name='il' maxlength='12' value='{location}'></input>
    <tr><td class="cap" >DOM. IDX<td class="cap" ><input class='inp2' name='mqidx' value='{idx}' size='4' length='4'></td></tr>
    <tr><td class="cap" >PANELS: 
    <td style='width: 230px;'>
    1&nbsp;<input type='checkbox' name='pan1' #1check>
    2&nbsp;<input type='checkbox' name='pan2' #2check>
    <span id='invspan'>
    3&nbsp;<input type='checkbox' name='pan3' #3check>
    4&nbsp;<input type='checkbox' name='pan4' #4check></tr></td>
    </span>

</table></form>
)=====";

// **********************************************************************************
//                         script
// **********************************************************************************

const char INV_SCRIPT[] PROGMEM = R"=====(
function showFunction(){
  //alert("showFunction");
  document.getElementById("invspan").style.display = "inline";
}
function hideFunction(){
  //alert("showFunction");
  document.getElementById("invspan").style.display = "none";
}
function myFunction(){
 if(document.getElementById("sel").value == 1 ){ 
    showFunction();
 } else {
   hideFunction();
 }
}

function delFunction(a){
  if(confirm("are you sure to delete this inverter ?")){  
  document.getElementById("bo").innerHTML="<br>wait...<br>processing<br>your<br>request"; 
  document.getElementById("bo").style.display="block";
  document.getElementById('formular').submit();   
  setTimeout(function(){window.location.href=a;}, 3000 ); 
  }
}

)====="; 


void handleInverterconfig(AsyncWebServerRequest *request);
void handleInverterdel(AsyncWebServerRequest *request);
void printInverters(); 
void remove_gaps();
String processor(const String& var);
void inverterForm();



#endif