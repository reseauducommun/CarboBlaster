
//////////////////// S E R V E R /////////////////

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>


///// De :  http://www.esp8266.com/viewtopic.php?f=32&t=5669&start=4#sthash.OFdvQdJF.dpuf ///
extern "C" {
#include<user_interface.h>
}
//Variables
int nbrC;
int apptoconnect=5;
IPAddress addIp[7];
int tempo=200;
int sensorValue;
String datas; 
String Json;
/* configuration  wifi */
const char *ssid = "COblasterGmaster";
//config du relai sur le master
const int relayPin = D1;
ESP8266WebServer server(80);
WiFiUDP Udp;
//char  ReplyBuffer[50] = "42";      // a string to send back
//// //// //// //// //// //// //// //// //// //// //// //// //// //// //// ////
//emission d'un JSON
//// //// //// //// //// //// //// //// //// //// //// //// //// //// //// ///
void handleRoot() { 
  server.send(200, "text/html", "<h1>You are connected</h1>");
}
 //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// ////
 //verification des appareils connectés
 //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// ///
void client_status() { /// This works, ty sohialsadiq!
unsigned char number_client;
struct station_info *stat_info;
struct ip_addr *IPaddress;
IPAddress address;
int i=1;
number_client= wifi_softap_get_station_num(); // Count of stations which are connected to ESP8266 soft-AP
stat_info = wifi_softap_get_station_info();
Serial.print(" Total connected_client are = ");
nbrC=number_client;
Serial.println(number_client);
while (stat_info != NULL) {
IPaddress = &stat_info->ip;
address = IPaddress->addr;
Serial.print("client= ");
Serial.print(i);
Serial.print(" ip adress is = ");
Serial.print((address));
addIp[i-1]=address;
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
delay(2000);
} 
//// //// //// //// //// //// //// //// //// //// //// //// //// //// //// ////
//traitement des données  /////////
//// //// //// //// //// //// //// //// //// //// //// //// //// //// //// ///
String envoiUDP(String tabbin[8]){
 Serial.println("envoi UDP");
  for(int i=0;i<8;i++){
   Serial.println(tabbin[i]);
   if(i==0){
      if(tabbin[i]=="1"){
         digitalWrite(relayPin, HIGH); 
      }else{
         digitalWrite(relayPin, LOW); 
      }
   }else{
        Udp.beginPacket(addIp[i-1], 2390);
        if(tabbin[i]=="1"){
        Udp.write("1"); // envoi du buffer
        }else{
         Udp.write("0"); // envoi du buffer  
        }
        Udp.endPacket();
        }
     }
  }
void binaire() {
datas=String(random(10));
  for(int i=0; i<datas.length(); i++){
    String BinString[8];
    char myChar = datas.charAt(i);
    for(int i=7; i>=0; i--){
      byte bytes = bitRead(myChar,i);
      //Serial.print(bytes, BIN);
      BinString[i]=String(bytes);/// a pousser en tableau et non en chaine le moment venu String(bytes)
      }
    delay(tempo);
    envoiUDP(BinString);
    //Json="{\"senseur\":\""+BinString+"\"}";
    //server.send(200, "application/javascript",Json );  
    } 
}
//// //// //// //// //// //// //// //// //// //// //// //// //// //// //// ////
//set up
//// //// //// //// //// //// //// //// //// //// //// //// //// //// //// ///
void setup() {
  delay(1000);
  pinMode(relayPin, OUTPUT);
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
//// //// //// //// //// //// //// //// //// //// //// //// //// //// //// ////
//boucle
//// //// //// //// //// //// //// //// //// //// //// //// //// //// //// ///
void loop() {
  server.handleClient();   
  int ledelai=2000; 
  ////////scan des objets connectes
  //si inferieur au nombre d'app necessaire
  if(nbrC<apptoconnect){
     client_status();
  //si suffisant lancer la mesure et la propagation des données
  }else{
     Serial.println("on est assez");
     ledelai=0;
     binaire();
  }
  delay(ledelai);
}


