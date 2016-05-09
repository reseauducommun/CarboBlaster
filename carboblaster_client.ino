/////////////////////////// C L I E N T /////////////////////

/* Adapté de WiFi UDP Send and Receive String
 This sketch wait an UDP packet on localPort using a WiFi shield.
 When a packet is received an Acknowledge packet is sent to the client on port remotePort
 Circuit:
 * WiFi shield attached
 created 30 December 2012
 by dlf (Metodo2 srl) */

#include <SPI.h>
#include <ESP8266WiFi.h> // changement pour pointer à la bonne librairie et pas la lib wifi du arduino
#include <WiFiUdp.h>

const char* ssid = "COblaster";
unsigned int localPort = 2390;      // local port to listen on
int attenteIP = 4000; // délai d'attente pour obtenir une adresse 

char packetBuffer[1]; //buffer to hold incoming packet
//bool packetBuffer; //buffer to hold incoming packet


const int relayPin = D1;
// const long interval = 30;  // 12-20ms commence à bugger (très beau) pause pour le relai 30 ms fonctionne

WiFiUDP Udp;

void printWiFiStatus() {

  Serial.println("");
  Serial.print("Connecté à: ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}



void setup() {
  pinMode(relayPin, OUTPUT);
  Serial.begin(115200);
  WiFi.begin(ssid); // pas besoin de mot de passe
  
  // if you get a connection, report back via serial:
  Udp.begin(localPort);
}

void loop() {

  // Check if module is still connected to WiFi.
  if (WiFi.status() != WL_CONNECTED) {

    while (WiFi.status() != WL_CONNECTED) {
      Serial.println("on réessaie de se connecter");
      digitalWrite(relayPin, LOW);  // turn off relay with voltage LOW
      delay(attenteIP);
    }
  
    // Print the new IP to Serial.
    printWiFiStatus();
  }

  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remoteIp = Udp.remoteIP();
    Serial.print(remoteIp);
    Serial.print(", port ");
    Serial.println(Udp.remotePort());

    // read the packet into packetBufffer
    int len = Udp.read(packetBuffer, 1);
    if (len > 0) packetBuffer[len] = 0;
    String data = String(packetBuffer);
    Serial.println(data);

    if (data=="1"){
      digitalWrite(relayPin, HIGH); // turn on relay with voltage HIGH 
      }
     
    else {
     digitalWrite(relayPin, LOW);  // turn off relay with voltage LOW
      }
    
  }
}

