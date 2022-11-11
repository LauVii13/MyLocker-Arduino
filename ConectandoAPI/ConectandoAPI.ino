

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

const char * ssid = "Honor"; 
const char * password = "senha123"; 
const char * host = "mylocker-api.herokuapp.com"; 
String path = "/students"; // Path of Server
const int httpsPort = 3333; // HTTPS PORT (default: 443)
int refreshtime = 15; // Make new HTTPS request after x seconds


String datarx; // Received data as string
long crontimer;

void setup () {
  delay (1000);
  Serial.begin (115200);
  WiFi.mode (WIFI_OFF);
  delay (1000);
  WiFi.mode (WIFI_STA);
  WiFi.begin (ssid, password);
  Serial.print ("Connecting");
  while (WiFi.status()!= WL_CONNECTED) {
    delay (500);
    Serial.print (".");
  }
  Serial.print ("Connected:");
  Serial.println (ssid);
}

void loop () {
  if (crontimer <millis() / 1000) {
    crontimer = (millis() / 1000) + refreshtime;
    callhttps (); //
  }
}

void callhttps () {
  WiFiClientSecure httpsClient;
  httpsClient.setTimeout(15000);
  delay (1000);
  int retry = 0;
  while ((!httpsClient.connect(host, httpsPort)) && (retry <15)) {
    delay (100);
    Serial.print(".");
    retry ++;
  }
  if (retry == 15) {
    Serial.println("Connection failed");
  }
  else {
    Serial.println("Connected to Server");
  }
  httpsClient.print (String ("GET") + path + 
                    "HTTP / 1.1 \ r \ n" +
                    "Host:" + host +
                    "\ r \ n" + "Connection: close \ r \ n \ r \ n");
  while (httpsClient.connected ()) {
    String line = httpsClient.readStringUntil ('\ n');
    if (line == "\ r") {
      break;
    }
  }
  while (httpsClient.available ()) {
    datarx += httpsClient.readStringUntil ('\ n');
  }
  Serial.println (datarx);
  datarx = "";
}
