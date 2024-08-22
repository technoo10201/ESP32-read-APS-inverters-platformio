#ifndef DETAILSPAGE_H
#define DETAILSPAGE_H

#include <Arduino.h>

const char DETAILSPAGE [] PROGMEM = R"=====(
<!DOCTYPE html><html><head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="stylesheet" type="text/css" href="/STYLESHEET">
<meta charset='utf-8'>
<link rel="icon" type="image/x-icon" href="/favicon.ico" />
<title>ESP32-ECU</title>
<style>
table, th, td {border: 1px solid blue; font-size:16px; text-align:center} 
body {margin:10px 10px 0px 20px; font-family:'lato',Verdana,Sans-serif;font-size:16px;}
tr {height:35px;}
</style>

</head><body><center>
)=====";

void sendPageDetails(int i);

#endif