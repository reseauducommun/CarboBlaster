
//////////////////// S E R V E R /////////////////

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>


///// De :  http://www.esp8266.com/viewtopic.php?f=32&t=5669&start=4#sthash.OFdvQdJF.dpuf ///
extern "C" {
#include<user_interface.h>
}

/* configuration  wifi */
const char *ssid = "COblaster";

ESP8266WebServer server(80);
WiFiUDP Udp;
char  ReplyBuffer[50] = "42";      // a string to send back

void handleRoot() { 
  server.send(200, "text/html", "<h1>You are connected</h1>");
  //String addy = server.client().remoteIP().toString();
  //Serial.println(addy);
}

void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");
  WiFi.softAP(ssid);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");  
}
 
void loop() {
  ///// server.handleClient();    /// cette fonction consomme ~ 200ms
   ///// delay(2000);
   //// client_status();  /// cette fonction consomme ~ 250ms
  ///// delay(2000);

  Serial.println("envoi UDP");
  //Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());  /// à adapter avec les résultats de client_status
  Udp.beginPacket("192.168.4.2", 2390);
  Udp.write(ReplyBuffer); // envoi du buffer
  Udp.endPacket();
  // Serial.println("fin envoi UDP");
  delay(50); // 35-40 commence à bugger (très beau), 50 ok
}

void client_status()  /// This works, ty sohialsadiq!

{

unsigned char number_client;
struct station_info *stat_info;

struct ip_addr *IPaddress;
IPAddress address;
int i=1;

number_client= wifi_softap_get_station_num(); // Count of stations which are connected to ESP8266 soft-AP
stat_info = wifi_softap_get_station_info();

Serial.print(" Total connected_client are = ");
Serial.println(number_client);

while (stat_info != NULL) {

IPaddress = &stat_info->ip;
address = IPaddress->addr;

Serial.print("client= ");

Serial.print(i);
Serial.print(" ip adress is = ");
Serial.print((address));
Serial.print(" with mac adress is = ");

Serial.print(stat_info->bssid[0],HEX);
Serial.print(stat_info->bssid[1],HEX);
Serial.print(stat_info->bssid[2],HEX);
Serial.print(stat_info->bssid[3],HEX);
Serial.print(stat_info->bssid[4],HEX);
Serial.print(stat_info->bssid[5],HEX);

stat_info = STAILQ_NEXT(stat_info, next);
i++;
Serial.println();

}

delay(500);

} 


