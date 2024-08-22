#include <MAIN.h>



//#include "esp_heap_caps.h"

AsyncWebServer server(80);
AsyncEventSource events("/events"); 
AsyncWebSocket ws("/ws");

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
const byte DNS_PORT = 53;
DNSServer dnsServer;

#ifdef TEST
int testCounter = 0;
#endif


//float calliBration = 1.66;
//bool stoPPed = false;
bool USB_serial = true;
char ssid[33] = ""; // was 33 
char pass[64] = ""; // was 40
bool tryConnectFlag = false;
WiFiUDP Udp;

// variables concerning time and timers
//time_t daybeginTime = 0;
time_t switchoffTime = 0;
time_t switchonTime = 0;
bool dayTime=true;

//  byte mDay = 0;
//  String  maan = "";
uint8_t actionFlag = 0;

// variables wificonfig
char pswd[11] = "0000";
char userPwd[11] = "1111";  
float longi = 5.123;
float lati = 51.123;
char gmtOffset[5] = "+120";  //+5.30 GMT
bool zomerTijd = true;
char static_ip[16] = "000.000.000.000";
uint8_t securityLevel = 6;


// ***************************************************************************
//                             variables ECU
// ***************************************************************************

char inMessage[CC2530_MAX_SERIAL_BUFFER_SIZE] = {0};
int readCounter = 0;
//char messageHead[5];
int diagNose = 1; // initial true but after a successful healthcheck false
bool Polling = true; // when true we have automatic polling
int errorCode = 10;
//int recovered = 0;
char txBuffer[50];

int t_saved[YC600_MAX_NUMBER_OF_INVERTERS] = {0};
float en_saved[YC600_MAX_NUMBER_OF_INVERTERS][4] = {0};

char InputBuffer_Serial[50]; // need to be global

typedef struct {
  char invLocation[13] = "N/A";
  char invSerial[13]   = "000000000000";
  char invID[5]        = "0000";
  int  invType         = 0;
  int  invIdx          = 0;
  bool conPanels[4]    = {true,true,true,true};
} inverters; 
inverters Inv_Prop[9]; 

typedef struct {
  float freq = 0.0;
  float sigQ = 0.0;
  float heath = 0.0;
  float acv = 0.0;
  float dcc[4] = {0.0, 0.0, 0.0, 0.0};              // ampere <100 
  float dcv[4] = {0.0, 0.0, 0.0, 0.0};              // volt <100
  float power[4] = {0.0, 0.0, 0.0, 0.0};       //watt < 1000
  float pw_total = 0.0;
  float en_total = 0;
} inverterdata;
inverterdata Inv_Data[9];
  
 
bool polled[9]={false,false,false,false,false,false,false,false,false};
uint8_t zigbeeUp = 11; // initial allways initializing, this changes to 1 = up or 0 not up after initial healthcheck
int pollOffset = 0;
int inverterKeuze = 0;
int inverterCount = 0;
char ECU_ID[13] = "D8A3011B9780";

char requestUrl[12] = {""}; // to remember from which webpage we came  

// variables mqtt ********************************************
char  Mqtt_Broker[30] = {"192.168.0.100"};
//  char  Mqtt_inTopic[16] =  {"ESP-ECU/in"};
char  Mqtt_outTopic[26] = {"domoticz/in"}; // was 26
char  Mqtt_Username[26] = {""};
char  Mqtt_Password[26] = {""};
char  Mqtt_Clientid[26] = {""};
char  Mqtt_Port[5] =  {"1883"};
int   Mqtt_Format = 0; 
int   event = 0;
long  mqtt_lastConnect = 0;

int dst;
int iKeuze;
//  int inverterTopoll = 0;
bool timeRetrieved = false;
int networksFound = 0; // used in the portal
int datum = 0; //

unsigned long previousMillis = 0;        // will store last temp was read
static unsigned long laatsteMeting = 0; //wordt ook bij OTA gebruikt en bij wifiportal
static unsigned long lastCheck = 0; //wordt ook bij OTA gebruikt en bij wifiportal


