#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <PORTAL_HTML.H>

class CaptiveRequestHandler : public AsyncWebHandler {
public:
  CaptiveRequestHandler() {}
  virtual ~CaptiveRequestHandler() {}

  bool canHandle(AsyncWebServerRequest *request) {
    //request->addInterestingHeader("ANY");
    return true;
  }

  void handleRequest(AsyncWebServerRequest *request) {
    handlePortalRoot();
    request->send(200, "text/html", toSend); //send the html code to the client
  }
};

void start_portal() {
  // setup of configportal and next an infinitive loop

  WiFi.mode(WIFI_OFF); // otherwise the scanning fails
  delay(5000);

  Serial.println("scan start");
  scanWifi();
  Serial.println("result scan networksFound = " + String(networksFound));

  /* Soft AP network parameters */
  IPAddress apIP(192, 168, 4, 1);
  IPAddress netMsk(255, 255, 255, 0);
  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(DNS_PORT, "*", apIP);

  //String chipId = getChipId();

  //String temp = String(chipId);
  Serial.println("\ncould not connect with the last known wificredentials,  starting access point...");
  WiFi.softAPConfig(apIP, apIP, netMsk);
  const char* apNaam = getChipId(false).c_str();
  const char* apPasswd = "123456789";
  WiFi.softAP(apNaam, apPasswd);

  delay(500); // without delay tje IP address can be blanco
  Serial.println("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  /* Setup web pages: root, wifi config pages, SO captive portal detectors and not found. */

  server.on("/redirect", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("/redirect requested");
    handlePortalRoot();
    request->send(200, "text/html", toSend); //send the html code to the client
  });

  server.on("/fwlink", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("/redirect requested");
    handlePortalRoot();
    request->send(200, "text/html", toSend); //send the html code to the client
  });

  server.on("/Bback", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("/Bback requested");
    handlePortalRoot();
    //sendHeaders();
    request->send(200, "text/html", toSend); //send the html code to the client
  });

  server.on("/Bback", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("/Bback requested");
    handlePortalRoot();
    //sendHeaders();
    request->send(200, "text/html", toSend); //send the html code to the client
  });

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("/ requested");
    handlePortalRoot();
    //sendHeaders();
    request->send(200, "text/html", toSend); //send the html code to the client
  });

  server.on("/wifiForm", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("/wifiForm requested");
    handleForm();
    //sendHeaders();
    request->send(200, "text/html", toSend); //send the html code to the client
  });

  server.on("/PORTAL_STYLE", HTTP_GET, [](AsyncWebServerRequest *request) {
    //Serial.println("stylesheet requested");
    request->send_P(200, "text/css", PORTAL_STYLESHEET);
  });

  // **********************************************************
  //                   CONNECTING TO WIFI
  // **********************************************************
  server.on("/wifiCon", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("/wifiCon requested");
    laatsteMeting = millis(); //om de timeout te verversen
    char ssid[33] = "";
    char pass[40] = "";

    // process the data and try to connect
    strcpy(ssid, request->getParam("s")->value().c_str());
    strcpy(pass, request->getParam("p")->value().c_str());
    strcpy(pswd, request->getParam("pw")->value().c_str());
    securityLevel = request->arg("sl").toInt();

    wifiConfigsave(); // save the admin passwd

    // trying to connect now
    // therefor we first go in wifi APSTA
    WiFi.mode(WIFI_AP_STA);
    Serial.println("Connecting to " + String(ssid));
    WiFi.begin(ssid, pass);
    Serial.println("send confirm page  ");

    if (connectWifi() == WL_CONNECTED) {
      Serial.println("youpy, connected");
      esp_task_wdt_reset();
      digitalWrite(led_onb, LED_UIT);
      event = 101;
    } else {
      Serial.println("could not connect, try again");
      esp_task_wdt_reset();
      ledblink(10, 200);
      digitalWrite(led_onb, LED_AAN);
      event = 100;
    }

    toSend = FPSTR(PORTAL_CONFIRM);

    if (event == 100) {
      toSend.replace("{text}", "connection has failed");
    } else {
      toSend.replace("{text}", "connection success");
    }

    request->send(200, "text/html", toSend); //send the html code to the client
  });

  //server.onNotFound(handlePortalNotFound);
  server.onNotFound([](AsyncWebServerRequest *request) {
    String message = "file not found";
    AsyncWebServerResponse *response = request->beginResponse(404, "text/plain", message);
    response->addHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    response->addHeader("Pragma", "no-cache");
    response->addHeader("Expires", "-1");
    request->send(response);
    //request->send(200, "text/html", toSend); //send the html code to the client
  });

  //server.on("/LittleFS_ERASE", eraseFiles);
  //server.on("/STATIC_ERASE", resetStatic);
  server.on("/STATIC_ERASE", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("static erase requested");
    String toSend = F("<!DOCTYPE html><html><head>");
    toSend += F("<script type='text/javascript'>setTimeout(function(){ window.location.href='/redirect'; }, 500 ); </script>");

    toSend += F("</head><body><h2>erase ip settings, please wait... !</h2></body></html>");

    static_ip[0] = '\0';
    static_ip[1] = '\0';
    wifiConfigsave();
    request->send(200, "text/html", toSend); //send the html code to the client
  });

  server.on("/close", HTTP_GET, [](AsyncWebServerRequest *request) {
    handlePortalClose();
    request->send(200, "text/html", toSend); //send the html code to the client
  });

  server.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER); //only when requested from AP

  server.begin(); // Web server start
  Serial.println("started async HTTP server for the portal");
  digitalWrite(led_onb, LED_AAN);  // led aan

  // now we enter an infinitive loop that we leave only after 5 minutes,
  // or when via server.handleClient something happens
  // bool Timed_Out = true;
  Serial.println("entering the infinitive loop with heartbeat");
  laatsteMeting = millis(); //voor de time-out

  // this is the infinitive loop
  static unsigned long heartbeat = 0;
  while (millis() < laatsteMeting + 300 * 1000UL) { // 5 minuten== 300 30 == 30sec
    if (millis() > heartbeat + 10 * 1000UL) {
      heartbeat = millis(); // elke 10 sec een heartbeat
      Serial.print("a ");
    }
    // SERIAL: *************** see if there is data available **********************
    // if(Serial.available()) {
    // handle_Serial();
    // }

    if (tryConnectFlag) { // there are credentials provided
      wifiConnect(); // if connected we break out of this loop
    }
    //DNS
    //dnsServer.processNextRequest();
  }
  // ************************ end while loop *******************************

  //we only are here after a timeout. If we click finish we restart
  // if (Timed_Out == true) {
  Serial.println("portal timed out, resetting...");
  ESP.restart();
  // }
}

