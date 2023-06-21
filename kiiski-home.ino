#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>

byte mac[] = { 0xA8, 0x61, 0x0A, 0xAE, 0x3A, 0xC4 };
EthernetServer server(80);

File page;

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
  Serial.println("SUCCESS - SD card initialized.");
  if (!SD.exists("index.htm")) {
    Serial.println("ERROR - Can't find index.htm file!");
  }
  Serial.println("SUCCESS - Found index.htm file.");
}

void loop() {
  EthernetClient client = server.available();
  if (client) { 
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) { 
        char c = client.read(); 
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
    } 
    delay(1);      
    client.stop(); 
  }
}