// we use this string only to send webpages
String toSend = "";
 
int value = 0; 

// int aantal = 0;
int resetCounter=0;
bool apFlag=false;
// *******************************  log *************************************
// variables To record and display last events on the home page.

// struct logEvent {
//  String    Log_date ;
//  String    Log_kind ;
//  String    Log_message;
// };
// *******************************  log *************************************
 
typedef struct {
  char date[14] ;
  int  kind ; // zigbee, system, mqtt, pairing
  char  message[13] ;
} logEvent;

logEvent Log_Events[Log_MaxEvents];
bool Log_MaxReached = false;
byte logNr = 0;

WiFiClient espClient ;
PubSubClient MQTT_Client ( espClient ) ;
int Mqtt_stateIDX = 123;
bool getallTrigger = false;
bool reloadTrigger = false;

// *****************************************************************************
// *                              SETUP
// *****************************************************************************
void setup(){
  Serial.begin(115200);

  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
  Serial.println(F("Serial Txd is on pin: 17"));
  Serial.println(F("Serial Rxd is on pin: 16"));

  
  pinMode(knop, INPUT_PULLUP); // de knop
  pinMode(led_onb, OUTPUT); // onboard led
  // pinMode(ZB_TX, OUTPUT);
  // digitalWrite(ZB_TX, LOW); //
  pinMode(ZB_RESET, OUTPUT);// resetpin cc2530   
  digitalWrite(ZB_RESET, HIGH);
  pinMode(4, OUTPUT); // 
  digitalWrite(4, LOW); //
  
  ledblink(1, 800);

  attachInterrupt(digitalPinToInterrupt(knop), isr, FALLING);

  SPIFFS_read();
  // now we know the number of inverters we can find an interval between pollings
  int pollintervall = 300/inverterCount;
  // takes care for the return to the last webpage after reboot
  read_eeprom(); // put the value of requestUrl back
  
  if (requestUrl[0] != '/'){
      strcpy(requestUrl, "/");
  }  // vangnet

  consoleOut("requestUrl = " + String(requestUrl));
  //DebugPrint("apFlag = " + String(apFlag) );
  start_wifi(); // start wifi and server

  // reed inverter files
  for (int x=0; x < inverterCount+1; x++){
    String bestand = "/Inv_Prop" + String(x) + ".str";

    if(diagNose != 0){
      consoleOut("reading file " + bestand);
    }
    
    if (!leesStruct(bestand)){ 
      // if the file not exists we break
      if(diagNose != 0){
        consoleOut("file " + bestand + "not exists"); //DebugPrintln(bestand);
      }
      break;
    }
  }
  #ifdef DEBUG
  printInverters(); // show the inverter files
  #endif
  getTijd(); // retrieve time from the timeserver
  Update_Log(1, "boot up");

  // ****************** mqtt init *********************
  MQTT_Client.setKeepAlive(150);
  MQTT_Client.setServer(Mqtt_Broker, atoi(Mqtt_Port));
  MQTT_Client.setCallback ( MQTT_Receive_Callback ) ;

  if ( Mqtt_Format != 0 ){
    Serial.println(F("setup: mqtt configure"));
    mqttConnect(); // mqtt connect
  } else {
    Update_Log(3, "not enabled"); 
  }
  initWebSocket();
    
  Serial.println(F("booted up, checking coordinator"));
  Serial.println(WiFi.localIP());
  delay(1000);
  
  ledblink(3,500);

  Update_Log(1,"healthcheck");
  healthCheck(); // check the state of the zigbee system and if oke then poll the inverters
  
  if(zigbeeUp == 1){
    Update_Log(2,"running");
    
    // we poll our inverters immediatly
    if(Polling){
      poll_all();
    }
  }

  resetCounter = 0;
  //events.send( "reload", "message"); //getGeneral and getAll triggered
  eventSend(0);

} // end setup()

