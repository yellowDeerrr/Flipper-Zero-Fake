#include <Arduino.h>
#include <vector> 
#include "WiFiNetwork.h"
#include "LCD.h"

#define DT_PIN 37 // s2
#define CLK_PIN 38 // 21
#define SW_PIN 39 // key

volatile bool encoderDir = 0; // 0 up, +1 down
volatile int counter = 0;
volatile bool prevCLKValue, currentCLKValue = HIGH;
bool changed = false;


void IRAM_ATTR rotary_encoder() {
    currentCLKValue = digitalRead(CLK_PIN);

    if(prevCLKValue == HIGH && currentCLKValue == LOW && !changed){
        if(digitalRead(DT_PIN) == HIGH){
            counter++;
            encoderDir = true;
        }
        else{
            counter--;
            encoderDir = false;
        }

        changed = true;
    }

    prevCLKValue = currentCLKValue;

    // lastTimeEncoder = millis();
}

void setup(){
    Serial.begin(115200);
    while(!Serial){
        delay(10);
    }

    pinMode(CLK_PIN, INPUT);
    pinMode(DT_PIN, INPUT);
    pinMode(SW_PIN, INPUT_PULLUP);

    initDisplay();
    initNetwork(1);

    attachInterrupt(digitalPinToInterrupt(CLK_PIN), rotary_encoder, CHANGE);

    std::vector<WiFiNetwork> nets = scanNetworks();
    displayNetworks(nets);
}

long unsigned now, lastPrintOut, lastDisplayPrint, lastButtonPress = 0;

void loop(){
    now = millis();

    if (changed) {
        
        if (encoderDir) 
            scrollDown();
        else 
            scrollUp();
        // Serial.print("Count: ");
        // Serial.println(counter);
        changed = false;
    }


    if (digitalRead(SW_PIN) == LOW && now - lastButtonPress >= 300) {
        Serial.println("Button Pressed");
        std::vector<WiFiNetwork> nets = scanNetworks();
        displayNetworks(nets);
        // drawMainMenu();
        lastButtonPress = now;
    }

    if(now - lastPrintOut >= 50){
        Serial.printf("CLK: %d\n", digitalRead(CLK_PIN));
        Serial.printf("DT: %d\n", digitalRead(DT_PIN));
        Serial.print("Count: ");
        Serial.println(counter);
        lastPrintOut = now;
    }
}