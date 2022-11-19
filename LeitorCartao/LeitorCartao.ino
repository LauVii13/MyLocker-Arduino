#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

#define SS_PIN D8
#define RST_PIN D0

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

MFRC522::MIFARE_Key key;

const char * ssid = "Honor"; 
const char * password = "senha123"; 
const char * host = "mylocker-api.herokuapp.com"; 
String path = "/students"; // Path of Server
const int httpsPort = 3333; // HTTPS PORT (default: 443)
int refreshtime = 15; // Make new HTTPS request after x seconds


HTTPClient http;    //Declare object of class HTTPClient
WiFiClient client;   // Declare out of loop like a global variable


// Init array that will store new NUID
byte nuidPICC[4];

void setup() {
  Serial.begin(115200);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522
  rfid.PCD_DumpVersionToSerial();

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
}

String ra = "200155";
String cartao = "65 117 26 38";
String leitor = "";
String armario = "1";

void loop() {

  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  if ( ! rfid.PICC_ReadCardSerial())
    return;

  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
          
  for (byte i = 0; i < 4; i++) {
    nuidPICC[i] = rfid.uid.uidByte[i];
  }

  leitor = "";
              
  Serial.println();
             
  Serial.print(F("In dec: "));
          
  printDec(rfid.uid.uidByte, rfid.uid.size);
  Serial.println();

  Serial.println("Cartao: " + cartao);

  Serial.println("Leitor: " + leitor);

  GetRequest("http://mylocker-api.herokuapp.com/students/" + ra + "/lockers/" + armario);

  delay(500);
  /*
    //verificacao se é dono ou n
    if(cartao == leitor){
      Serial.println("Armario aberto");
    }
    else{
      Serial.println("Armario nao liberado");
    }*/
    
    rfid.PICC_HaltA();
          
    rfid.PCD_StopCrypto1();
}

void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);

      
     if(i == 0){
      if(buffer[i] < 10){
          leitor = "0" + String(buffer[i], DEC);
        }
        else{
          leitor =String(buffer[i], DEC);
        }
     }
     else{
      if(buffer[i] < 10){
          leitor = leitor + " 0" + buffer[i];
        }
        else{
          leitor = leitor + " " + buffer[i];
        }
     }
        
  }
}

void GetRequest(String url){
  yield();

  Serial.println(url);

  if (http.begin(client, url)){
    Serial.println("http.begin ok");
  }
  
  delay(100);
  int httpCode = http.GET();
  Serial.print("HTTP STATUS: ");
  const String& payload = http.getString();
  Serial.println(httpCode);
  Serial.println(payload);

  if(httpCode > 0){
    switch (httpCode){
      yield();
      
      case 200:
          Serial.println("Valor correto(200)");
        break;
      
      case 400:
          Serial.println("Valor incorreto(400)");
        break;
      
      default:
          Serial.println("Nem um nem outro");
        break;
    }
  }
  else{
    Serial.println("Fail Request");
  }

}
