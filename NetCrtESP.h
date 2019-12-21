#ifndef NetCrtESP_h
#define NetCrtESP_h

#include <Arduino.h>
#include <EEPROM.h>
#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif

#define cfgStart 32 // Tell it where to store your config data in EEPROM

const char checkVer[] = "abc"; //For first init EEPROM

typedef struct  {
    char myVersion[3];
    String nameSSID;
    String passSSID;
    String nameSSDP;
    String nameAPSSID;
    String passAPSSID;
} storeStruct_t;

typedef struct {
    uint8_t oct1;
    uint8_t oct2;
    uint8_t oct3;
    uint8_t oct4;
} srtctIP;

class NetCrtESP {
    public:
        NetCrtESP(); 
        bool begin(const char* ssidAP, const char* passwordAP, const char* apLocalIP = "192.168.4.1");
        bool begin(const char* ssid, const char* password, const char* SSDP_Name, const char* ssidAP, const char* passwordAP, const char* apLocalIP = "192.168.4.1");
        String getDevStatusIP();
        String getNameSSID();
        String getPassSSID();
        String getNameSSDP();
        String getNameAPSSID();
        String getPassAPSSID();
        bool setConfigWIFI(const char* ssid, const char* password, const char* SSDP_Name, const char* ssidAP, const char* passwordAP);        
        srtctIP strToIPArr(const char* chIP);
    private:
        String _apLocalIP;
        String _ssidAP;
        storeStruct_t _configWIFI;
        bool _modeAP; //after connect we know AP or not
        bool _startAPMode();
        bool _saveConfigEEPROM(storeStruct_t _conf);
        void _loadConfigEEPROM();
};

#endif