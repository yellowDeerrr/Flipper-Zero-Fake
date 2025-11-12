#ifndef MENUSYSTEM_H
#define MENUSYSTEM_H

#include <Arduino.h>

enum MenuLevel { MAIN_MENU, SUB_MENU, ITEM_LIST, ITEM_OPTIONS, INFO_VIEW };

class MenuSystem {
public:
  MenuSystem();
  void init();
  void setMenu(String* menuItems, int menuSize);
  void handleSelection();
  void navigateBack();
  
  // Menu navigation
  void enterSubMenu(int mainIndex);
  void handleSubMenuSelection(int mainIndex, int subIndex);
  void handleItemOption(int optionIndex);
  
  // Getters
  MenuLevel getCurrentLevel() { return currentLevel; }
  int getCurrentIndex() { return currentIndex; }
  int getCurrentSize() { return currentSize; }
  String* getCurrentMenuItems() { return currentMenuItems; }
  String* getInfoLabels() { return infoLabels; }
  String* getInfoValues() { return infoValues; }
  int getInfoItemsCount() { return infoItemsCount; }
  
  // Setters
  void setCurrentIndex(int index) { currentIndex = index; }
  
private:
  MenuLevel currentLevel;
  int currentIndex;
  int currentSize;
  String* currentMenuItems;

    String infoLabels[10];
    String infoValues[10];
    int infoItemsCount;

  // Parent indices for navigation
  int parentMainIndex;
  int parentSubIndex;
  int parentItemIndex;
  
  // Menu arrays - defined in .cpp file
  String* mainMenuItems;
  int mainMenuSize;
  
  String* networksSubMenu;
  int networksSubMenuSize;
  
  String* irSubMenu;
  int irSubMenuSize;
  
  String* bluetoothSubMenu;
  int bluetoothSubMenuSize;
  
  String* rfidSubMenu;
  int rfidSubMenuSize;
  
  String scannedNetworks[10];
  int scannedNetworksCount;
  
  String* networkOptions;
  int networkOptionsSize;
  
  // Action functions
  void scanNetworks();
  void captureIR();
  void readRFID();
  void showNetworkInfo(String ssid);
  void backToMainMenu();
  void backToSubMenu();
  void backToItemList();
  void backToItemOptions();
};

#endif