#include <CONFIG_GEO.h>

void zendPageGEOconfig(){
  //DebugPrintln("we are on zendPageGeoconfig");
  //loginAdmin(AsyncWebServerRequest *request);

  toSend = FPSTR(HTML_HEAD);
  toSend += FPSTR(GEOCONFIG);  

  // en de gegevens terugzetten
  //toSend.replace("{le}",  String(lengte) );
  //toSend.replace("{be}",  String(breedte) );
  toSend.replace("{le}",  String(longi,3) );
  toSend.replace("{be}",  String(lati,3) );
  toSend.replace("{tz}",  String(timezone) );

  //Serial.println("zendPageGEOconfig zomerTijd = " + String(zomerTijd) );
  if (zomerTijd){ 
    //  Serial.println("zomerTijd = true");
    toSend.replace("#check", "checked");
  } else {
    //   Serial.println("zomerTijd = false");   
  }
}

void handleGEOconfig(AsyncWebServerRequest *request){
  //char static_ip2[16] = "";

  //de serverargumenten verzamelen
  //strcpy(lengte, request->getParam("le")->value().c_str());

  longi = request->getParam("le")->value().toFloat();

  //strcpy(breedte, request->getParam("be")->value().c_str());

  lati = request->getParam("be")->value().toFloat();

  strcpy(timezone, request->getParam("tz")->value().c_str());

  //BEWARE CHECKBOX
  String dag = "";
  if(request->hasParam("ts")){
    dag = request->getParam("ts")->value();  
  }
  if (dag == "on"){ 
    zomerTijd = true; 
  } else { 
    zomerTijd = false;
  }
   
  //nu gaan we de confirmpage tonen
  //DebugPrintln("we gaan een nieuwe webpage inlezen in toSend");

  //request->send(200, "text/html", toSend); //send the html code to the client
  //DebugPrintln("saved GEOconfig");
     
  wifiConfigsave();

  actionFlag = 25; // recalculate with these settings 
}
