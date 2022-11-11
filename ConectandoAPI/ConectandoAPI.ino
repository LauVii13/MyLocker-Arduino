//https://stackoverflow.com/questions/67828007/how-can-i-send-get-or-post-request-to-heroku-api-using-esp8266

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


HTTPClient https;    //Declare object of class HTTPClient
WiFiClient client;   // Declare out of loop like a global variable


https.begin(client, "https://mylocker-api.herokuapp.com/");      
//Specify request destination
    
    https.addHeader("Content-Type", "application/x-www-form-urlencoded"); //Specify content-type header
      
    int httpCode = https.POST(""); //Send the request    
    
    String payload = https.getString(); //Get the response payload

    Serial.print("[HTTP] POST...\n");
    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] POST... code: %d\n", httpCode);
      
      // file found at server
      if (httpCode == HTTP_CODE_OK) {
        const String& payload = https.getString();
        Serial.println("received payload:\n<<");
        Serial.println(payload);
        Serial.println(">>");
        https.end();  //Close connection
      }
    } else {
      Serial.print (payload);
      Serial.print (httpCode);
      Serial.println(WiFi.localIP());
      
      Serial.printf("[HTTP] POST... failed, error: %s\n", https.errorToString(httpCode).c_str());
      https.end();  //Close connection
    }
