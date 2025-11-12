// WiFiNetwork.h
#pragma once
#include <Arduino.h>
#include <vector>

class WiFiNetwork {
public:
    String ssid;
    int rssi;
    String auth;
    int channel;

    WiFiNetwork(String ssid, int rssi, String auth, int channel)
        : ssid(ssid), rssi(rssi), auth(auth), channel(channel) {}

    // void print() const {
    //     Serial.printf("SSID: %-20s | RSSI: %4d | Auth: %-8s | Ch: %2d\n",
    //                   ssid.c_str(), rssi, auth.c_str(), channel);
    // }
};


void initNetwork(int n);
std::vector<WiFiNetwork> scanNetworks();
void handleRoot();
void initServer();
void handleClient();
void printUsers();

