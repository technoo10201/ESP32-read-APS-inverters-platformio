#include <CONFIG_IP.h>

void zendPageIPconfig(){
   //if( diagNose != 0 )("we are now on zendPageIPconfig");
   //loginAdmin(AsyncWebServerRequest *request);
   toSend = FPSTR(HTML_HEAD);
   toSend += FPSTR(IPCONFIG);  

   // see if we have a static, if so we put the select right and read the ip vars
   if ( static_ip[0] == '\0' || static_ip[0] == '0' ) 
   {
        if( diagNose != 0 ) consoleOut("static_ip = " + String(static_ip));
        if( diagNose != 0 ) consoleOut("no static ip");
        toSend.replace("option}" , "selected" );
        //if we hide the page there are no data put back, the will be saved as null 
  } 
  else 
  {
      // we have a static ip so 
        if( diagNose != 0 ) consoleOut("static_ip = " + String(static_ip));
        //if( diagNose != 0 ) consoleOut("there is a static ip");   
        toSend.replace("option2" , "selected" );
  }
  //always put back the ip data
  toSend.replace("{ip}" , String(static_ip) );
  toSend.replace("{gw}" , WiFi.gatewayIP().toString().c_str() );
  toSend.replace("{sn}" , WiFi.subnetMask().toString().c_str() );
 
  // we construct the regex for static ip
  String GateWay = WiFi.gatewayIP().toString();
  if( diagNose != 0 ) consoleOut("gateway in ipconfig = " + GateWay);
  int punt1 = GateWay.indexOf('.');
  int punt2 = GateWay.indexOf('.', punt1+1);
  int punt3 = GateWay.indexOf('.', punt2+1);
  String deel_a=GateWay.substring(0, punt3+1);
  //Serial.print("part_a = "); Serial.println(deel_a);
  //st3l de regex samen als afgeleid van gateway 
  String patroon="(";
  patroon += deel_a;
  patroon += ")([0-9]{1,2}|1[0-9]{2}|2[0-4][0-9]|25[0-5])";
  //Serial.print("patroon = "); Serial.print(patroon);
  toSend.replace("{patroon}" , patroon);
  // de melding bij verkeerd invullen
  deel_a += "x";
  toSend.replace("{title}" , deel_a);  
}

void handleIPconfig(AsyncWebServerRequest *request){

  char static_ip2[16] = "";

  IPAddress ipc = WiFi.gatewayIP();
  String gwcheck = String(ipc[0]) + "." + String(ipc[1]) + "." + String(ipc[2]);
  // almost impossible to enter a wrong IP. no need to check
  // collect the serverarguments
  
  strcpy( static_ip2, request->getParam("ip")->value().c_str() );
  
  String ipcheck = String(static_ip2[0]) + "." + String(static_ip2[1]) + "." + String(static_ip2[2]);   
  //Serial.println(F("\ipcheck = " + ipcheck ));

  String adres="";
  String zin="";
  bool reBoot = false;
  bool leegmaken = false;

  // see if dhcp is selected. if yes, empty static_ip2
  String optie = request->getParam("keuze")->value();
  //String optie = server.arg("keuze");
  if ( optie == "DHCP"){
      if( diagNose != 0 ) consoleOut("dhcp set, dropped static_ip, optie = " + optie);
      //if( diagNose != 0 ) consoleOut(optie);
      static_ip2[0] = '\0';
    }

    //we must chec if ip has changed, this influences the confirm page
    //when dhcp static_ip is zeroed, so this is alway treu.
    //how do we do this?
    //We have the variable static_ip, we compare that with the supplied value
    //If not equal, it has changed 
     String test1=String(static_ip);  
     String test2=String(static_ip2);
     if( diagNose != 0 ) consoleOut("de teststrings zijn: " + test1 + " " + test2);        

    if( diagNose != 0 ) consoleOut("read the confirm page in toSend");
    toSend = FPSTR(CONFIRM_IP);
   
    if (String(static_ip) != String(static_ip2) ) 
    {
        if( diagNose != 0 ) consoleOut("the IP has changed");
        //static_ip=static_ip2;
        strcpy(static_ip, static_ip2);

        // if an ip was entered we put the ip data in the confirmpage
        if (static_ip[0] != '\0' && static_ip[0] != '0') 
        {
             actionFlag = 10; // make it reboot in the loop
             adres = String(static_ip);
             if( diagNose != 0 ) consoleOut("the specified ip = " + adres);
             zin = F("The entered IP is <strong><a href='http://{adres1}'>http://{adres2}</a></strong>");
             zin += F("<br>Use the new IP adres in your browser or click the link.<br>");
             zin += F("<br>This page will close after a few seconds...");
             zin.replace("{adres1}" , adres);
             zin.replace("{adres2}" , adres);
             toSend.replace("#zin#" , zin);
             toSend.replace("{adres0}" , adres); // the address in the settimeout          
        } 
        else 
        {
            // if no ip is specified then we try to connect immediately 
             zin="IP not specified, this is unknown now !!<br><br>NOTE: the configuration modus (AP) is started<br>The led is lighted up. Connect to the AP<br>so you can find out the DHCP ip address.<br><br>You can close this page.<br><br></div>";
             adres ="/";   
             actionFlag = 11; //makes it reboot in AP mode
             toSend.replace("#zin#" , zin);
             toSend.replace("{adres0}" , adres);
       }
  
   //now we show the confirmpage
   }
   //Serial.println("set actionFlag to " + String(actionFlag) );
   request->send(200, "text/html", toSend);
   if( diagNose != 0 ) consoleOut("IPconfig saved");
   wifiConfigsave();
}
