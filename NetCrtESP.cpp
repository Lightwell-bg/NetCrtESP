#include <Arduino.h>
#include <NetCrtESP.h>

NetCrtESP::NetCrtESP() {
}

bool NetCrtESP::begin(const char* ssidAP, const char* passwordAP, const char* apLocalIP) {
    for (uint8_t i=0; i<strlen(checkVer); i++) {_configWIFI.myVersion[i] = checkVer[i];}
    _configWIFI.nameAPSSID = ssidAP;
    _configWIFI.passAPSSID = passwordAP;
    _loadConfigEEPROM();
    _apLocalIP = apLocalIP;
    _ssidAP = ssidAP;
    //Serial.println(F("WiFi up AP"));
    if (_startAPMode())  {
        _modeAP = true;
        //Serial.print(F("AP IP address: ")); Serial.print(WiFi.softAPIP());
        //Serial.print(F(" AP Name: ")); Serial.println(_configWIFI.nameAPSSID);
        return true; 
    }
    else {
        Serial.println(F("Can't create AP"));
        return false;
    }    
}

bool NetCrtESP::begin(const char* ssid, const char* password, const char* SSDP_Name, const char* ssidAP, const char* passwordAP, const char* apLocalIP) {
    for (uint8_t i=0; i<3; i++) {_configWIFI.myVersion[i] = checkVer[i];}
    _configWIFI.nameSSID = ssid;
    _configWIFI.passSSID = password;
    _configWIFI.nameSSDP = SSDP_Name;
    _configWIFI.nameAPSSID = ssidAP;
    _configWIFI.passAPSSID = passwordAP;
    _loadConfigEEPROM();    
    _apLocalIP = apLocalIP;
    _ssidAP = ssidAP;
    WiFi.mode(WIFI_STA); //WiFi.mode(WIFI_AP_STA);
    uint8_t tries = 10;
    WiFi.begin(_configWIFI.nameSSID.c_str(), _configWIFI.passSSID.c_str());
    while (--tries && WiFi.status() != WL_CONNECTED)  {   
        Serial.print(F("."));
        delay(1000);
    }
    if (WiFi.status() != WL_CONNECTED)  {   
        Serial.println(F("WiFi up AP"));
        if (_startAPMode())  {
            _modeAP = true;
            Serial.print(F("AP IP address: ")); Serial.print(WiFi.softAPIP());
            Serial.print(F(" AP Name: ")); Serial.println(_configWIFI.nameAPSSID);
            return true;  
        }
        else {
            Serial.println(F("Can't create AP"));
            return false;
        }  
    }
    else {  
        _modeAP = false;
        //Serial.println(F("WiFi connected"));
        //Serial.print(F("IP address: "));    Serial.println(WiFi.localIP());
        return true; 
    }
}

bool NetCrtESP::_startAPMode() {
    srtctIP myIP = strToIPArr(_apLocalIP.c_str());
    IPAddress apLocalIPCl(myIP.oct1,myIP.oct2,myIP.oct3,myIP.oct4);
    WiFi.disconnect();   
    WiFi.mode(WIFI_AP);   // Меняем режим на режим точки доступа
    WiFi.softAPConfig(apLocalIPCl, apLocalIPCl, IPAddress(255, 255, 255, 0));  
    if (WiFi.softAP(_configWIFI.nameAPSSID.c_str(), _configWIFI.passAPSSID.c_str())) return true;
    else {
        Serial.println(F("Can't create AP"));
        return false;
    }
}

String  NetCrtESP::getDevStatusIP() {
    if (_modeAP) return String("WIFI_AP " + WiFi.softAPIP().toString() + " AP name " + _ssidAP);
    else return String("WIFI_STA " + WiFi.localIP().toString());
}

srtctIP NetCrtESP::strToIPArr(const char* chIP) {
    srtctIP apAPt;
    String strIP = String(chIP);
    uint8_t ind1 = strIP.indexOf(".");
    apAPt.oct1 = strIP.substring(0,ind1).toInt();
    uint8_t ind2 = strIP.indexOf(".",ind1+1);
    apAPt.oct2 = strIP.substring(ind1+1,ind2).toInt();
    ind1 = strIP.indexOf(".",ind2+1);
    apAPt.oct3 = strIP.substring(ind2+1,ind1).toInt();
    ind2 = strIP.indexOf(".",ind1);
    apAPt.oct4 = strIP.substring(ind2+1).toInt();
    return apAPt;
}

String NetCrtESP::getNameSSID() {
    return _configWIFI.nameSSID;
}
String NetCrtESP::getPassSSID() {
    return _configWIFI.passSSID;    
}
String NetCrtESP::getNameSSDP() {
    return _configWIFI.nameSSDP;       
}
String NetCrtESP::getNameAPSSID() {
    return _configWIFI.nameAPSSID; 
}
String NetCrtESP::getPassAPSSID() {
    return _configWIFI.passAPSSID; 
}

bool NetCrtESP::setConfigWIFI(const char* ssid, const char* password, const char* SSDP_Name, const char* ssidAP, const char* passwordAP) {
    storeStruct_t newConfWIFI;
    for (uint8_t i=0; i<3; i++) {newConfWIFI.myVersion[i] = checkVer[i];}
    _configWIFI.nameSSID = ssid;
    _configWIFI.passSSID = password;
    _configWIFI.nameSSDP = SSDP_Name;
    _configWIFI.nameAPSSID = ssidAP;
    _configWIFI.passAPSSID = passwordAP;
    if(_saveConfigEEPROM(_configWIFI)) return true; else return false;       
}

bool NetCrtESP::_saveConfigEEPROM(storeStruct_t _conf) {   // Save configuration from RAM into EEPROM
    EEPROM.begin(4095);
    EEPROM.put(cfgStart, _conf);
    //delay(200);
    if (EEPROM.commit()) {                      // Only needed for ESP8266 to get data written
        EEPROM.end();                           // Free RAM copy of structure
        return true; 
    }
    else {
        EEPROM.end();                           // Free RAM copy of structure
        return false;         
    }                         
                        
}

void NetCrtESP::_loadConfigEEPROM() {  // Loads configuration from EEPROM into RAM
    Serial.println("Loading config EEPROM");
    storeStruct_t load;
    EEPROM.begin(4095);
    EEPROM.get(cfgStart, load);
    EEPROM.end();
    // Check if it is your real struct
    if (load.myVersion[0] != checkVer[0] ||
        load.myVersion[1] != checkVer[1] ||
        load.myVersion[2] != checkVer[2]) {
        _saveConfigEEPROM(_configWIFI);
        return;
    }
    _configWIFI = load;
    /*Serial.print("_configWIFI.myVersion ");Serial.println(_configWIFI.myVersion);
    Serial.print("_configWIFI.nameSSID ");Serial.println(_configWIFI.nameSSID);
    Serial.print("_configWIFI.passSSID ");Serial.println(_configWIFI.passSSID);
    Serial.print("_configWIFI.nameSSDP ");Serial.println(_configWIFI.nameSSDP);
    Serial.print("_configWIFI.nameAPSSID ");Serial.println(_configWIFI.nameAPSSID);
    Serial.print("_configWIFI.passAPSSID ");Serial.println(_configWIFI.passAPSSID);*/
}