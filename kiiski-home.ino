#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>

byte mac[] = { 0xA8, 0x61, 0x0A, 0xAE, 0x3A, 0xC4 };
EthernetServer server(80);

File page;

String request;

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

  //Initialized SD Card
  Serial.println("SUCCESS - SD card initialized.");
  if (!SD.exists("index.htm")) {
    Serial.println("ERROR - Can't find index.htm file!");
  }

  //Found file
  Serial.println("SUCCESS - Found index.htm file.");
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

        if (c == '\n'){
          
          int indx = request.indexOf("GET");

          //Requesting file
          if (request.indexOf("GET") >= 0){
            String  = request.substring(4, request.length());

            Serial.println("FILENAME");
            Serial.println(fileName);
            Serial.println("END");


          }
        }
      }

      /*
        //Last line of client req is always blank
        if (c == '\n' && currentLineIsBlank) {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println();
          
          page = SD.open("index.htm");        
          if (page) {
            while (page.available()) {
              client.write(page.read());
            }
            page.close();
          }
          break; 
        }
        if (c == '\n') {
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          currentLineIsBlank = false;
        }
      } 
      */
    } 
    delay(1);      
    client.stop(); 
  }
}