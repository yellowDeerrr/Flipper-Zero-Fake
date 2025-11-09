#include <Arduino.h>
#include <vector> 

#include "WiFiNetwork.h"
#include "LCD.h"
#include "Encoder.h"



void setup(){
    Serial.begin(115200);
    while(!Serial){
        delay(10);
    }

    initEncoder();

    initDisplay();
    initNetwork(1);

    std::vector<WiFiNetwork> nets = scanNetworks();
    displayNetworks(nets);
}

long unsigned now, lastDisplayPrint = 0;

void loop(){
    now = millis();

    encoderCheckAndHandle(now);

    // printOutEncoderData(now);
}