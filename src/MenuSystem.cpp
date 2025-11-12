#include "MenuSystem.h"
#include "LCD.h"

extern LCDDisplay display;

// Define all menu arrays in .cpp file with automatic size calculation
static String mainMenuItemsArray[] = {"Networks", "IR", "Bluetooth", "RFID"};
static const int mainMenuSizeValue = sizeof(mainMenuItemsArray) / sizeof(mainMenuItemsArray[0]);

static String networksSubMenuArray[] = {"Scan Networks", "Connect", "Last Scan"};
static const int networksSubMenuSizeValue = sizeof(networksSubMenuArray) / sizeof(networksSubMenuArray[0]);

static String irSubMenuArray[] = {"Capture", "Simulate"};
static const int irSubMenuSizeValue = sizeof(irSubMenuArray) / sizeof(irSubMenuArray[0]);

static String bluetoothSubMenuArray[] = {"Scan Devices", "Pair"};
static const int bluetoothSubMenuSizeValue = sizeof(bluetoothSubMenuArray) / sizeof(bluetoothSubMenuArray[0]);

static String rfidSubMenuArray[] = {"Read Tag"};
static const int rfidSubMenuSizeValue = sizeof(rfidSubMenuArray) / sizeof(rfidSubMenuArray[0]);

static String networkOptionsArray[] = {"Connect", "Deauth", "Info", "Back"};
static const int networkOptionsSizeValue = sizeof(networkOptionsArray) / sizeof(networkOptionsArray[0]);

MenuSystem::MenuSystem() {
  currentLevel = MAIN_MENU;
  currentIndex = 0;
  currentSize = 0;
  currentMenuItems = nullptr;
//   infoLabels = nullptr;
//   infoValues = nullptr;
  infoItemsCount = 0;
  parentMainIndex = 0;
  parentSubIndex = 0;
  parentItemIndex = 0;
  scannedNetworksCount = 0;
}

void MenuSystem::init() {
  // Assign pointers to static arrays
  mainMenuItems = mainMenuItemsArray;
  mainMenuSize = mainMenuSizeValue;
  
  networksSubMenu = networksSubMenuArray;
  networksSubMenuSize = networksSubMenuSizeValue;
  
  irSubMenu = irSubMenuArray;
  irSubMenuSize = irSubMenuSizeValue;
  
  bluetoothSubMenu = bluetoothSubMenuArray;
  bluetoothSubMenuSize = bluetoothSubMenuSizeValue;
  
  rfidSubMenu = rfidSubMenuArray;
  rfidSubMenuSize = rfidSubMenuSizeValue;
  
  networkOptions = networkOptionsArray;
  networkOptionsSize = networkOptionsSizeValue;
  
  // Set initial menu
  setMenu(mainMenuItems, mainMenuSize);
}

void MenuSystem::setMenu(String* menuItems, int menuSize) {
  currentMenuItems = menuItems;
  currentSize = menuSize;
  currentIndex = 0;
}

void MenuSystem::handleSelection() {
  switch (currentLevel) {
    case MAIN_MENU:
      parentMainIndex = currentIndex;
      enterSubMenu(currentIndex);
      break;
      
    case SUB_MENU:
      parentSubIndex = currentIndex;
      handleSubMenuSelection(parentMainIndex, currentIndex);
      break;
      
    case ITEM_LIST:
      parentItemIndex = currentIndex;
      currentLevel = ITEM_OPTIONS;
      setMenu(networkOptions, networkOptionsSize);
      break;
      
    case ITEM_OPTIONS:
      handleItemOption(currentIndex);
      break;
      
    case INFO_VIEW:
      // Press button to go back from info view
      backToItemOptions();
      break;
  }
}

void MenuSystem::enterSubMenu(int mainIndex) {
  currentLevel = SUB_MENU;
  
  if (mainIndex == 0) {
    setMenu(networksSubMenu, networksSubMenuSize);
  } else if (mainIndex == 1) {
    setMenu(irSubMenu, irSubMenuSize);
  } else if (mainIndex == 2) {
    setMenu(bluetoothSubMenu, bluetoothSubMenuSize);
  } else if (mainIndex == 3) {
    setMenu(rfidSubMenu, rfidSubMenuSize);
  }
}

void MenuSystem::handleSubMenuSelection(int mainIndex, int subIndex) {
  if (mainIndex == 0) { // Networks
    if (subIndex == 0) {
      scanNetworks();
    } else if (subIndex == 1) {
      display.clear();
      display.printCentered("Connect to...", 1);
      delay(1500);
      backToSubMenu();
    } else if (subIndex == 2) {
      if (scannedNetworksCount > 0) {
        currentLevel = ITEM_LIST;
        setMenu(scannedNetworks, scannedNetworksCount);
      } else {
        display.clear();
        display.printCentered("No scan data", 1);
        delay(1500);
        backToSubMenu();
      }
    }
  } else if (mainIndex == 1) { // IR
    if (subIndex == 0) {
      captureIR();
    } else if (subIndex == 1) {
      display.clear();
      display.printCentered("IR Simulate", 1);
      delay(1500);
      backToSubMenu();
    }
  } else if (mainIndex == 2) { // Bluetooth
    if (subIndex == 0) {
      display.clear();
      display.printCentered("BT Scanning...", 1);
      delay(1500);
      backToSubMenu();
    } else if (subIndex == 1) {
      display.clear();
      display.printCentered("BT Pairing...", 1);
      delay(1500);
      backToSubMenu();
    }
  } else if (mainIndex == 3) { // RFID
    if (subIndex == 0) {
      readRFID();
    }
  }
}

