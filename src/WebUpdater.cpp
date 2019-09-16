/*
  To upload through terminal you can use: 
  curl -F "image=@firmware.bin" esp8266-webupdate.local/update
*/
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "FS.h"
#include <ESP8266HTTPUpdateServer.h>
//#include <GyverUART.h>;
#include <HTTP.h>
#include <StasWiFi.h>
/*
#define STASSID "SAN"
#define STAPSK  "37212628"
IPAddress ip(192,168,1,222);  //статический IP
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

*/

//#ifndef STASSID
//#define STASSID "ZTE54"
//#define STAPSK  "121211119"

//#endif

const char* host = "esp8266";
const char* ssid1 = STASSID1;
const char* password1 = STAPSK1;
const char* ssid2 = STASSID2;
const char* password2 = STAPSK2;
String ipt = "";
FSInfo fs_info;


ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;

//////////////////////////////////////////////////////////////////////////////////
void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += httpServer.uri();
  message += "\nMethod: ";
  message += (httpServer.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += httpServer.args();
  message += "\n";
  for (uint8_t i=0; i<httpServer.args(); i++){
    message += " " + httpServer.argName(i) + ": " + httpServer.arg(i) + "\n";
  }
  httpServer.send(404, "text/plain", message);
}




//////////////////////////////////////////////////////////////////////////////////
uint cmillis;
void setup(void) {

  Serial.begin(9600);
  Serial.println();
  Serial.println("Booting Sketch...");
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid1, password1);
  WiFi.config(ip, gateway, subnet);
  cmillis=millis();
  while ((WiFi.waitForConnectResult() != WL_CONNECTED) and (cmillis+TtoConn<millis())) {
    //WiFi.begin(ssid1, password1);
    Serial.println(ip.toString());
    //Serial.println("WiFi failed, retrying.");
  }
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
   WiFi.begin(ssid2, password2);
  WiFi.config(ip, gateway, subnet); 
  cmillis=millis();
  while ((WiFi.waitForConnectResult() != WL_CONNECTED) and (cmillis+TtoConn<millis())) {
    //WiFi.begin(ssid2, password2);
    Serial.println(ip.toString());
    //Serial.println("WiFi failed, retrying.");
  }
  }
//////////////////////////////////////////////////////////////////////////////////
  Serial.println("SPIFFS begin");
  SPIFFS.begin();
  //Serial.println("SPIFFS format");
  //SPIFFS.format();
  Serial.println("SPIFFS stop");
  SPIFFS.info(fs_info);
  Serial.println(fs_info.totalBytes);
  httpServer.on("/t", [](){
      Serial.print(httpServer.arg(0)+" "+httpServer.arg(1));
      Serial.println("");
       String addy = httpServer.client().remoteIP().toString();
       //char addyp=addy.toCharArray();
      httpServer.send_P(200, "text/html", res_t);
      //String addy = httpServer.client().remoteIP().toString();
      if(ipt != addy){
      Serial.print(addy);
      Serial.println(" is on the web");
      ipt = addy;
      }
  });
 httpServer.on("/", [](){
      httpServer.send_P(200, "text/html", index_html);
      String addy = httpServer.client().remoteIP().toString();
      if(ipt != addy){
      Serial.print(addy);
      Serial.println(" is on the web");
      ipt = addy;
      }
  });
//////////////////////////////////////////////////////////////////////////////////

  httpServer.onNotFound(handleNotFound);
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
