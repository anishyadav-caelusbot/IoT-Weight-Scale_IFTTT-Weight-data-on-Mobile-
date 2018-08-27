#include "HX711.h"
#include <IFTTTMaker.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

char ssid[] = "*******";       // SSID (name)
char password[] = "********";  // network key

#define KEY "*********"  // Get it from this page https://ifttt.com/services/maker/settings
#define EVENT_NAME "*****" // Name of event name, set when creating the applet

WiFiClientSecure client;
IFTTTMaker ifttt(KEY, client);

HX711 cell(D2, D3); // The two pins connected to the Load Cell and Node MCU 

void setup() {
  Serial.begin(115200);

  // Set WiFi to station mode and disconnect from an AP if it was Previously
  // connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);


  
}
long valCali = 0;
long val = 0;
float count = 0;


void loop() {
  
  IPAddress ip = WiFi.localIP();

  // Calibrating the Load Cell
  
  count = count + 1;
  
  val = cell.read(); // most recent reading
  valCali = (((val-8279253)/22872.0)*50); 
  //Serial.println(valCali);
  
  // Conditions for sending a notification to mobile phone, current definition is acheived if the weight is over
  // 400g, the delay of 10000 sends another notification every 10 seconds

  if( valCali > 400){

    (ifttt.triggerEvent(EVENT_NAME, String(valCali), ip.toString())); // This is sent to IFTTT, 'String(valCali)' sends the weight, 'ip.toString()' sends the ip address of the device
    Serial.println("Weight Over 100");
    delay(10000); // Change this value to delay notifications being sent
  }
   else{ 
  }
  
}
