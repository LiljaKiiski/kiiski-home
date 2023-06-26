#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>

byte mac[] = { 0xA8, 0x61, 0x0A, 0xAE, 0x3A, 0xC4 };
EthernetServer server(80);

String request;
String fileName;

void setup() {
  Ethernet.begin(mac);
  server.begin();
  Serial.begin(9600);

  Serial.println(Ethernet.localIP());

  Serial.println("Initializing SD card...");
  if (!SD.begin(4)) {
    Serial.println("ERROR - SD card initialization failed!");
    return;
  }
  Serial.println("Successfully initialized SD card!");
}

void loop() {
  EthernetClient client = server.available();

  //Got a client
  if (client) { 
    boolean currentLineIsBlank = true;

    //While client is connected
    while (client.connected()) {
      if (client.available()) { 
        char c = client.read(); 

        request += c;
        Serial.print(c);

        if (c == '\n'){
          Serial.println(request);
          int indx = request.indexOf("GET /");
          
          //GET Request
          if (indx >= 0){
            String fileName;

            //Looking for IMAGES/*
            if (request.indexOf("assets/") > 0){
              fileName = "assets/";             
              if (request.indexOf("assets/joona.png") >= 0) { fileName += "joona.png"; }
              if (request.indexOf("assets/senja.png") >= 0) { fileName += "senja.png"; }
              if (request.indexOf("assets/lilja.png") >= 0) { fileName += "lilja.png"; }
              if (request.indexOf("assets/noora.png") >= 0) { fileName += "noora.png"; }
              if (request.indexOf("assets/petteri.png") >= 0) { fileName += "petteri.png"; }
              if (request.indexOf("assets/susanna.png") >= 0) { fileName += "susanna.png"; }
              if (request.indexOf("assets/luukas.png") >= 0) { fileName += "luukas.png"; }

              client.println("HTTP/1.1 200 OK");
              client.println("Content-Type: image/png");
              client.println();

            //Index.htm
            } else {
              fileName = "index.htm";
              client.println("HTTP/1.1 200 OK");
              client.println("Content-Type: text/html");
              client.println();
            }

            //Send client
            Serial.print("FILENAME: ");
            Serial.println(fileName);
            File file = SD.open(fileName);
            if (file){
              
              byte tBuf[64];
              int clientCount = 0;
              
              //Image
              if (fileName.indexOf(".png") >= 0){
                while(file.available()) {
                  tBuf[clientCount] = file.read();
                  clientCount++;
                  if(clientCount > 63) {
                    client.write(tBuf,64);
                    clientCount = 0;
                  }

                  //write in any stragglers
                  if(clientCount > 0) {
                    client.write(tBuf,clientCount);
                  }
                }

              //Index.htm
              } else {
                while (file.available()){
                  client.write(file.read());
                }
              }
              file.close();

            } else {
              Serial.print(fileName);
              Serial.println(" does NOT exist!");
            }
            break;
          }
          request = "";
        }
      }
    } 
    delay(1);      
    client.stop(); 
  }
}