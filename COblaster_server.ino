///////////////////////////////////////////////////////
//////////////////// RESEAU DU COMMUN /////////////////
//////////////        CARBO BLASTER   /////////////////
///////////////////////////////////////////////////////
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
///// De :  http://www.esp8266.com/viewtopic.php?f=32&t=5669&start=4#sthash.OFdvQdJF.dpuf ///
extern "C" {
#include<user_interface.h>
}
//Variables
int nbrC;
//nbr d'appareil a connecter (max8)
int apptoconnect=2;
//////////////////////////////////
bool testrelai=false;
///tableau des adresses IP à distribuer
IPAddress addIp[7];
///tempo d'emission des données
int tempo=100;
//valeur du sensor
int sensorValue;
//autres variables
String datas; 
String Json;
/* configuration  wifi */
const char *ssid = "COblasterGmaster";
const char *password = "";
//config du relai sur le master
const int relayPin = D1;
//declaration du serveur
ESP8266WebServer server(80);
//declaration UDP
WiFiUDP Udp;
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
  //changement du nombre max de connexion
   struct softap_config config;
   wifi_softap_get_config(&config); // obtenir la configuration initiale.
   config.max_connection = 8; // changer.
   wifi_softap_set_config(&config);// appliquer la nouvelle config .
   //
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
delay(1000);
} 
//// //// //// //// //// //// //// 
//traitement des données  /////////
//// //// //// //// //// //// //// 
String envoiUDP(String tabbin[8]){
  Serial.println(tabbin[0]);
  Serial.println(tabbin[1]);
  Serial.println(tabbin[2]);
  Serial.println(tabbin[3]);
  Serial.println(tabbin[4]);
  Serial.println(tabbin[5]);
  Serial.println(tabbin[6]);
  Serial.println(tabbin[7]); 
 Serial.println("envoi UDP");
      if(tabbin[0]=="1"){
         digitalWrite(relayPin, HIGH); 
      }else{
         digitalWrite(relayPin, LOW); 
      }
      //
        Udp.beginPacket("192.168.4.2", 2390);
        if(tabbin[1]=="1"){
        Udp.write("1"); // envoi du buffer
        }else{
         Udp.write("0"); // envoi du buffer  
        }
        Udp.endPacket();
      // 
       Udp.beginPacket("192.168.4.3", 2390);
        if(tabbin[2]=="1"){
        Udp.write("1"); // envoi du buffer
        }else{
         Udp.write("0"); // envoi du buffer  
        }
        Udp.endPacket();
        }
       
void binaire() {
  //////////boucle finale a retoucher
/*datas=String(random(10));
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
    } */
    ////////////////////////////boucle de test
   if(testrelai==false){
    String BinString[]={"0","0","0","0","0","0","0","0"};
    testrelai=true;
    envoiUDP(BinString);
   }else{
     String BinString[]={"1","1","1","1","1","1","1","1"};
    testrelai=false;
    envoiUDP(BinString);
   }
   delay(tempo);
   /////////////////////////////
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
  WiFi.softAP(ssid,password,6); 
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
  int ledelai=1000; 
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


