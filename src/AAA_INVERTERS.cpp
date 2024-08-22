#include <AAA_INVERTERS.h>


//*******************************************************************************************
//             prepare for saving the data
// *****************************************************************************************
void handleInverterconfig(AsyncWebServerRequest *request) 
{ 
  //  form action = handleInverterconfig
  //  we only collect the data for this specific inverter
  //  read the serverargs and copy the values into the variables

  //  if( diagNose != 0 ) consoleOut("we are in handleInverterconfig");    
  //  if( diagNose != 0 ) consoleOut("inverterCount initial = " + String(inverterCount));
  //  if( diagNose != 0 ) consoleOut("iKeuze = " + String(iKeuze));  
  
  // collect the serverarguments
  strcpy(Inv_Prop[iKeuze].invLocation, request->arg("il").c_str());
  strcpy(Inv_Prop[iKeuze].invSerial, request->arg("iv").c_str());
  Inv_Prop[iKeuze].invType = request->arg("invt").toInt(); //values are 0 1 2  
  Inv_Prop[iKeuze].invIdx = request->arg("mqidx").toInt(); //values are 0 1  
  
  // the selectboxes
  char tempChar[1] = "";
  String dag = request->arg("pan1");  // mqselect
  if ( dag == "on"){ Inv_Prop[iKeuze].conPanels[0] = true;} else { Inv_Prop[iKeuze].conPanels[0] = false; }   
  dag = request->arg("pan2");  // mqselect
  if ( dag == "on"){ Inv_Prop[iKeuze].conPanels[1] = true;} else { Inv_Prop[iKeuze].conPanels[1] = false; }

  Inv_Prop[iKeuze].conPanels[2] = false;
  Inv_Prop[iKeuze].conPanels[3] = false;

  //we only collect this when type = 1
  if(Inv_Prop[iKeuze].invType == 1){

    dag = request->arg("pan3");  // mqselect
    if ( dag == "on"){
      Inv_Prop[iKeuze].conPanels[2] = true;
    }

    dag = request->arg("pan4");  // mqselect
    if ( dag == "on"){
      Inv_Prop[iKeuze].conPanels[3] = true;
    }
  }

  // DebugPrintln("checked panels are : " + String(Inv_Prop[iKeuze].conPanels[0])+ String(Inv_Prop[iKeuze].conPanels[2])+ String(Inv_Prop[iKeuze].conPanels[2])+ String(Inv_Prop[iKeuze].conPanels[3]));
  
  // is this a addition?
  String bestand = "/Inv_Prop" + String(iKeuze) + ".str"; // Inv_Prop0.str
  if(diagNose != 0){
    consoleOut("going to write " + bestand );
  } 

  // initial their both 0
  writeStruct(bestand, iKeuze); // alles opslaan in SPIFFS
  if(iKeuze == inverterCount){
    inverterCount += 1;
    if(diagNose != 0){
      onsoleOut("we appended, inverterCount now : " + String(inverterCount)); 
    }
  }
   
  basisConfigsave();  // save inverterCount

  #ifdef DEBUG
  if( diagNose != 0 ) consoleOut("\ninverterCount after edit (saved) = " + String(inverterCount));  
  if( diagNose != 0 ) consoleOut("list of the files we have after edit");
  printInverters();
  #endif
       
  // #ifdef DEBUG    
  // Serial.println("files after the ops");
  // printInverters();            
  // #endif

  actionFlag = 10; // triggers the reboot
}

//*******************************************************************************************
//             delete an inverter
// *****************************************************************************************
void handleInverterdel(AsyncWebServerRequest *request) 
{ 
  // form action = handleInverterconfig
  // we only collect the data for this specific inverter
  // read the serverargs and copy the values into the variables

  // if( diagNose != 0 ) consoleOut("we are in handleInverterdel");    
  // if( diagNose != 0 ) consoleOut("inverterCount initial = " + String(inverterCount));
  // if( diagNose != 0 ) consoleOut("iKeuze = " + String(iKeuze));  

   String bestand = "/Inv_Prop" + String(iKeuze) + ".str"; // /Inv_Prop0.str
   if( diagNose != 0 ) consoleOut("remove file " + bestand ); 
 
   if(SPIFFS.exists(bestand) ) SPIFFS.remove(bestand);
   
   if( diagNose != 0 ) consoleOut("list of the files we have after removed one");
   printInverters();
   inverterCount -= 1;
   basisConfigsave();  // save inverterCount   
//   // now we may have a gap in the file order
//   // check if we have one and remove it
      remove_gaps();
      //Serial.println(F("list of the files after remove gaps"));
    
    printInverters(); 
    
    if( diagNose != 0 ) consoleOut("inverterCount after removal = " + String(inverterCount));

      actionFlag = 10;
}

void printInverters(){ 
      if(diagNose == 0 ) return;     
      if( diagNose != 0 ) consoleOut(F(" ****** excisting inverter files ******"));
      for (int x=0; x < inverterCount+1; x++) 
      {
      String bestand = "/Inv_Prop" + String(x) + ".str";
      
      if(SPIFFS.exists(bestand)) 
          {
              if( diagNose != 0 ) consoleOut("filename: " + bestand);
              printStruct(bestand);
          }
         
      }
}

// say we have
// Inv_prop0.str
// Inv_prop1.str

// Inv_prop3.str
// Inv_Prop4.str

// after we found 3 and not 2 we have
// Inv_prop0.str
// Inv_prop1.str
// inv_prop2.str
// Inv_prop3.str we renamed this one so the gap moved
// Inv_Prop4.str

