// #include <WiFi.h>
// #include <esp_wifi.h>
// #include <WebServer.h>
// #include <vector>

// #include "WiFiManager.h"

// WebServer server(80);

// IPAddress local_ip(192,168,1,1);
// IPAddress gateway(192,168,1,1);
// IPAddress subnet(255,255,255,0);

// void WiFiNetowrk::initMode(wifi_mode_t mode){
//     WiFi.mode(mode);
//     WiFi.disconnect();

//     // esp_wifi_80211_tx();
//     // WiFi.softAP("ESP32", "12345678", 3, 0, 2);
//     // WiFi.softAPConfig(local_ip, gateway, subnet);


//     // IPAddress apIP = WiFi.softAPIP();
//     // Serial.print("AP IP address: ");
//     // Serial.println(apIP);
// }

// void handleRoot(){
//     Serial.printf("Received request from %s\n", server.client().remoteIP().toString().c_str());
//     server.send(200, "text/html", "Hi");
// }

// void handleClient(){
//     server.handleClient();
// }

// void printUsers(){
//     uint8_t nClients = WiFi.softAPgetStationNum();
//     Serial.printf("Connected clients: %u\n", nClients);
// }
// void initServer(){
//     server.on("/", handleRoot);
//     server.onNotFound(handleRoot);
//     server.begin();
//     Serial.println("HTTP server started");
// }

// // void nets(){
// //     wifi_scan_config_t scan_config = {};
// //     scan_config.show_hidden = true;
// //     scan_config.scan_type = WIFI_SCAN_TYPE_ACTIVE;
// //     ESP_ERROR_CHECK(esp_wifi_scan_start(&scan_config, true));

// //     uint16_t ap_count = 0;
// //     ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&ap_count));

// //     wifi_ap_record_t *ap_records = (wifi_ap_record_t*) 
// //         malloc(sizeof(wifi_ap_record_t) * ap_count);
    
// //     Serial.printf("Found %d networks:\n", ap_count);

// //     for (int i = 0; i < ap_count; i++) {
// //         Serial.printf("%2d) SSID: %s | RSSI: %d | Channel: %d | Auth: %d\n",
// //               i,
// //               (char*)ap_records[i].ssid,
// //               ap_records[i].rssi,
// //               ap_records[i].primary,
// //               ap_records[i].authmode);

// //     }

// //     free(ap_records);
// // }

// std::vector<WiFiNetwork> scanNetworks(){
//     std::vector<WiFiNetwork> networks;

//     int n = WiFi.scanNetworks();

//     if(n == 0){
//         return networks;
//     }else{
//         // Serial.printf("%d networks found\n", n);
        
//         for(int i = 0; i < n; i++){
//             String ssid = WiFi.SSID(i);
//             int rssi = WiFi.RSSI(i);
//             int channel = WiFi.channel(i);
//             String auth;

            // switch (WiFi.encryptionType(i)) {
            //     case WIFI_AUTH_OPEN:           auth = "open"; break;
            //     case WIFI_AUTH_WEP:            auth = "WEP"; break;
            //     case WIFI_AUTH_WPA_PSK:        auth = "WPA"; break;
            //     case WIFI_AUTH_WPA2_PSK:       auth = "WPA2"; break;
            //     case WIFI_AUTH_WPA_WPA2_PSK:   auth = "WPA+WPA2"; break;
            //     case WIFI_AUTH_WPA2_ENTERPRISE:auth = "WPA2-EAP"; break;
            //     case WIFI_AUTH_WPA3_PSK:       auth = "WPA3"; break;
            //     case WIFI_AUTH_WPA2_WPA3_PSK:  auth = "WPA2+WPA3"; break;
            //     case WIFI_AUTH_WAPI_PSK:       auth = "WAPI"; break;
            //     default:                       auth = "unknown"; break;
            // }
//             networks.emplace_back(ssid, rssi, auth, channel);
//             // Serial.printf("%d | SSID: %s (RSSI: %d) \t| Auth: %s \t| Channel: %d\n", i, WiFi.SSID(i).c_str(), WiFi.RSSI(i), auth, WiFi.channel(i));
//         }
//     }

//     return networks;
// }



#include <WiFiManager.h>

void WiFiManager::setMode(wifi_mode_t mode){
    _currentMode = mode;
    WiFi.mode(mode);
}

void WiFiManager::setAPConfig(String APssid, String APpassword){
    _APssid = APssid;
    _APpassword = APpassword;
}

void WiFiManager::setSTAConfig(String ssid, String password){
    _ssid = ssid;
    _password = password;
}

bool WiFiManager::startAP(){
    setMode(WIFI_MODE_AP);
    WiFi.disconnect();

    WiFi.softAP(_APssid, _APpassword);
    delay(100);
    return isAPstarted();
}