void MenuSystem::scanNetworks() {
  display.clear();
  display.printCentered("Scanning...", 1);
  delay(1000);
  
  // Simulate network scanning
  scannedNetworksCount = 5;
  scannedNetworks[0] = "WiFi_Home";
  scannedNetworks[1] = "Office_Net";
  scannedNetworks[2] = "Guest_WiFi";
  scannedNetworks[3] = "Neighbor_5G";
  scannedNetworks[4] = "Public_AP";
  
  display.clear();
  display.print("Found: " + String(scannedNetworksCount), 1, 0);
  delay(1500);
  
  currentLevel = ITEM_LIST;
  setMenu(scannedNetworks, scannedNetworksCount);
}

void MenuSystem::handleItemOption(int optionIndex) {
  String selectedNetwork = scannedNetworks[parentItemIndex];
  
  if (optionIndex == 0) { // Connect
    display.clear();
    display.printCentered("Connecting to:", 1);
    display.printCentered(selectedNetwork, 2);
    delay(2000);
    backToSubMenu();
  } else if (optionIndex == 1) { // Deauth
    display.clear();
    display.printCentered("Deauth:", 1);
    display.printCentered(selectedNetwork, 2);
    delay(2000);
    backToSubMenu();
  } else if (optionIndex == 2) { // Info
    showNetworkInfo(selectedNetwork);
  } else if (optionIndex == 3) { // Back
    backToItemList();
  }
}

void MenuSystem::showNetworkInfo(String ssid) {
  // Simulate getting network info - replace with real WiFi data
  infoItemsCount = 0;
  
  infoLabels[infoItemsCount] = "SSID";
  infoValues[infoItemsCount] = ssid;
  infoItemsCount++;
  
  infoLabels[infoItemsCount] = "BSSID";
  infoValues[infoItemsCount] = "AA:BB:CC:DD:EE:FF";
  infoItemsCount++;
  
  infoLabels[infoItemsCount] = "Channel";
  infoValues[infoItemsCount] = "6";
  infoItemsCount++;
  
  infoLabels[infoItemsCount] = "Frequency";
  infoValues[infoItemsCount] = "2437 MHz";
  infoItemsCount++;
  
  infoLabels[infoItemsCount] = "RSSI";
  infoValues[infoItemsCount] = "-45 dBm";
  infoItemsCount++;
  
  infoLabels[infoItemsCount] = "Security";
  infoValues[infoItemsCount] = "WPA2-PSK";
  infoItemsCount++;
  
  infoLabels[infoItemsCount] = "Encryption";
  infoValues[infoItemsCount] = "AES-CCMP";
  infoItemsCount++;
  
  infoLabels[infoItemsCount] = "Signal Quality";
  infoValues[infoItemsCount] = "Excellent (95%)";
  infoItemsCount++;
  
  infoLabels[infoItemsCount] = "Hidden";
  infoValues[infoItemsCount] = "No";
  infoItemsCount++;
  
  infoLabels[infoItemsCount] = "WPS";
  infoValues[infoItemsCount] = "Enabled";
  infoItemsCount++;
  
  // Enter info view mode
  currentLevel = INFO_VIEW;
  currentIndex = 0;
}

void MenuSystem::captureIR() {
  display.clear();
  display.printCentered("IR Capture Mode", 1);
  display.printCentered("Point remote...", 2);
  delay(2000);
  backToSubMenu();
}

void MenuSystem::readRFID() {
  display.clear();
  display.printCentered("Reading RFID...", 1);
  display.printCentered("Scan tag", 2);
  delay(2000);
  backToSubMenu();
}

void MenuSystem::backToMainMenu() {
  currentLevel = MAIN_MENU;
  setMenu(mainMenuItems, mainMenuSize);
  currentIndex = parentMainIndex;
}

void MenuSystem::backToSubMenu() {
  currentLevel = SUB_MENU;
  enterSubMenu(parentMainIndex);
  currentIndex = parentSubIndex;
}

void MenuSystem::backToItemList() {
  currentLevel = ITEM_LIST;
  setMenu(scannedNetworks, scannedNetworksCount);
  currentIndex = parentItemIndex;
}

void MenuSystem::backToItemOptions() {
  currentLevel = ITEM_OPTIONS;
  setMenu(networkOptions, networkOptionsSize);
}