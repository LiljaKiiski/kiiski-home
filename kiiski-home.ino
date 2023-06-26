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
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println();

            //Send index.htm to client
            File file = SD.open("index.htm");
            if (file){
              
                while (file.available()){
                  client.write(file.read());
                }
              file.close();

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