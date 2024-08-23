#include <ABOUT.h>

void handleAbout(AsyncWebServerRequest *request){
    char page[1536] = {0};
    char temp[100] = {0};
    strcpy_P(page, ABOUT);

    int minutens = millis() / 60000;
    int urens = minutens / 60;
    int dagen = urens / 24;

    strcat(page, "<br><table><tr><TH colspan='2'> SYSTEM INFORMATION</th></tr>");
    strcat(page, "<tr><td>firmware version<td>ESP32-ECU-v0_7</tr>");
    
    if (timeRetrieved){
        strcat(page, "<tr><td>time retrieved<td>yes</tr>");
    } else {
        strcat(page, "<tr><td>time retrieved<td>n</tr>");
    }

    sprintf(temp, "<tr><td>systemtime<td> %d:%d ", hour(), minute());
    switch (dst){
        case 1: 
            strncat(temp, "summertime</td>", 19); 
            break;
        case 2: 
            strncat(temp, "wintertime</td>", 19); 
            break;
        case 0: 
            strncat(temp, "no dst set</td>", 19); 
            break;
    }
    strcat(page, temp);

    sprintf(temp, "<tr><td>system uptime<td>%d d %d h %d m </td>", dagen, urens - dagen * 24, minutens - urens * 60);
    strcat(page, temp);

    sprintf(temp, "<tr><td>wifi signalstrength<td>%lddB</td>", WiFi.RSSI());
    strcat(page, temp);

    sprintf(temp, "<tr><td>ESP CHIP ID<td>%s</td>", getChipId(true).c_str());
    strcat(page, temp);

    sprintf(temp, "<tr><td>Free heap<td>%ld bytes</td>", esp_get_free_heap_size());
    strcat(page, temp);

    if (Mqtt_Format != 0){ 
        sprintf(temp, "<tr><td>mqtt clientId<td>%s</td>", getChipId(false).c_str());
        strcat(page, temp);

        if (MQTT_Client.connected()){ 
            sprintf(temp, "<tr><td>mqtt connected<td>%s</td>", Mqtt_Broker);
        } else {
            sprintf(temp, "<tr><td>mqtt status<td>not connected</td>");
        }
    } else {
        sprintf(temp, "<tr><td>mqtt status<td>not configured</td>");
    }
    strcat(page, temp);

    sprintf(temp, "<tr><td>securityLevel<td>%d</td>", securityLevel);
    strcat(page, temp);

    sprintf(temp, "<tr><td>ZB resetCounter<td>%d</td></table>", resetCounter);
    strcat(page, temp);

    Serial.println("length = " + String(strlen(page)));

    request->send(200, "text/html", page); //send the html code to the client
    memset(page, 0, sizeof(page));
}