#ifndef CONFIG_GEO_H
#define CONFIG_GEO_H

#include <Arduino.h>
#include <ESPAsyncWebServer.h>

const char GEOCONFIG[] PROGMEM = R"=====(
<body>
  <div id='msect'>
    <div id='bo'></div>
    <div id='help'>
      <span class='close' onclick='sl();'>&times;</span>
      <h3>GEOGRAPHICAL SETTINGS HELP</h3>
      This data is necessary to determine the correct time and to calculate the times of sunset and sunrise.
      <br><br>
      <b>Latitude and longitude</b>
      <br>
      The location of this device. (google) Not more than 3 digits.
      <br><br>
      <b>Timezone:</b>
      <br>minutes offset relative to Greenwich Main Time.
      <br>This can be a "+" or a "-" value
      <br><br>
      <b>DST:</b>
      <br>Check when daylight saving time applies.
      <br><br>
    </div>
  </div>
  <div id='msect'>
    <ul>
      <li><a href='/MENU'>back<a></li>
    </ul>
  </div>
  <div id='msect'>
    <br><kop>GEOGRAPHICAL SETTINGS</kop><br><br>
  </div>
  <div id='msect'>
    <div class='divstijl' style='height:62vh;'>
      <form id='formulier' method='get' action='geoconfig' oninput='showSubmit()'>
        <center>
          <table>
            <tr>
              <td style='width:140px;'>latitude &deg</td>
              <td><input class='inp3' name='be' length='8' placeholder='latitude' value='{be}'></input></td>
            </tr>
            <tr>
              <td>longitude &deg</td>
              <td><input class='inp3' name='le' length='8' placeholder='longitude' value='{le}'></input></td>
            </tr> 
            <tr>
              <td>timezone</td>
              <td><input class='inp2' name='tz' length='6' placeholder='minutes relative to GMT' value='{tz}'></input></td>
            </tr>
            <tr>
              <td>dst y/n</td>
              <td><input type='checkbox' style='width:30px; height:30px;' name='ts' #check></input></td>
            </tr>
          </table> 
        </center>
      </form>
    </div>
  </div>
  <br>
  <div id='msect'>
    <ul>
      <li id='sub'><a href='#' onclick='submitFunction("/SW=BACK")'>save</a></li>
      <li><a href='/MENU'>done</a></li>
      <li><a href='#' onclick='helpfunctie()'>help</a></li>
    </ul>
  </div>
  <br>
</body>
</html>
)=====";

void zendPageGEOconfig();
void handleGEOconfig(AsyncWebServerRequest *request);
#endif
