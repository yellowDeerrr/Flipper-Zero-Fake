#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFiNetwork.h>
#include <vector>
#include <Menu.h>

uint8_t arrow[8] = {0x00, 0x04 ,0x06, 0x1f, 0x06, 0x04, 0x00}; //Send 0,4,6,1F,6,4,0 for the arrow

LiquidCrystal_I2C lcd(0x27, 20, 4);


// temp
const int maxItemTitleLength = 11;
const int itemsNum = 4;
const int indexOfLastItem = itemsNum - 1;
String menuItems[][maxItemTitleLength] = {"Network", "Infrared", "RFID", "Bluetooth"};

int currentScreen = 0; // 0 - Main menu, 1 - Sub Menu ...
int currentPointedMenuItem = 0;

// ---- 

void initDisplay(){
    Wire.begin(41, 42);

    lcd.init();           // Initialize LCD
    lcd.backlight();      // Turn on backlight

    lcd.createChar(0, arrow);

    lcd.setCursor(0, 0);
    lcd.write(0);

    lcd.setCursor(1, 0);
    lcd.print("Scan Networks");
    lcd.setCursor(1, 2);
}

bool displayingNetworks = false;

void displayNetworks(std::vector<WiFiNetwork>& nets){
    lcd.clear();

    if (nets.empty()) {
        lcd.setCursor(0, 0);
        lcd.print("No networks found");
        return;
    }

    lcd.setCursor(0, 0);
    lcd.printf("Found: %d", nets.size());

    for(int i = 0; i < 3; i++){
        lcd.setCursor(1, i + 1);
        lcd.printf("%s", nets[i].ssid);
    }    
    displayingNetworks = true;
}

void displayScanLoading(){
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Scanning...");
}

void displayCounter(int counter){
    if(displayingNetworks) return;

    lcd.setCursor(0, 1);
    lcd.print("Count: ");
    lcd.print(counter);
    lcd.print("   ");
}


void drawMainMenu(){
    lcd.clear();
    for(int i = 0; i < 4; i++){
        lcd.setCursor(1, i);
        lcd.printf(menuItems[currentScreen][i].c_str());
    }

    lcd.setCursor(0, currentPointedMenuItem);
    lcd.write(0);
}

void scrollUp(){
    currentPointedMenuItem--;
    // Serial.println(currentPointedMenuItem);

    if(currentPointedMenuItem < 0) currentPointedMenuItem = indexOfLastItem;
    drawMainMenu();
}
void scrollDown(){
    currentPointedMenuItem++;
    // Serial.println(currentPointedMenuItem);
    if(currentPointedMenuItem > indexOfLastItem) currentPointedMenuItem = 0;
    drawMainMenu();
}