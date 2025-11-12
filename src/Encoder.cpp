// #include <Arduino.h>
// #include "LCD.h"

// #define DT_PIN 37 // s2
// #define CLK_PIN 38 // 21
// #define SW_PIN 39 // key

// volatile bool encoderDir = 0; // 0 up, +1 down
// volatile int counter = 0;
// volatile bool prevCLKValue, currentCLKValue = HIGH;
// bool changed = false;

// bool isEncoderButtonPressed();
// bool isEncoderPositionChanged();

// void IRAM_ATTR rotary_encoder() {
//     currentCLKValue = digitalRead(CLK_PIN);

//     if(prevCLKValue == HIGH && currentCLKValue == LOW && !changed){
//         if(digitalRead(DT_PIN) == HIGH){
//             counter++;
//             encoderDir = true;
//         }
//         else{
//             counter--;
//             encoderDir = false;
//         }

//         changed = true;
//     }

//     prevCLKValue = currentCLKValue;
// }

// void initEncoder(){
//     pinMode(CLK_PIN, INPUT);
//     pinMode(DT_PIN, INPUT);
//     pinMode(SW_PIN, INPUT_PULLUP);

//     attachInterrupt(digitalPinToInterrupt(CLK_PIN), rotary_encoder, FALLING);
// }


// bool isEncoderButtonPressed(){
//     return !digitalRead(SW_PIN);
// }

// unsigned long lastPrintOut, lastButtonPress = 0;

// void printOutEncoderData(unsigned long &now){
//     if(now - lastPrintOut >= 50){
//          Serial.printf("CLK: %d\n", digitalRead(CLK_PIN));
//         Serial.printf("DT: %d\n", digitalRead(DT_PIN));
//         Serial.print("Count: ");
//         Serial.println(counter);
//         lastPrintOut = now;
//     }
// }

// void encoderCheckAndHandle(unsigned long &now){
//     if(isEncoderPositionChanged()){
//         if (encoderDir) 
//             scrollDown();
//         else 
//             scrollUp();
//         changed = false;
//     }

//     if (isEncoderButtonPressed() && now - lastButtonPress >= 300) {
//         Serial.println("Button Pressed");
//         // displayScanLoading();
//         // std::vector<WiFiNetwork> nets = scanNetworks();
//         // displayNetworks(nets);
//         lastButtonPress = now;
//     }
// }

// bool isEncoderPositionChanged(){
//     return changed;
// }



#include "Encoder.h"

Encoder* Encoder::instance = nullptr;

Encoder::Encoder(int clkPin, int dtPin, int swPin) {
  pinCLK = clkPin;
  pinDT = dtPin;
  pinSW = swPin;
  position = 0;
  lastCLK = HIGH;
  minBound = 0;
  maxBound = 100;
  btnPressed = false;
  lastDebounceTime = 0;
  instance = this;
}

void Encoder::init() {
  pinMode(pinCLK, INPUT_PULLUP);
  pinMode(pinDT, INPUT_PULLUP);
  pinMode(pinSW, INPUT_PULLUP);
  
  attachInterrupt(digitalPinToInterrupt(pinCLK), readEncoderISR, CHANGE);
}

void Encoder::readEncoderISR() {
  if (instance) {
    instance->readEncoder();
  }
}

void Encoder::readEncoder() {
  int clkState = digitalRead(pinCLK);
  
  if (clkState != lastCLK && clkState == LOW) {
    if (digitalRead(pinDT) == HIGH) {
      position++;
    } else {
      position--;
    }
    
    // Keep within bounds
    if (position < minBound) position = minBound;
    if (position > maxBound) position = maxBound;
  }
  lastCLK = clkState;
}

void Encoder::setBounds(int min, int max) {
  minBound = min;
  maxBound = max;
  
  // Adjust position if out of new bounds
  if (position < minBound) position = minBound;
  if (position > maxBound) position = maxBound;
}

void Encoder::update() {
  // This can be used for non-ISR button reading if needed
}

bool Encoder::buttonPressed() {
  if (digitalRead(pinSW) == LOW && !btnPressed) {
    if (millis() - lastDebounceTime > 200) {
      btnPressed = true;
      lastDebounceTime = millis();
      return true;
    }
  }
  
  if (digitalRead(pinSW) == HIGH) {
    btnPressed = false;
  }
  
  return false;
}