// ********************************************************************
//                 de homepagina van het portal
// ********************************************************************
void handlePortalRoot() {
  // always as we are here, portalstart is updated, so when there is activity in the
  // webinterface we won't time out.
  //sendHeaders();
  laatsteMeting = millis(); // update portalstart
  Serial.println("handlePortalRoot, event = " + String(event));
  //toSend = FPSTR(PORTAL_HEAD);
  toSend = FPSTR(PORTAL_PAGE);
  //toSend.replace("{haha}" , "if (window.location.hostname != \"192.168.4.1\") {window.location.href = 'http://192.168.4.1'};");
  toSend.replace("{maandstroom}", "{maandstroom} W");
  toSend.replace("{uurstroom}", "{uurstroom} W");
  toSend.replace("{status}", "solarimeter");

  // int netwerkenFound = scanWifi();

  toSend.replace("{netwerkenFound}", String(networksFound));
  String options = FPSTR(HTTP_FORM_START);

  for (int n = 0; n < networksFound; n++) {
    options += FPSTR(HTTP_FORM_ITEM);
    options.replace("{v}", WiFi.SSID(n));
    options.replace("{r}", String(WiFi.RSSI(n)));
    options.replace("{e}", WiFi.encryptionType(n) == WIFI_AUTH_OPEN ? "" : "*");
  }

  options += FPSTR(HTTP_FORM_END);
  toSend.replace("{netwerken}", options);
}
