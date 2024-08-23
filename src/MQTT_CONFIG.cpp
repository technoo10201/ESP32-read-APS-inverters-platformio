#include <MQTT_CONFIG.h>

void zendPageMQTTconfig(){
  //DebugPrintln("we are at zendPageMQTTconfig");
  //toSend = FPSTR(HTML_HEAD);
  //toSend.replace("tieTel", swname );
  toSend = FPSTR(HTML_HEAD);
  toSend += FPSTR(MQTTCONFIG);  

    //altijd de mqtt gegevens terugzetten
  
  //if (Mqtt_Enabled){ toSend.replace("#check", "checked");}
  toSend.replace("{mqttAdres}", String(Mqtt_Broker));
  toSend.replace("{mqttPort}", String(Mqtt_Port));
  toSend.replace("{mqttinTopic}", String(Mqtt_inTopic));
  toSend.replace("{mqttoutTopic}", String(Mqtt_outTopic));
  toSend.replace("{mqtu}", String(Mqtt_Username) );
  toSend.replace("{mqtp}", String(Mqtt_Password) );

  switch (Mqtt_Format){
    case 0:
        toSend.replace("fm_0", "selected");
        break;
    case 1:
        toSend.replace("fm_1", "selected");
        break;
    case 2:
        toSend.replace("fm_2", "selected");
        break;
    case 3:
        toSend.replace("fm_3", "selected");
        break;
    case 4:
        toSend.replace("fm_4", "selected");
        break;
      }
  }

void handleMQTTconfig(AsyncWebServerRequest *request){

  //collect serverarguments
  strcpy( Mqtt_Broker  , request->getParam("mqtAdres")   ->value().c_str() );
  strcpy( Mqtt_Port    , request->getParam("mqtPort")    ->value().c_str() );
  strcpy( Mqtt_inTopic , request->getParam("mqtinTopic") ->value().c_str() );
  strcpy( Mqtt_outTopic, request->getParam("mqtoutTopic")->value().c_str() );
  strcpy( Mqtt_Username, request->getParam("mqtUser")    ->value().c_str() );
  strcpy( Mqtt_Password, request->getParam("mqtPas")     ->value().c_str() );

  Mqtt_Format = request->arg("fm").toInt(); //values are 0 1 2

  if(diagNose != 0){
    consoleOut("saved mqttconfig");
  }

  mqttConfigsave();
  actionFlag = 24; // reconnect with these settings
}
