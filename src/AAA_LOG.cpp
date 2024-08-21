#include <main.h>

const char LOGPAGE[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
  <meta charset='utf-8'>
  <title>ESP32-ECU</title>
  <meta http-equiv="refresh" content="112">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" type="image/x-icon" href="/favicon.ico"/>
  <link rel="stylesheet" type="text/css" href="/STYLESHEET">
  <style>
    #lijst {
      font-family: "Trebuchet MS", Arial, Helvetica, sans-serif;
      border-collapse: collapse;
      width: 380px;
      font-size: 16px;
      border: 1px solid;
      text-align: left;
    }
    #lijst td {
      padding-left: 10px;
      border: 1px solid;
    }
    #lijst tr:nth-child(even) {
      background-color: #f2f2f2;
      border: 1px solid #ddd;
      padding: 6px;
    }
    #lijst th {
      padding-top: 5px;
      padding-bottom: 5px;
      padding-left: 10px;
      text-align: left;
      background-color: #4CAF50;
      color: white;
    }
    .th1 { width: 30%; }
    .th2 { width: 20%; }
    .th3 { width: 50%; }
    tr { height: 20px; }

    @media only screen and (max-width: 600px) {
      #lijst { font-size: 12px; width: 320px; }
      tr { width: 94vw; }
      .th1 { width: 25%; }
      .th2 { width: 15%; }
    }
  </style>
  <script type="text/javascript" src="SECURITY"></script>
</head>
<body>
  <center>
    <ul>
      <li><a href='/'>close</a></li>
      <li><a href='/ABOUT'>info</a></li>
      <li><a onclick="return confirm('are you sure?')" href='/CLEAR_LOG'>clear</a></li>
    </ul>
    <br>
    <kop>ESP32-ECU LOG</kop>
    <div class='divstijl'>
      <center>
        <table>
          <button onClick='window.location.reload();' style='width: 100px' value='0'>Refresh</button>
        </table>
        <table id='lijst'>
          <tr>
            <th class='th1'>Time</th>
            <th class='th2'>Type</th>
            <th class='th3'>Command</th>
          </tr>
)=====";

void handleLogPage(AsyncWebServerRequest *request) {
    Serial.println("zendPageLog");
    
    char page[3072] = {0};
    char temp[100] = {0};
    strcpy_P(page, LOGPAGE);
    
    uint32_t lengte = strlen(page);
    uint32_t added = 0;
    Serial.println("initial length = " + String(lengte));

    byte Log_Count = 0;
    Log_MaxReached ? Log_Count = Log_MaxEvents : Log_Count = logNr;
    Serial.println("log_Count = " + String(Log_Count));
    Serial.println("printlog current event = " + String(logNr));
    int j = logNr;
    
    for (int i = 1; i <= Log_Count; i++) {
        j--; 
        if (j == -1) j = Log_MaxEvents - 1; 

        sprintf(temp, "<tr><td>%s</td><td>%s</td><td>%s</td>", Log_date[j], Log_kind[j], Log_message[j]);
        strcat(page, temp);
        added = strlen(page) - lengte;
        Serial.println("length added = " + String(added));
        lengte = strlen(page);
    }
    
    strcat(page, "</table></center></div></body></html>"); 
    Serial.println("length = " + String(strlen(page)));
    request->send(200, "text/html", page); 
}

void Update_Log(char what[], char message[]) {
    char nu[14];
    Serial.println("what = " + String(what));

    if (String(what) != "clear") {  
        sprintf(nu, "%d-%d:%d:%d ", day(), hour(), minute(), second());
    } else { 
        nu[0] = '\0';
        what[0] = '\0';
    }
    
    strcpy(Log_date[logNr], nu);
    strcpy(Log_kind[logNr], what);
    strcpy(Log_message[logNr], message);
    logNr++;
    Serial.println("log current event = " + String(logNr));
    
    if (logNr >= Log_MaxEvents) {
        logNr = 0;
        Log_MaxReached = true;
    }
    Serial.println("log updated");
}

void Clear_Log(AsyncWebServerRequest *request) {
    if (!checkRemote(request->client()->remoteIP().toString())) {
        if (logNr != 0) {
            for (int i = 0; i <= Log_MaxEvents; i++) {
                Log_date[i][0] = '\0';
                Log_kind[i][0] = '\0';
                Log_message[i][0] = '\0'; 
            }
            logNr = 0;
            Log_MaxReached = false;     
        }
    } 
}
