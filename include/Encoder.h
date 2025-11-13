// #pragma once

// void initEncoder();
// bool isEncoderButtonPressed();
// void printOutEncoderData(unsigned long &now);
// void encoderCheckAndHandle(unsigned long &now);
#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>

class Encoder {
public:
  Encoder(int clkPin, int dtPin, int swPin);
  void init();
  void update();
  bool buttonPressed();
  bool isButtonHeld();
  int getPosition() { return position; }
  void setPosition(int pos) { position = pos; }
  void setBounds(int min, int max);
  
private:
  int pinCLK;
  int pinDT;
  int pinSW;
  
  int position;
  int lastCLK;
  int minBound;
  int maxBound;
  
  bool btnPressed;
  unsigned long lastDebounceTime;
  
  static void readEncoderISR();
  static Encoder* instance;
  void readEncoder();
};

#endif