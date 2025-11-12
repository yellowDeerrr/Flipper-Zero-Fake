// #include <Wire.h>
// #include <LiquidCrystal_I2C.h>
// #include <WiFiNetwork.h>
// #include <vector>
// #include <Menu.h>

// uint8_t arrow[8] = {0x00, 0x04 ,0x06, 0x1f, 0x06, 0x04, 0x00}; //Send 0,4,6,1F,6,4,0 for the arrow

// LiquidCrystal_I2C lcd(0x27, 20, 4);

// void initDisplay(){
    // Wire.begin(41, 42);

//     lcd.init();           // Initialize LCD
//     lcd.backlight();      // Turn on backlight

    // lcd.createChar(0, arrow);
// }

// bool displayingNetworks = false;

// void displayNetworks(std::vector<WiFiNetwork>& nets){
    
//     lcd.clear();

//     if (nets.empty()) {
//         lcd.setCursor(0, 0);
//         lcd.print("No networks found");
//         return;
//     }

//     lcd.setCursor(0, 0);
//     lcd.printf("Found: %d", nets.size());

//     for(int i = 0; i < 3; i++){
//         lcd.setCursor(1, i + 1);
//         lcd.printf("%s", nets[i].ssid);
//     }    
//     displayingNetworks = true;
// }

// void displayScanLoading(){
//     lcd.clear();
//     lcd.setCursor(1, 0);
//     lcd.print("Scanning...");
// }

// void displayCounter(int counter){
//     if(displayingNetworks) return;

//     lcd.setCursor(0, 1);
//     lcd.print("Count: ");
//     lcd.print(counter);
//     lcd.print("   ");
// }


// // void drawMainMenu(){
// //     lcd.clear();
// //     for(int i = 0; i < 4; i++){
// //         lcd.setCursor(1, i);
// //         lcd.printf(menuItems[currentScreen][i].c_str());
// //     }

// //     lcd.setCursor(0, currentPointedMenuItem);
// //     lcd.write(0);
// // }

// // void scrollUp(){
// //     currentPointedMenuItem--;
// //     // Serial.println(currentPointedMenuItem);

// //     if(currentPointedMenuItem < 0) currentPointedMenuItem = indexOfLastItem;
// //     drawMainMenu();
// // }

// // void scrollDown(){
// //     currentPointedMenuItem++;
// //     // Serial.println(currentPointedMenuItem);
// //     if(currentPointedMenuItem > indexOfLastItem) currentPointedMenuItem = 0;
// //     drawMainMenu();
// // }



#include "LCD.h"


LCDDisplay::LCDDisplay(uint8_t address, uint8_t cols, uint8_t rows) 
  : lcd(address, cols, rows), columns(cols), rows(rows) {
}

void LCDDisplay::init() {
    Wire.begin(41, 42);
    lcd.init();
    lcd.createChar(0, arrow);
    lcd.backlight();
    lcd.clear();
}

void LCDDisplay::clear() {
    lcd.clear();
}

void LCDDisplay::print(String text, int row, int col) {
    lcd.setCursor(col, row);
    lcd.print(text);
}

void LCDDisplay::printCentered(String text, int row) {
    int padding = (columns - text.length()) / 2;
    if (padding < 0) padding = 0;
    lcd.setCursor(padding, row);
    lcd.print(text);
}

void LCDDisplay::printScrolling(String text, int row, int scrollDelay) {
  // If text fits on screen, just print it normally
    if (text.length() <= columns) {
        lcd.setCursor(0, row);
        lcd.print(text);
        return;
    }
  
  // Add spaces for smooth scrolling loop
    String scrollText = text + "   ";
    int textLength = scrollText.length();

    // Calculate how many scroll positions we need
    int scrollPositions = textLength - columns + 1;

    // Scroll the text
    for (int i = 0; i < scrollPositions; i++) {
        lcd.setCursor(0, row);

        // Print substring that fits on display
        String displayPart = scrollText.substring(i, i + columns);
        lcd.print(displayPart);

        delay(scrollDelay);
    }

    // Pause at the end
    delay(scrollDelay * 2);
}

void LCDDisplay::displayMenu(String* menuItems, int menuSize, int currentIndex) {
    lcd.clear();

    // Calculate which items to show (centered around current selection)
    int startIndex = currentIndex - 1;
    if (startIndex < 0) startIndex = 0;

    // Make sure we show 'rows' items if possible
    if (startIndex + rows > menuSize) {
        startIndex = menuSize - rows;
        if (startIndex < 0) startIndex = 0;
    }

    // Display up to 'rows' items
    for (int i = 0; i < rows && (startIndex + i) < menuSize; i++) {
        lcd.setCursor(0, i);

        // Show cursor on selected item
        if (startIndex + i == currentIndex) {
            lcd.write(0);
            lcd.print(" ");
        } else {
            lcd.print("  ");
        }

        lcd.print(menuItems[startIndex + i]);
    }
}

void LCDDisplay::displayInfo(String* labels, String* values, int itemsCount, int currentIndex) {
    lcd.clear();

    // Calculate which items to show
    int startIndex = currentIndex;
    if (startIndex + rows > itemsCount) {
        startIndex = itemsCount - rows;
        if (startIndex < 0) startIndex = 0;
    }

    // Display up to 'rows' items (label: value pairs)
    for (int i = 0; i < rows && (startIndex + i) < itemsCount; i++) {
        lcd.setCursor(0, i);

        // Show cursor on selected item
        if (startIndex + i == currentIndex) {
            lcd.write(0);
        } else {
            lcd.print(" ");
        }

        // Print label
        lcd.print(labels[startIndex + i]);
        lcd.print(": ");

        // Print value (truncate if too long)
        String value = values[startIndex + i];
        int maxValueLen = columns - labels[startIndex + i].length() - 3; // 3 for ">", ":", " "

        if (value.length() > maxValueLen) {
            lcd.print(value.substring(0, maxValueLen));
        } else {
            lcd.print(value);
        }
    }
}