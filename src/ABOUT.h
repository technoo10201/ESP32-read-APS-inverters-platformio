#ifndef ABOUT_H
#define ABOUT_H

#include <Arduino.h>
#include <ESPAsyncWebServer.h>

const char ABOUT[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
  <meta http-equiv="refresh" content="180">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <meta charset='utf-8'>
  <link rel="stylesheet" type="text/css" href="/STYLESHEET_HOME">
  <title>ESP32-ECU</title>
  <style>
    table, th, td {
      border: 1px solid blue; 
      font-size: 12px; 
      background-color: #ffffcc;
    }
    th, td { 
      padding-left: 4px; 
      padding-right: 4px;
    }
    th {
      background-color: #22ffb3;
    }
    body { 
      font-size: 12px; 
    } 
    tr {
      height: 26px;
    }
  </style>
  <script type="text/javascript" src="SECURITY"></script>
  <script> 
    function cl() { 
      window.location.href='/MENU'; 
    }
  </script>
</head>
<body>
  <div id='msect'>
    <ul>
      <li id='fright'>
        <span class='close' onclick='cl();'>&times;</span>
      </li>
    </ul>
  </div>
  <center>
    <h2>ESP-ECU SYSTEM DATA</h2>
  </center>
)=====";

void handleAbout(AsyncWebServerRequest *request);


#endif