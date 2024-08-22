//<td>DS3 conversion<td><input class='inp2' type='number' id='cali' name='cali' min='0.2' max='2' value='{ca}'</input></td><tr>

#include <CONFIG_BASIS.h>

void zendPageBasis(){
  //DebugPrintln("we zijn nu op zendPageBasis");
  toSend = FPSTR(HTML_HEAD);
  toSend += FPSTR(BASISCONFIG);
  
  // Replace data
  toSend.replace("'{id}'", "'" + String(ECU_ID) + "'");
  // toSend.replace("'{pr}'", "'" + String(pollRes) + "'");
  toSend.replace("'{pw1}'", "'" + String(userPwd) + "'");
  toSend.replace("'{of}'", "'" + String(pollOffset) + "'"); 
  
  if (Polling){ 
    toSend.replace("#check", "checked");
  }
  // toSend.replace("'{ca}'", "'" + String(calliBration) + "'");
}

void handleBasisconfig(AsyncWebServerRequest *request){ 
  // Form action = handleConfigsave
  // Verzamelen van de serverargumenten   
  strcpy(ECU_ID, request->arg("ecuid").c_str());
  strcpy(userPwd, request->arg("pw1").c_str());
  // pollRes = request->arg("pr").toInt();
  // hc_IDX = request->arg("hcidx").toInt();
  pollOffset = request->arg("offs").toInt();  
  
  // This value gets currupted when it is negative, we get 256 -/- the number
  // So -2 becomes 254
  // if (po > 200){ pollOffset = po - 256; } else { pollOffset = po; } 
  
  // calliBration = request->arg("cali").toFloat();
  
  // BEWARE CHECKBOX
  String dag = "";
  if (request->hasParam("pL")){
    dag = request->getParam("pL")->value();  
  }
  if (dag == "on"){ 
    Polling = true; 
  } else { 
    Polling = false;
  }
  
  //toSend = FPSTR(CONFIRM);
  basisConfigsave();  // Alles opslaan
  // request->send_P(200, "text/html", CONFIRM); //send the html code to the client
 
  // DebugPrintln("basisconfig saved");
  actionFlag = 25; // Recalculates the time with these new values 
}