//****************************************************************************
//*                         LOOP
//*****************************************************************************
void loop(){

  // ***************************************************************************
  //                       day or night mode
  // ***************************************************************************
  #ifdef TEST
  // always daytime to be able to test
  dayTime = true;
  #endif
 
   if(now() > switchonTime && now() < switchoffTime) 
    {
          if(!dayTime)  
          {
             dayTime = true;
             Update_Log(1, "woke up");
             if( diagNose != 0 ) consoleOut("woke-up");
             // reset the dayly energy at wakeup and send mqtt message
             resetValues(true, true);
             //events.send( "reload", "message"); // refresh the data and state
             eventSend(1);
            }
    } else {
         if(dayTime) 
         {
            dayTime = false;
            //String term= "nightmode";
            Update_Log(1, "nightmode");
            if( diagNose != 0 ) consoleOut("nightmode");
            // clean memory
            //memset( &inMessage, 0, sizeof(inMessage) ); //zero out the 
            //delayMicroseconds(250);
            // we send null messages for each inverter
            resetValues(false, true); // make all values zero exept energy and send mqtt
            //events.send( "reload", "message"); // refresh the data and state
            eventSend(0);
            actionFlag = 250; // triggers the reset values and mqtt null message at midnight
            
         }
    }

// ******************************************************************
//              polling every 300 seconds
// ******************************************************************

  unsigned long nu = millis();  // the time the program is running

   if (nu - laatsteMeting >= 1000UL * 300) // 300 sec
   {
     if(diagNose) Serial.println("300 secs passed, polling" + String(millis()) ); //
        laatsteMeting += 1000UL * 300 ; // increases each time with (300/inverterCount * miliseconds);
        if(dayTime && Polling) // we only poll at day and when Polling = true 
           { 
              ledblink(1,100);
              poll_all(); //if inverterCount = 9 than we have inverters 0-8
           } 
 }

// ******************************************************************
//              healthcheck every 10 minutes
// ******************************************************************

   nu = millis() + 1000UL*120; // 2 minutes later // 
   if (nu - lastCheck >= 1000UL * 600){  // =10min
    Serial.println("600 secs passed, healthcheck" + String(millis()));
    lastCheck += 1000UL * 600;
    //we dont do healtcheck when stopped
    healthCheck(); // checks zb radio, mqtt and time, when false only message if error
   }

  // we recalcultate the switchtimes for this day when there is a new date
  // if retrieve fails, day will not be datum, so we keep trying by healthcheck
  if (day() != datum && hour() > 2){ // if date overflew and later then 2 
    getTijd(); // retrieve time and recalculate the switch times
    //delay(500);
    //ESP.restart();
    //DebugPrintln("date overflew, retrieve time");
  }
 
// ***************************************************************************
//                       m q t t
// ***************************************************************************
  // before each transmission the connection is tested
  // so we don't do this in the loop
  if(Mqtt_Format != 0){
    MQTT_Client.loop(); //looks for incoming messages
  }
    
  //*********************************************************************
  //             send null data at midnight 
  // ********************************************************************
  if(hour() == 0 && timeRetrieved && actionFlag == 250){
    if(second() > 0 ){
      resetValues(true, true); //set all values to zero and sent mqtt
      Update_Log(1, "values wipe");
      actionFlag = 0; // to prevent repetition
      //events.send( "getall", "message"); // refresh the data
      eventSend(2);
    }
  }
  
  test_actionFlag();
  
  if( Serial2.available() ){
    empty_serial2(); // clear unexpected incoming data
  }

  ws.cleanupClients();
  yield(); // to avoid wdt resets

  // SERIAL: *************** kijk of er data klaar staat op de seriele poort **********************
  if(Serial.available()){
    handle_Serial();
  }

  //  if(getallTrigger){
  //    getallTrigger = false;
  //    events.send( "getall", "message", millis());
  //  }
  //  if(reloadTrigger){
  //    reloadTrigger = false;
  //    events.send( "reload", "message", millis());
  //   }
}