// remove the gaps
void remove_gaps(){
String bestand_1;
String bestand_2;
bool found = false;  

  for(int i=0; i < 10; i++ ) 
  {
      bestand_1 = "/Inv_Prop" + String(i) + ".str";
      bestand_2 = "/Inv_Prop" + String(i+1) + ".str";
      //Serial.println("bestand_1 = " + bestand_1);
      //Serial.println("bestand_2 = " + bestand_2);
      if(!SPIFFS.exists(bestand_1) && SPIFFS.exists(bestand_2)) 
      {
      //Serial.println(bestand_1 + " not exist and " + bestand_2 + " exists"); 
        found = true;
        SPIFFS.rename(bestand_2, bestand_1); // file 2 becomes file 1
      //Serial.println("renamed " + bestand_1);
        printInverters();    
      }
  }
  // we remove the last file
  if (found) 
    {
    bestand_1 = "/Inv_Prop" + String(inverterCount) + ".str"; 
    if(!SPIFFS.exists(bestand_1) ) SPIFFS.remove(bestand_1);
    }
}
// ********************************************************************
//                     processor
// *********************************************************************
String processor(const String& var)
{
//
  if(var == "LOADBAG") 
  {
    Serial.println(F("found LOADBAG"));
    if(Inv_Prop[iKeuze].invType == 1) 
      {
      return F("showFunction()"); 
      } else {
      return F("hideFunction()");  
      }
  }
// make the menu items visable

  for(int x=0; x<9; x++){ // for every button we have to set the visibility
     String placeholder = "none'" + String(x);
     //Serial.println("placeholder = " + placeholder);
       if(var == "none'" + String(x) ){ 
        if (x < inverterCount){ return F("block'"); } else { return F("none'"); }
       }
  }
//   
   if(inverterCount < 9){
    Serial.println(F("show add button"));
    if(var == "none'99") return F("block'"); // show the add button
   }
  
  if(var == "<FORMPAGE>") 
  {
  return(toSend);  
  }

if(var == "none'p"){
    String bestand = "/Inv_Prop" + String(iKeuze) + ".str";
    if(SPIFFS.exists(bestand)) 
    {
    // we make the pair and delete button visible
      return "block'";
    } else {
      return "none'";
    }
}

return String(); //return empty when no match
}


// we construct the form
void inverterForm(){
  //Serial.println("bool nix = " + String(nix));
  int verklikker = 0;
    if (inverterCount >= 88 )
    { 
        verklikker = 88;
        inverterCount -= verklikker; // restore the original inverterCount
    }
    inverterCount += verklikker; // add 88 again
    // now we have 3 situations
    // inverterCount == 0, show the page currently no inverters
    // iKeuze < invertercount, we have an exixting inverter
    // iKeuze = invertercount, we add a new inverter
    // we clicked the add button then invertercount is at least 88
    if( inverterCount != 0 ){
   
    // **********************************************************************
    //        construct the inverterpage with actual data
    // **********************************************************************
        if (inverterCount >= 88 ) inverterCount -= 88; // restore inverterCount
        toSend = FPSTR(INVERTER_GENERAL);  
        // is there a file iKeuze then
        String bestand = "/Inv_Prop" + String(iKeuze) + ".str";
        if(SPIFFS.exists(bestand)) 
       {
        if(diagNose != 0){
          consoleOut("File exists" + bestand);
        }
        //the file exists so we can display the values 
        toSend.replace("{nr}" , String(iKeuze)); // vervang inverter nummer not available
        toSend.replace("000000", String(Inv_Prop[iKeuze].invSerial)); // handled by the script
        toSend.replace("{location}", String(Inv_Prop[iKeuze].invLocation));
        toSend.replace("{idx}", String(Inv_Prop[iKeuze].invIdx));
        // the selectboxes
        if (Inv_Prop[iKeuze].conPanels[0]){ toSend.replace("#1check", "checked");}
        if (Inv_Prop[iKeuze].conPanels[1]){ toSend.replace("#2check", "checked");}
                
        if(Inv_Prop[iKeuze].invType != 1 ){ // when the type = yc600 (0) or ds3 (2)
              
            toSend.replace("onload='showFunction()", "onload='hideFunction()" );
            if(Inv_Prop[iKeuze].invType == 0) 
            { 
              toSend.replace("invtype_0", "selected");
            } else {
             toSend.replace("invtype_2", "selected");  
           }
        } else { // inv type == 1 
          
          //Serial.println(" inverter type = 1");
          toSend.replace("invtype_1", "selected");
           if (Inv_Prop[iKeuze].conPanels[2]){ toSend.replace("#3check", "checked");}
           if (Inv_Prop[iKeuze].conPanels[3]){ toSend.replace("#4check", "checked");}
        }
        
        if(String(Inv_Prop[iKeuze].invID) != "0000") 
        {
           toSend.replace("unpaired", String(Inv_Prop[iKeuze].invID) );
        }

        } else {
        // the file does not exist so we show an empty page
        if(diagNose != 0){
          consoleOut("File does not exist");
        }
        toSend.replace("invtype_2", "selected");
        toSend.replace("000000", "");
        toSend.replace("{location}", "");
        toSend.replace("{idx}", "0");
        }

    } else { // so if inverterCount == 0 we present this page
      toSend = "<br><br><br><h3>currently no inverters</h3>"; 
    }
// now we have toSend ready to include in the inverterpage
}
