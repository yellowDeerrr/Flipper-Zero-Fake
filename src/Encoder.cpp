#include <Arduino.h>
#include "LCD.h"

#define DT_PIN 37 // s2
#define CLK_PIN 38 // 21
#define SW_PIN 39 // key

volatile bool encoderDir = 0; // 0 up, +1 down
volatile int counter = 0;
volatile bool prevCLKValue, currentCLKValue = HIGH;
bool changed = false;

bool isEncoderButtonPressed();
void handleEncoderInterrupt();
bool isEncoderPositionChanged();

void IRAM_ATTR rotary_encoder() {
    handleEncoderInterrupt();
}

void initEncoder(){
    pinMode(CLK_PIN, INPUT);
    pinMode(DT_PIN, INPUT);
    pinMode(SW_PIN, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(CLK_PIN), rotary_encoder, CHANGE);
}

void handleEncoderInterrupt(){
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
}

bool isEncoderButtonPressed(){
    return !digitalRead(SW_PIN);
}

unsigned long lastPrintOut, lastButtonPress = 0;

void printOutEncoderData(unsigned long &now){
    if(now - lastPrintOut >= 50){
         Serial.printf("CLK: %d\n", digitalRead(CLK_PIN));
        Serial.printf("DT: %d\n", digitalRead(DT_PIN));
        Serial.print("Count: ");
        Serial.println(counter);
        lastPrintOut = now;
    }
}

void encoderCheckAndHandle(unsigned long &now){
    if(isEncoderPositionChanged()){
        if (encoderDir) 
            scrollDown();
        else 
            scrollUp();
        changed = false;
    }

    if (isEncoderButtonPressed() && now - lastButtonPress >= 300) {
        Serial.println("Button Pressed");
        displayScanLoading();
        std::vector<WiFiNetwork> nets = scanNetworks();
        displayNetworks(nets);
        // drawMainMenu();
        lastButtonPress = now;
    }
}

bool isEncoderPositionChanged(){
    return changed;
}