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
          Serial.println("ENDED - CHECK THIS ALL");
          int indx = request.indexOf("GET /");
          Serial.println(indx);
          
          //GET Request
          if (indx >= 0){
            String fileName;

            //Looking for ASSETS/IMAGES/*
            if (request.indexOf("assets/images") > 0){
              //Get filename
              while(request.charAt(indx) != ' '){
                fileName += request.charAt(indx);
                indx++;
              }

              client.println("HTTP/1.1 200 OK");
              client.println("Content-Type: image/png");
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
            }
            break;
          }
        }
      }
    } 
    delay(1);      
    client.stop(); 
  }
}