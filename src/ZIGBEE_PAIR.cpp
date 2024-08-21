void pairOnActionflag() {
    // Start with setup the coordinator
    // Can we pair when the radio is up for normal operation

    char term[20];
    sprintf(term, "inverter %d", Inv_Prop[iKeuze].invSerial);
    Update_Log("pairing", term);

    if (!coordinator(false)) {
        // term="pairing failed, zb down";
        Update_Log("pairing", "failed, zb down");
        if (diagNose != 0) {
            consoleOut("pairing failed, zb down");
        }
        return;
    }

    if (diagNose != 0) {
        consoleOut("trying pair inv " + String(iKeuze));
    }
    // Now that we know that the radio is up, we don't need to test this in the pairing routine

    if (pairing(iKeuze)) {
        // DebugPrintln("pairing success, saving configfile");
        String term = "success, inverter got id " + String(Inv_Prop[iKeuze].invID);
        // Update_log("pairing".c_str(), term.c_str());
        if (diagNose != 0) {
            consoleOut(term);
        }
        // } else if(diagNose==2){ws.textAll(term);}
    } else {
        // if (diagNose != 0) consoleOut("pairing failed");
        strncpy(Inv_Prop[iKeuze].invID, "0000", 6);
        String term = "failed, inverter got id " + String(Inv_Prop[iKeuze].invID);
        // Update_log("pair".c_str(), term.c_str());
        if (diagNose != 0) {
            consoleOut(term);
        }
        // } else if(diagNose==2){ws.textAll(term);}
    }

    String bestand = "/Inv_Prop" + String(iKeuze) + ".str"; // /Inv_Prop0.str
    writeStruct(bestand, iKeuze); // Save everything in SPIFFS

    // After successful pairing we issue the command for normal ops
    sendNO();
    checkCoordinator(); // Updates the log
}

void handlePair(AsyncWebServerRequest *request) {
    strncpy(Inv_Prop[iKeuze].invID, "1111", 4); // This value makes the pairing page visible

    actionFlag = 60; // We do this because no delay is allowed within an async request
    toSend = FPSTR(WAIT_PAIR);
    toSend.replace("{#}", String(iKeuze));
    request->send(200, "text/html", toSend); // Send the HTML code to the client
}

bool pairing(int which) {
  // The pairing process consists of 4 commands sent to the coordinator
  char pairCmd[254] = {0};
  char s_d[250] = {0};
  char ecu_id_reverse[13];
  ECU_REVERSE().toCharArray(ecu_id_reverse, 13);
  char ecu_short[5] = {0};
  strncat(ecu_short, ECU_ID + 2, 2); // D8A3011B9780 should be A3D8
  strncat(ecu_short, ECU_ID, 2);
  // ecu_short[5] = '\0'; no need for as strncat terminates with \0
  String term = "";
  bool success = false;

  for (int y = 0; y < 4; y++) {
    switch (y) {
      case 0:
          // Build command 0 this is "24020FFFFFFFFFFFFFFFFF14FFFF14" + "0D0200000F1100" + String(invSerial) + "FFFF10FFFF" + ecu_id_reverse
          snprintf(pairCmd, sizeof(pairCmd),
                    "24020FFFFFFFFFFFFFFFFF14FFFF140D0200000F1100%sFFFF10FFFF%s",
                    Inv_Prop[which].invSerial, ecu_id_reverse);
          break;
      case 1:
          // Build command 1 this is "24020FFFFFFFFFFFFFFFFF14FFFF14" + "0C0201000F0600"  + inv serial,
          snprintf(pairCmd, sizeof(pairCmd),
                    "24020FFFFFFFFFFFFFFFFF14FFFF140C0201000F0600%s",
                    Inv_Prop[which].invSerial);
          break;
      case 2:
          // Build command 2 this is "24020FFFFFFFFFFFFFFFFF14FFFF14" + "0F0102000F1100"  + invSerial + short ecu_id_reverse, + 10FFF + ecu_id_reverse
          snprintf(pairCmd, sizeof(pairCmd),
                    "24020FFFFFFFFFFFFFFFFF14FFFF140F0102000F1100%s%s10FFFF%s",
                    Inv_Prop[which].invSerial, ecu_short, ecu_id_reverse);
          break;
      case 3:
          // Now build command 3 this is "24020FFFFFFFFFFFFFFFFF14FFFF14"  + "010103000F0600" + ecu_id_reverse,
          snprintf(pairCmd, sizeof(pairCmd),
                    "24020FFFFFFFFFFFFFFFFF14FFFF14010103000F0600%s",
                    ecu_id_reverse);
          break;
    }

    delayMicroseconds(250);
    // Send
    term = "pair command " + String(y) + " = " + String(pairCmd);
    if (diagNose != 0) {
      consoleOut(term);
    }
    // else if(diagNose == 2) ws.textAll(term);

    sendZB(pairCmd);
    delay(1500); // Give the inverter the chance to answer

    if (y == 1 || y == 2) {
        // If y 1 or 2 we catch and decode the answer
        if (decodePairMessage(which)) {
            success = true; // If at least one of these 2 was true we had success
        }
    } else {
        // If not y == 1 or y == 2 we waste the received message
        readZB(s_d);
    }
  }

  // Now all 4 commands have been sent
  if (success) {
      return true;
  } else {
      return false;
  } // When paired 0x103A
}

