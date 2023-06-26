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

            //Looking for ASSETS/IMAGES/*
            if (request.indexOf("assets/images") > 0){
              fileName = "assets/images/";             
              if (request.indexOf("assets/images/favicon.png") >= 0) { fileName += "favicon.png"; }
              if (request.indexOf("assets/images/joona.png") >= 0) { fileName += "joona.png"; }
              if (request.indexOf("assets/images/senja.png") >= 0) { fileName += "senja.png"; }
              if (request.indexOf("assets/images/lilja.png") >= 0) { fileName += "lilja.png"; }
              if (request.indexOf("assets/images/noora.png") >= 0) { fileName += "noora.png"; }
              if (request.indexOf("assets/images/petteri.png") >= 0) { fileName += "petteri.png"; }
              if (request.indexOf("assets/images/susanna.png") >= 0) { fileName += "susanna.png"; }
              if (request.indexOf("assets/images/luukas.png") >= 0) { fileName += "luukas.png"; }

              client.println("HTTP/1.1 200 OK");
              client.println("Content-Type: image/png");
              client.println();

            //Looking for ASSETS/script.js
            } else if (request.indexOf("assets/script.js") > 0){
              fileName = "assets/script.js";
              client.println("HTTP/1.1 200 OK");
              client.println("Content-Type: text/javascript");
              client.println();

            //Looking for ASSETS/style.css
            } else if (request.indexOf("assets/style.css") > 0){
              fileName = "assets/style.css";
              client.println("HTTP/1.1 200 OK");
              client.println("Content-Type: text/css");
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
              while (file.available()){
                client.write(file.read());
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