#ifndef AAA_LOG_H
#define AAA_LOG_H

#include <Arduino.h>
#include <ESPAsyncWebServer.h>

//<script type="text/javascript" src="SECURITY"></script>

//<table><tr><td style='width:240px;'>Last refresh : !@@!<td>

//<button onClick='window.location.reload();' style='width: 100px' value='0'>Refresh</button></table><br>

const char LOGPAGE[] PROGMEM = R"=====(
<!DOCTYPE html><html><head><meta charset='utf-8'>
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
  font-size:16px;
  border: 1px solid;
  text-align: left;
}
#lijst td  {
padding-left: 10px;
border: 1px solid;
}

#lijst tr:nth-child(even){background-color: #f2f2f2;
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
.th1 { width:30%; }
.th2 { width:20%; }
.th3 { width:50%; }


tr {height:20px;}


@media only screen and (max-width: 600px){
#lijst{ font-size:12px; width: 320px;}
 tr {width:94vw;}
.th1 { width:25%; }
.th2 { width:15%; }

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
<br><kop>ESP32-ECU LOG</kop>
<div class='divstijl'><center>
<table><button onClick='window.location.reload();' style='width: 100px' value='0'>Refresh</button></table>
<table id='lijst'><tr><th class='th1'>Time</th><th class='th2'>Type</th><th class='th3'>Command</th></tr> 
  
)=====";
//</tr><tr><td>date </td><td>system</td><td>192.168.0.aaa.sss.ddd </td></tr>

#endif

