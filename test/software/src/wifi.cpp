//
// using ideas from:
// https://github.com/sstaub/NTP
//
// When calling configtime with NTP server name (like "pool.ntp.org"), make sure you have a valid DNS settings
// A valid DNS will be available if you started the network connection with default parameters (like Wifi.begin(ssid,sspass)), 
// but will not be available in case you started your network connection with WiFi.config(IP,gateway,subnet) (used to get fix IP)
// without giving it valid DNS -> WiFi.config(IP,GW,SUB,DNS1,DNS2)!!!
//

#include <WiFi.h>
#include "credentials.h"

const char ntpServer[] = "pool.ntp.org";
const char hostname[]  = "ESP32-Nimo";
const char tzinfo[]    = "WEST-1DWEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00";   // Western European Time

String sNtpMessage     = "";

void  NTP_printLocalTime(int mode);
char *asc_time(const struct tm *timeptr, char mode);

extern char SERDEBUG;
extern void whirl(char delaycnt, char mode); 

void WiFi_init() {
  char retry = 0;
  WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE); // workaround hostname setup (https://github.com/espressif/arduino-esp32/issues/2537)
  WiFi.begin(ssid, password);
  WiFi.setHostname(hostname);
  delay(500);
  
  if(SERDEBUG) {
    Serial.print("Connecting to '");
    Serial.print(ssid);
    Serial.println("' ...");
  }

  while (WiFi.status() != WL_CONNECTED) {
    if(retry++ > 8) {
      if(SERDEBUG) 
        Serial.println("Retry connecting..");
      //delay(800);
      WiFi.begin(ssid, password);
      WiFi.setHostname(hostname);
      retry=0;
    }
    whirl(50, 1);
    //delay(500);
  }
  if(SERDEBUG)
    if(WiFi.status() == WL_CONNECTED) {
      Serial.print("Successful connected to ");
      Serial.println(ssid);
    }

  // init NTP and get the time
  configTzTime(tzinfo, ntpServer);

  //disconnect WiFi
  //WiFi.disconnect(true);
  //WiFi.mode(WIFI_OFF);
}


void NTP_printLocalTime(int mode)
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)) {
    if(SERDEBUG)
      Serial.println("Failed to obtain ntptime!");
    return;
  }

  mode ? sNtpMessage = asc_time(&timeinfo, 1) : sNtpMessage = asc_time(&timeinfo, 0);
  if(SERDEBUG)
    Serial.println(sNtpMessage);
}


//
// date & time routine
//
char *asc_time(const struct tm *timeptr, char mode)
{
  static char time_buff[20] = {};

  // time
  if(mode == 0) {
    sprintf(time_buff, "%.2d:%.2d:%.2d",
      timeptr->tm_hour,
      timeptr->tm_min, 
      timeptr->tm_sec);
    return time_buff;
  }

  // shortdate
  if(mode == 1) {
    sprintf(time_buff, "%.2d-%.2d-%.4d",
      timeptr->tm_mday,
      timeptr->tm_mon+1, 
      1900 + timeptr->tm_year);
    return time_buff;
  }

  return time_buff;
}
