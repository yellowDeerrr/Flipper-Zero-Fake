#pragma once

void initEncoder();
void handleEncoderInterrupt();
bool isEncoderButtonPressed();
void printOutEncoderData(unsigned long &now);
void encoderCheckAndHandle(unsigned long &now);