bool decodePairMessage(int which) {
    char messageToDecode[CC2530_MAX_SERIAL_BUFFER_SIZE] = {0};
    char _CC2530_answer_string[] = "44810000";
    char _noAnswerFromInverter[32] = "FE0164010064FE034480CD14011F";
    char *result;
    char temp[13];
    char s_d[250] = {0};
    String term = "";

    strcpy(messageToDecode, readZB(s_d));
    // Serial.println("messageToDecode = " + String(messageToDecode));
    if (diagNose != 0) {
        consoleOut("decoding : " + String(messageToDecode));
    }

    if (readCounter == 0 || strlen(messageToDecode) < 6) { // Invalid message
        if (diagNose != 0) {
            consoleOut("no usable code, returning..");
        }
        messageToDecode[0] = '\0';
        return false;
    }

    // Can we conclude that a valid pair answer cannot be less than 60 bytes
    if (strlen(messageToDecode) > 222 || readCounter < 60 || strlen(messageToDecode) < 6) {
        // term = "no pairing code, returning...";
        if (diagNose != 0) {
            consoleOut(F("no valid pairing code, returning..."));
        }
        messageToDecode[0] = '\0';
        return false;
    }

    // The message is shorter but not too short so continuing
    if (!strstr(messageToDecode, Inv_Prop[which].invSerial)) {
        if (diagNose != 0) {
            consoleOut("not found serialnr, returning");
        }
        // if(diagNose==1) Serial.println(term); else if(diagNose==2) ws.textAll(term);
        messageToDecode[0] = '\0';
        return false;
    }

    if (strstr(messageToDecode, Inv_Prop[which].invSerial)) {
        result = split(messageToDecode, Inv_Prop[which].invSerial);
    }

    if (diagNose != 0) {
        consoleOut("result after 1st splitting = " + String(result));
    }

    // Now we keep splitting as long as result contains the serial nr
    while (strstr(result, Inv_Prop[which].invSerial)) {
        result = split(result, Inv_Prop[which].invSerial);
    }

    consoleOut("result after splitting = " + String(result));
    // Result are the bytes behind the serial nr
    // Now we know that it is what we expect, a string right behind the last occurrence of the serial nr

    memset(&Inv_Prop[which].invID, 0, sizeof(Inv_Prop[which].invID)); // Zero out the
    delayMicroseconds(250);
    strncpy(Inv_Prop[which].invID, result, 4); // Take the 1st 4 bytes

    term = "found invID = " + String(Inv_Prop[which].invID);
    if (diagNose != 0) {
        consoleOut(term);
    }
    
    // Why is this? Can it get this value?
    if (String(Inv_Prop[which].invID) == "0000") {
        return false;
    }
    
    return true;
}