bool WiFiManager::connectSTA(){
    setMode(WIFI_MODE_STA);
    WiFi.disconnect();

    WiFi.begin(_ssid, _password);
    
    int attempts = 0;
    while(isConnected() != WL_CONNECTED && attempts++ < 20){
        delay(300);
    }

    isConnected();
    return _connected;
}

bool WiFiManager::isConnected(){
    _connected = WiFi.status();
    return _connected;
}



bool WiFiManager::isAPstarted(){
    if(_currentMode == WIFI_AP || _currentMode == WIFI_AP_STA){
        _APstarted = getSoftAP_IP() != IPAddress(0, 0, 0, 0);
        return _APstarted;
    }
    _APstarted = false;
    return _APstarted;
}

IPAddress WiFiManager::getSoftAP_IP(){
    return WiFi.softAPIP();
}

IPAddress WiFiManager::getSTAIP(){
       if (isConnected() == WL_CONNECTED) {
        return WiFi.localIP();
    }
    return IPAddress(0, 0, 0, 0);
}

uint8_t WiFiManager::getConnectedUsers(){
    if(isAPstarted()) return WiFi.softAPgetStationNum();
    else return -1;
}

std::vector<WiFiManager::NetworkInfo> WiFiManager::scanNetworks(int &returnSize, bool showHidden, bool passive){
    Serial.println(passive ? "Performing passive scan..." : "Performing active scan...");

    _lastScanResults.clear();

    int n = WiFi.scanNetworks(false, false, passive, 300);
    returnSize = n;

    if(n == 0)
        return _lastScanResults;
    
    for(int i = 0; i < n; i++){
        NetworkInfo info;
        info.ssid = WiFi.SSID(i);
        info.rssi = WiFi.RSSI(i);
        info.channel = WiFi.channel(i);
        memcpy(info.bssid, WiFi.BSSID(i), 6);
        info.encryption = getEncryptionString(WiFi.encryptionType(i));

        _lastScanResults.push_back(info);
    }

    return _lastScanResults;
}

std::vector<WiFiManager::NetworkInfo> WiFiManager::scanNetworksManaged(int &returnSize, bool forceScan) {
    bool staConnected = isConnected() && (_currentMode == WIFI_STA || _currentMode == WIFI_AP_STA);
    bool apStarted = isAPstarted();
    
    // Determine scan mode: passive if connected or AP running, unless forced
    bool usePassive = (staConnected || apStarted) && !forceScan;
    
    if (!forceScan && staConnected) {
        Serial.println("Warning: STA connected. Using passive scan to minimize disruption. Use forceScan=true for active.");
    }
    if (!forceScan && apStarted) {
        Serial.println("AP started. Using passive scan to avoid performance impact.");
    }
    
    // Perform the scan with chosen mode
    std::vector<WiFiManager::NetworkInfo> results = scanNetworks(returnSize, false, usePassive);
    
    // Handle reconnection if STA was connected and active scan was used
    if (staConnected && !usePassive) {
        delay(1000);
        if (!isConnected()) {
            Serial.println("STA disconnected after active scan. Reconnecting...");
            connectSTA();
        } else {
            Serial.println("STA reconnected after scan.");
        }
    }
    
    return results;
}

std::vector<WiFiManager::NetworkInfo> WiFiManager::getLastScanResults(){
    return _lastScanResults;
}

WiFiManager::NetworkInfo WiFiManager::getNetworkByIdLastScan(int n){
    return _lastScanResults[n];
}


String WiFiManager::getEncryptionString(wifi_auth_mode_t encryption) {
    switch (encryption) {
        case WIFI_AUTH_OPEN:            return "Open";
        case WIFI_AUTH_WEP:             return "WEP";
        case WIFI_AUTH_WPA_PSK:         return "WPA-PSK";
        case WIFI_AUTH_WPA2_PSK:        return "WPA2-PSK";
        case WIFI_AUTH_WPA_WPA2_PSK:    return "WPA/WPA2-PSK";
        case WIFI_AUTH_WPA2_ENTERPRISE: return "WPA2-Enterprise";
        case WIFI_AUTH_WPA3_PSK:        return "WPA3-PSK";
        case WIFI_AUTH_WPA2_WPA3_PSK:   return "WPA2/WPA3-PSK";
        case WIFI_AUTH_WAPI_PSK:        return "WAPI-PSK";
        case WIFI_AUTH_WPA3_ENT_192:    return "WPA3-Enterprise-192";
        default:                        return "Unknown";
    }
}

String WiFiManager::getBSSIDString(uint8_t bssid[6]) {
    char buf[18]; // "AA:BB:CC:DD:EE:FF" + null terminator
    sprintf(buf, "%02X:%02X:%02X:%02X:%02X:%02X",
            bssid[0], bssid[1], bssid[2],
            bssid[3], bssid[4], bssid[5]);
    return String(buf);
}
