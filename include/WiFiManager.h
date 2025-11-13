#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>
#include <WiFi.h>
#include <vector>

class WiFiManager {
public:
    struct NetworkInfo {
        String ssid;
        int rssi;
        int channel;
        uint8_t bssid[6];
        String encryption;
    };

    void setMode(wifi_mode_t mode);
    void setAPConfig(String APssid, String APpassword);
    void setSTAConfig(String ssid, String password);
    bool startAP();
    bool connectSTA();
    bool isConnected();
    bool isAPstarted();
    uint8_t getConnectedUsers();

    String getEncryptionString(wifi_auth_mode_t encryption);
    String getBSSIDString(uint8_t bssid[6]);

    IPAddress getSoftAP_IP();
    IPAddress getSTAIP();

    std::vector<NetworkInfo> scanNetworks(int &returnSize, bool showHidden = false, bool passive = false);
    std::vector<NetworkInfo> scanNetworksManaged(int &returnSize, bool forcedScan = false);
    std::vector<NetworkInfo> getLastScanResults();
    NetworkInfo getNetworkByIdLastScan(int n);
    
private:
    String _ssid;
    String _password;

    String _APssid;
    String _APpassword;

    wifi_mode_t _currentMode;
    bool _connected; 
    bool _APstarted;

    std::vector<NetworkInfo> _lastScanResults;
};


// std::vector<WiFiNetwork> scanNetworks();
// void handleRoot();
// void initServer();
// void handleClient();
// void printUsers();

#endif