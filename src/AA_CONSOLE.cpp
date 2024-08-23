#include <AA_CONSOLE.h>

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len){
  AwsFrameInfo *info = (AwsFrameInfo*)arg;

  for(int i=0; i<len; i++ ){
    txBuffer[i] = data[i];
  }
  txBuffer[len]='\0'; // terminate the array

  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT){
      diagNose = 2; // direct the output to ws
      data[len] = 0;

     
           if (strncasecmp(txBuffer+3,"INV_REBOOT",10) == 0){
              ws.textAll("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n<br>");
              ws.textAll("Reboot an inverter that stopped working.");
              ws.textAll("Characteristics: not responsive, (slow blinking red led).");
              ws.textAll("type REBOOT_INVERTER=x (x=inverternumber 0, 1 etc.)");
              ws.textAll("DISCLAIMER: THIS HAS NOT BEEN TESTED, USE AT YOUR OWN RISK!");
              return;
          } else if (strncasecmp(txBuffer+3,"POLL=",5) == 0){
            //input can be 10;POLL=0; 
            //ws.textAll("received " + String( (char*)data) + "<br>"); 
              int kz = String(txBuffer[8]).toInt();
              if ( kz > inverterCount-1 ){
              ws.textAll("error, no such inverter");
              if ( kz == 9 ) actionFlag = 48; // poll all
              return;  
              }
              ws.textAll("poll inverter " + String(kz));
              iKeuze = kz;
              actionFlag = 47;
              diagNose = true;
              return;
          } else if (strncasecmp(txBuffer+3,"EDIT=",5) == 0){
            //input can be 10;EDIT=0-AABB; 
            //ws.textAll("received " + String( (char*)data) + "<br>"); 
              int kz = String(txBuffer[8]).toInt();
              if ( kz > inverterCount-1 ){
                ws.textAll("error, no such inverter");
                return;  
              }
              char invid[5];
              for(int i=10;  i<15; i++){ invid[i-10] = txBuffer[i]; }
              ws.textAll("edit inverter " + String(kz));
              ws.textAll("id = " + String(invid));
              strncpy(Inv_Prop[kz].invID, invid, 4);
              String bestand = "/Inv_Prop" + String(kz) + ".str"; // /Inv_Prop0.str
              writeStruct(bestand, kz); // save in SPIFFS 
              return;
          } else if (strncasecmp(txBuffer+3,"HEALTH",6) == 0){  
              ws.textAll("check zb system");
              actionFlag = 44; // perform the healthcheck
              diagNose=true;
              return;             
          } else if (strncasecmp(txBuffer+3,"TESTMQTT",8) == 0){   // ************  rtest mosquitto *******************************       
              ws.textAll("test mosquitto");
              actionFlag = 49; // perform the healthcheck
              diagNose=true;
              return;             
          } else if (strncasecmp(txBuffer+3,"CLEAR",5) == 0){  
              ws.textAll("clearWindow");
              return;             
          } else if (strncasecmp(txBuffer+3,"REBOOT_INVERTER=",16) == 0){
              int kz = String(txBuffer[19]).toInt();
              ws.textAll("reboot inverter " + String(kz));
              if (kz > inverterCount-1) {
                ws.textAll("error, non-excisting inverter");
                return;  
              }
              actionFlag = 34;
              return;
          } else if (strncasecmp(txBuffer+3,"FILES",5) == 0){  
              //we do this in the loop
              ws.textAll("listing files..\n");
              actionFlag = 46;
              return;             
          
          } else if (strncasecmp(txBuffer+3,"ZBT=",4) == 0){   // ********************** zigbee test new*****************************       
              ws.textAll("going to send a teststring, len=" + String(len));
              //we do this in the loop
              actionFlag = 45;
              diagNose=true;
               return;             
          } else if (strncasecmp(txBuffer+3,"ERASE",5) == 0){  
              ws.textAll("going to delete all inverter files");
              String bestand;
              for(int i=0; i<50; i++) 
              {
                  String bestand = "/Inv_Prop" + String(i) + ".str";
                  if (SPIFFS.exists(bestand)) 
                  {
                      SPIFFS.remove(bestand);
                      ws.textAll("removed file " + bestand);
                  }

              }
              inverterCount = 0;
              basisConfigsave(); // save inverterCount
              ws.textAll("done");
              return;             
          
          } else if (strncasecmp(txBuffer+3,"DELETE=",7) == 0){  
              //input can be 10;DELETE=filename
              String bestand="";

              for(int i=10;  i<len+1; i++){ 
                bestand += String(txBuffer[i]);
              }

              ws.textAll("bestand = " + bestand); 
              
              if (SPIFFS.exists(bestand)){
                ws.textAll("going to delete file " + bestand); 
                if(!bestand.indexOf("Inv_Prop") == -1 ){
                  SPIFFS.remove(bestand);
                  ws.textAll("file " + bestand + " removed!"); 
                } else {
                  ws.textAll("inverterfile not removed, use 10;erase!"); 
                }
              } else { 
                ws.textAll("no such file");
              }

              return;                      
          } else if (strncasecmp(txBuffer+3, "DIAG",4) == 0){ // normal operation
            if(diagNose){
              diagNose = false;
            } else {
              diagNose= true;
            } 
            ws.textAll("set diagnose to " + String(diagNose) );  
// ****************************************************************
          } else if (strncasecmp(txBuffer+3, "INIT_N",6) == 0){ // normal operation
            ws.textAll("command = " + String(txBuffer) );  
            actionFlag = 21;
            diagNose=true;
            return;
// ***************************************************************
//      } else 
//
//      if (strncasecmp(txBuffer+3, "INIT_P",6) == 0)  // pairing
//      {
//         ws.textAll("command = " + String(txBuffer) );  
//         actionFlag = 22;

      #ifdef TEST
            } else 

            if (strncasecmp(txBuffer+3, "TESTINV",7) == 0)  
            {
              ws.textAll("command = " + String(txBuffer) );  
      //          which = String(txBuffer[10]).toInt();
        //         ws.textAll("chosen = " + String(which) );
      
              actionFlag = 122;
      #endif      
      
 
      } else {
        ws.textAll("unknown command"); 
      }
  
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len){
    //Serial.println("onEvent triggered");
    switch (type){
      case WS_EVT_CONNECT:
        //Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
        break;
      case WS_EVT_DISCONNECT:
        //Serial.printf("WebSocket client #%u disconnected\n", client->id());
        break;
      case WS_EVT_DATA:
        //Serial.println("WebSocket received data");
        handleWebSocketMessage(arg, data, len);
        break;
      case WS_EVT_PONG:
      case WS_EVT_ERROR:
        break;
  }
}

void initWebSocket(){
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}
