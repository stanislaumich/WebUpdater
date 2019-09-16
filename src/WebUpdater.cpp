/*
  To upload through terminal you can use: 
  curl -F "image=@firmware.bin" esp8266-webupdate.local/update
*/
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>
#include <StasWiFi.h>

#ifndef STASSID
//#define STASSID "ZTE54"
//#define STAPSK  "121211119"

#endif

const char* host = "esp8266";
const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;

void setup(void) {

  Serial.begin(74880);
  Serial.println();
  Serial.println("Booting Sketch...");
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, password);

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
    Serial.println("WiFi failed, retrying.");
  }

  //MDNS.begin(host);

  httpUpdater.setup(&httpServer);
  httpServer.begin();

  //MDNS.addService("http", "tcp", 80);
  
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Do not forget /update   or ->");
   Serial.println("curl -F \"image=@firmware.bin\" esp8266-webupdate.local/update");
}

void loop(void) {
  httpServer.handleClient();
  //MDNS.update();
}
