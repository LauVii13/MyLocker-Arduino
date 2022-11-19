// link da conexao com api: https://blogmasterwalkershop.com.br/embarcados/nodemcu/nodemcu-como-criar-um-web-server-e-conectar-a-uma-rede-wifi


#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>

#define SS_PIN D8
#define RST_PIN D0

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

MFRC522::MIFARE_Key key;

// Init array that will store new NUID
byte nuidPICC[4];

const char * ssid = "Honor";
const char * password = "senha123";
WiFiServer server(80); 


void setup() {
  Serial.begin(115200);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522
  //Serial.println();
  //Serial.print(F("Reader :"));
  rfid.PCD_DumpVersionToSerial();


        Serial.println(""); //PULA UMA LINHA NA JANELA SERIAL
        Serial.println(""); //PULA UMA LINHA NA JANELA SERIAL
        Serial.print("Conectando a "); //ESCREVE O TEXTO NA SERIAL
        Serial.print(ssid); //ESCREVE O NOME DA REDE NA SERIAL
         
        WiFi.begin(ssid, password); //PASSA OS PARÂMETROS PARA A FUNÇÃO QUE VAI FAZER A CONEXÃO COM A REDE SEM FIO
         
        while (WiFi.status() != WL_CONNECTED) { //ENQUANTO STATUS FOR DIFERENTE DE CONECTADO
        delay(500); //INTERVALO DE 500 MILISEGUNDOS
        Serial.print("."); //ESCREVE O CARACTER NA SERIAL
        }
        Serial.println(""); //PULA UMA LINHA NA JANELA SERIAL
        Serial.print("Conectado a rede sem fio "); //ESCREVE O TEXTO NA SERIAL
        Serial.println(ssid); //ESCREVE O NOME DA REDE NA SERIAL
        server.begin(); //INICIA O SERVIDOR PARA RECEBER DADOS NA PORTA DEFINIDA EM "WiFiServer server(porta);"
        Serial.println("Servidor iniciado"); //ESCREVE O TEXTO NA SERIAL


        Serial.print("IP para se conectar ao NodeMCU: "); //ESCREVE O TEXTO NA SERIAL
        Serial.print("http://"); //ESCREVE O TEXTO NA SERIAL
        Serial.println(WiFi.localIP()); //ESCREVE NA SERIAL O IP RECEBIDO DENTRO DA REDE SEM FIO (O IP NESSA PRÁTICA É RECEBIDO DE FORMA AUTOMÁTICA)









          

  pinMode(D1, INPUT);

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
}

String cartao = "65 117 26 38";
String leitor = "";

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
           
              printDec(rfid.uid.uidByte, rfid.uid.size);
              Serial.println();

              Serial.println("Leitor: " + leitor);



          //verificacao se é dono ou n
              if(cartao == leitor){
                Serial.println("Armario aberto");
              }
              else{
                Serial.println("Armario nao liberado");
              }
    
            rfid.PICC_HaltA();
          
            rfid.PCD_StopCrypto1();
}

void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    //Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    //Serial.print(buffer[i], DEC);

// pega o valor do nogocio   
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
