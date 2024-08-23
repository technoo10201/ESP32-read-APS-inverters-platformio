#include <AAA_LOG.h>


void handleLogPage( AsyncWebServerRequest *request ){
  Serial.println("zendPageLog ");

  // String uur = String(hour());
  // if(hour() < 10){ 
  //        uur = "0" + String(hour());
  // } 
  //  String minuten = String(minute());
  //  if(minute() < 10){ 
  //       minuten = "0" + String(minute());
  // }

  //String cont = "";
  //cont += uur + " : " + minuten + " hr.";

  char page[3072] = {0};
  char temp[100]={0};
  strcpy_P(page, LOGPAGE);
  //strcat(page, "<table id='lijst'><tr><th class='th1'>Time</th><th class='th2'>Type</th><th class='th3'>Command</th></tr>"); 
  
  uint32_t lengte= strlen(page );
  uint32_t added = 0;
  Serial.println("initial length = " + String(lengte )); // 1319
//toSEND.replace("!@@!", cont);

//DebugPrintln(" zendlogpage :build eventlist");  
  byte Log_Count = 0;
  Log_MaxReached ? Log_Count = Log_MaxEvents : Log_Count = logNr;  // determine if the max number of event is reached
  Serial.println("log_Count = " + String(Log_Count ));
  Serial.println("printlog current event = " + String(logNr));
  int j = logNr;
  
  String content = "";
  for ( int i = 1; i <= Log_Count; i++ ){
 //Serial.println("een regel van de lijst, nummer i = "); //Serial.println(i);    
  j--; //  we get the index of the last record in the array
 //Serial.println("een regel van de lijst, nummer j = "); //Serial.println(j);
  if (j ==-1) j = Log_MaxEvents - 1; // if we are under the first index of the array ,we go to the last
  ////////////////// One table line ///////////////////
  //sprintf(temp,"<tr><td>%s</td><td>%s</td><td>%s</td>", Log_date[logNr], Log_list[j].Log_kind, Log_list[j].Log_message);
  sprintf(temp,"<tr><td>%s</td><td>%s</td><td>%s</td>", Log_date[j], Log_kind[j], Log_message[j] );
  strcat(page, temp);
  added = strlen(page) - lengte;
  Serial.println("length added = " + String(added ));
  lengte= strlen(page );
  ////////////////// One table line ///////////////////
  }
  //Serial.print(content);
  
  // end of page
  strcat(page, "</table></center></div></div></body></html>"); 
  Serial.println("length = " + String(strlen(page )));
  request->send(200, "text/html", page); 
}

void Update_Log(int what, char message[14]) {
  char nu[14];
        //DebugPrintln("updating the log");
        Serial.println("what = " + String(what));
        if(String(what) != "clear"){  
        sprintf(nu,"%d-%d:%d:%d ", day(), hour(), minute(), second());
        } else { 
          nu[0] = '\0';
          what[0] = '\0';}
        //Serial.println("nu = " + String(nu)); 
        strcpy( Log_date[logNr], nu );
        //Serial.println("Log_date[logNr] = " + String(Log_date[logNr]) );
        strcpy( Log_kind[logNr], what );
        //Serial.println("Log_kind[logNr] = " + String(Log_kind[logNr]) );
        strcpy( Log_message[logNr], message );
        //Serial.println("Log_message[logNr] = " + String(Log_message[logNr]) );
        //strcpy( Log_list[logNr].Log_kind, what );
        //strcpy( Log_list[logNr].Log_message, message );
        logNr++;
        
        Serial.println("log current event = " + String(logNr));
        if (logNr >= Log_MaxEvents)
        {
            logNr = 0;//start again
            Log_MaxReached = true;
        }
          Serial.println("log updated");
}

void Clear_Log(AsyncWebServerRequest *request){

    if(!checkRemote( request->client()->remoteIP().toString()) ){
        if(logNr != 0){
        for (int i=0; i <= Log_MaxEvents; i++){
           Log_date[20][0] = '\0';
           Log_kind[20][0] = '\0';
           Log_message[20][0] = '\0'; 
        }
        logNr = 0;//start again
        Log_MaxReached = false;     
        //Serial.println("log cleared");   
        }
    } 
}


//void Clear_Log(){
//    //Serial.println("clearing the log");
//    if(logNr != 0){
//      //char nu[0]='\0';
//      //char what[0]='\0';
//      //char message[0]='\0';;
//            for (int i=0; i <= Log_MaxEvents; i++){
//            //Update_log("clear", "");
//            }
//         logNr = 0;//start again
//         Log_MaxReached = false;     
//        //Serial.println("log cleared");   
//    }
//}
