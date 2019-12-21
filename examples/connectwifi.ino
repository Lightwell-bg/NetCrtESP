#include <ESP8266WiFi.h>
#include "NetCrtESP.h"

NetCrtESP myWIFI;

bool changeWIFI = false; //change to true for set new config for Wi-Fi

String ssid = "ssid";
String password = "password";
String ssidAP = "AP-ESP";   // AP name
String passwordAP = ""; // AP password
String SSDP_Name = "ESPDevice"; // SSDP name. You may use it with <ESP8266SSDP.h>
String apIP = "192.168.4.1"; //IP for AP

void setup() {
  Serial.begin(115200);
  if (myWIFI.begin(ssid.c_str(), password.c_str(), SSDP_Name.c_str(), ssidAP.c_str(), passwordAP.c_str(), apIP.c_str())) {
    Serial.println(F("Connect to Wi-Fi or create AP"));
    Serial.println(myWIFI.getDevStatusIP());
  }
  // create only AP
  /*if (myWIFI.begin(ssidAP.c_str(), passwordAP.c_str(), apIP.c_str())) {
    Serial.println(F("Сreate AP"));
    Serial.println(myWIFI.getDevStatusIP());   
  }*/
  Serial.print("Wi-Fi ssid: "); Serial.println(myWIFI.getNameSSID());
  Serial.print("Wi-Fi password: "); Serial.println(myWIFI.getPassSSID());
  Serial.print("AP name: "); Serial.println(myWIFI.getNameAPSSID());
  Serial.print("AP password: "); Serial.println(myWIFI.getPassAPSSID());
}

void loop() {
 if (changeWIFI) {
  String ssid = "ssid2";
  String password = "password2";
  String ssidAP = "AP-ESP2";   // AP name
  String passwordAP = ""; // AP password
  String SSDP_Name = "ESPDevice2"; // SSDP name. You may use it with <ESP8266SSDP.h>
  myWIFI.setConfigWIFI(ssid.c_str(), password.c_str(), SSDP_Name.c_str(), ssidAP.c_str(), passwordAP.c_str());
  Serial.print("Wi-Fi ssid: "); Serial.println(myWIFI.getNameSSID());
  Serial.print("Wi-Fi password: "); Serial.println(myWIFI.getPassSSID());
  Serial.print("AP name: "); Serial.println(myWIFI.getNameAPSSID());
  Serial.print("AP password: "); Serial.println(myWIFI.getPassAPSSID());
  Serial.println("You must restart ESP for for the changes to take effect!!!");
  changeWIFI = false; 
 }
}
