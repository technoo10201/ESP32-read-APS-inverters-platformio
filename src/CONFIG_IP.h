#ifndef CONFIG_IP_H
#define CONFIG_IP_H

#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ZIGBEE_HEALTH.h>
#include <SPIFFS_RW.h>


const char IPCONFIG[] PROGMEM = R"=====(
<body onload='showTable()'>

<script type="text/javascript">
function showTable(){
var tbl = document.getElementById('choice').value;
if (tbl == 'STAT'){
    document.getElementById('tabel').style.display="block";
  } else {
    document.getElementById('tabel').style.display="none";
  } 
}
function submitF(){
   document.getElementById('fM').submit();  
}
</script>
<div id='msect'>
  <div id='help'>
  <span class='close' onclick='sl();'>&times;</span><h3>IP ADDRESS HELP</h3>
  <b>STATIC IP:</b><br>You can configure a static ip. 
  Usually redundant, the dhcp ip is usually already fixed.
  <br><br>
  <b>DHCP IP:</b><br> the router determines the IP address.<br><br>
  <b>IP address:</b><br>
  Should be derived from the router's IP address. If ip router is 192.168.2.1, <br>a correct static ip is <br>192.168.2.<span style='color:red;'>2 t/m 254</span>
  <br><br>
  If you set a DHCP IP address, a valid IP must be entered, or leave this field empty.<br>
  <br><br>
  </div>
</div>

<div id='msect'>
  <kop>IP ADDRESS SETTINGS</kop>
</div>
<div id='main-sect'>
<div class='divstijl' style='height:62vh;'><br><center>
<form id='fM' method='get' action='IPconfig' oninput='showSubmit();'>
<table><tr><td style='width:145px;'>IP configuration<td style='width:190px;'>
<select name='keuze' class='sb1' id='choice' onchange='showTable()'>
<option value="DHCP" option1>DHCP IP</option>
<option value="STAT" option2>STATIC IP</option>
</select>
</td></table>

<div id='tabel'><table>
<tr><td style='width:145px;'>IP address<td style='width:190px;'>
<input class='inp5' name='ip' placeholder='leeg = DHCP' value='{ip}' pattern = '^\b({patroon})([0-9]{1,2}|1[0-9]{2}|2[0-4][0-9]|25[0-5])$' title='must be derived from ip router'>
</input></tr>
<tr><td>IP address router<td>
<input class='inp5' name='gw' value='{gw}' readonly></input></tr>
<tr><td>subnet mask<td>
<input class='inp5' name='sn' value='{sn}' readonly></input></tr>
</table>
</form>
<p>NOTE: a fixed IP has to be correct, or empty.</p></center>
</div>
</div>
<div id='msect'>
  <br><ul>
  <li id="sub"><a href="#" onclick='submitF()'>save</a></li>
  <li><a href='#' onclick='helpfunctie()'>help</a>
  <li><a href='/MENU'>done</a></li>
  </ul>
  <br></div>
</div>
</body></html>
)=====";

void zendPageIPconfig();
void handleIPconfig(AsyncWebServerRequest *request);

#endif