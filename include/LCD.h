// #pragma once
// #include <vector>
// #include "WiFiNetwork.h"

// void initDisplay();
// void displayNetworks(std::vector<WiFiNetwork>& nets);
// void displayCounter(int counter);
// // void drawMainMenu();
// // void scrollUp();
// // void scrollDown();
// void displayScanLoading();



#ifndef LCD_H
#define LCD_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

class LCDDisplay {
public:
    LCDDisplay(uint8_t address, uint8_t cols, uint8_t rows);
    void init();
    void clear();
    void displayMenu(String* menuItems, int menuSize, int currentIndex);
    void displayInfo(String* labels, String* values, int itemsCount, int currentIndex);
    void print(String text, int row = 0, int col = 0);
    void printCentered(String text, int row);
    void printScrolling(String text, int row, int scrollDelay = 300);
  
private:
    LiquidCrystal_I2C lcd;
    uint8_t columns;
    uint8_t rows;
    uint8_t arrow[8] = {0x00, 0x04 ,0x06, 0x1f, 0x06, 0x04, 0x00}; //Send 0,4,6,1F,6,4,0 for the arrow
};

#endif