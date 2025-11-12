#include <WiFi.h>
#include <esp_wifi.h>
#include <WebServer.h>
#include <vector>

#include "WiFiNetwork.h"

WebServer server(80);

IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

void initNetwork(int n){
    // if(n == 1)
    //     WiFi.mode(WIFI_MODE_STA);
    // else if(n == 2)
    //     WiFi.mode(WIFI_MODE_AP);
    // else if(n == 3)
    //     WiFi.mode(WIFI_MODE_APSTA);
    // else 
    //     WiFi.mode(WIFI_MODE_STA);

    // WiFi.disconnect();

    // esp_wifi_80211_tx();
    WiFi.softAP("ESP32", "12345678", 3, 0, 2);
    WiFi.softAPConfig(local_ip, gateway, subnet);


    IPAddress apIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(apIP);
}

void handleRoot(){
    Serial.printf("Received request from %s\n", server.client().remoteIP().toString().c_str());
    server.send(200, "text/html", "Hi");
}

void handleClient(){
    server.handleClient();
}

void printUsers(){
    uint8_t nClients = WiFi.softAPgetStationNum();
    Serial.printf("Connected clients: %u\n", nClients);
}
void initServer(){
    server.on("/", handleRoot);
    server.onNotFound(handleRoot);
    server.begin();
    Serial.println("HTTP server started");
}

// void nets(){
//     wifi_scan_config_t scan_config = {};
//     scan_config.show_hidden = true;
//     scan_config.scan_type = WIFI_SCAN_TYPE_ACTIVE;
//     ESP_ERROR_CHECK(esp_wifi_scan_start(&scan_config, true));

//     uint16_t ap_count = 0;
//     ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&ap_count));

//     wifi_ap_record_t *ap_records = (wifi_ap_record_t*) 
//         malloc(sizeof(wifi_ap_record_t) * ap_count);
    
//     Serial.printf("Found %d networks:\n", ap_count);

//     for (int i = 0; i < ap_count; i++) {
//         Serial.printf("%2d) SSID: %s | RSSI: %d | Channel: %d | Auth: %d\n",
//               i,
//               (char*)ap_records[i].ssid,
//               ap_records[i].rssi,
//               ap_records[i].primary,
//               ap_records[i].authmode);

//     }

//     free(ap_records);
// }

std::vector<WiFiNetwork> scanNetworks(){
    std::vector<WiFiNetwork> networks;

    int n = WiFi.scanNetworks();

    if(n == 0){
        return networks;
    }else{
        // Serial.printf("%d networks found\n", n);
        
        for(int i = 0; i < n; i++){
            String ssid = WiFi.SSID(i);
            int rssi = WiFi.RSSI(i);
            int channel = WiFi.channel(i);
            String auth;

            switch (WiFi.encryptionType(i)) {
                case WIFI_AUTH_OPEN:           auth = "open"; break;
                case WIFI_AUTH_WEP:            auth = "WEP"; break;
                case WIFI_AUTH_WPA_PSK:        auth = "WPA"; break;
                case WIFI_AUTH_WPA2_PSK:       auth = "WPA2"; break;
                case WIFI_AUTH_WPA_WPA2_PSK:   auth = "WPA+WPA2"; break;
                case WIFI_AUTH_WPA2_ENTERPRISE:auth = "WPA2-EAP"; break;
                case WIFI_AUTH_WPA3_PSK:       auth = "WPA3"; break;
                case WIFI_AUTH_WPA2_WPA3_PSK:  auth = "WPA2+WPA3"; break;
                case WIFI_AUTH_WAPI_PSK:       auth = "WAPI"; break;
                default:                       auth = "unknown"; break;
            }
            networks.emplace_back(ssid, rssi, auth, channel);
            // Serial.printf("%d | SSID: %s (RSSI: %d) \t| Auth: %s \t| Channel: %d\n", i, WiFi.SSID(i).c_str(), WiFi.RSSI(i), auth, WiFi.channel(i));
        }
    }

    return networks;
}

