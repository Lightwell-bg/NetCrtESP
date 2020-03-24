#ifndef NetCrtESP_h
#define NetCrtESP_h

#include <Arduino.h>
#include <EEPROM.h>
#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif

#define cfgStart 0 // Tell it where to store your config data in EEPROM

const char checkVer[] = "abv"; //For first init EEPROM

const uint8_t lenStr = 20;

typedef struct storeStruct_t {
    char myVersion[3];
    char nameSSID[lenStr];
    char passSSID[lenStr];
    char nameSSDP[lenStr];
    char nameAPSSID[lenStr];
    char passAPSSID[lenStr];
};

typedef struct srtctIP {
    uint8_t oct1;
    uint8_t oct2;
    uint8_t oct3;
    uint8_t oct4;
};

class NetCrtESP {
    public:
        NetCrtESP(); 
        bool begin(const char* ssidAP, const char* passwordAP, const char* apLocalIP = "192.168.4.1");
        bool begin(const char* ssid, const char* password, const char* SSDP_Name, const char* ssidAP, const char* passwordAP, const char* apLocalIP = "192.168.4.1");
        bool reconectWIFI();
        String getDevStatusIP();
        String getNameSSID();
        String getPassSSID();
        String getNameSSDP();
        String getNameAPSSID();
        String getPassAPSSID();
        bool setConfigWIFI(const char* ssid, const char* password, const char* SSDP_Name, const char* ssidAP, const char* passwordAP);        
        srtctIP strToIPArr(const char* chIP);
    private:
        char _apLocalIP[15];
        storeStruct_t _configWIFI;
        bool _modeAP; //after connect we know AP or not
        bool _startAPMode();
        bool _saveConfigEEPROM(storeStruct_t _conf);
        void _loadConfigEEPROM();
};

#endif