// #include <Arduino.h>
// #include <vector> 

// #include "WiFiNetwork.h"
// #include "LCD.h"
// #include "Encoder.h"


// void setup(){
//     Serial.begin(115200);
//     delay(100);
   
//     initDisplay();
//     delay(100);

//     // initNetwork(1);
//     // delay(100);
//     // initServer();
    
//     delay(1000);
//     initEncoder();
//     delay(1000);
// }

// long unsigned now, lastDisplayPrint = 0;

// void loop(){
//     now = millis();
//     handleClient();

//     encoderCheckAndHandle(now);
//     printUsers();
//     // printOutEncoderData(now);
// }

// // #include <WiFi.h>
// // #include <WebServer.h>

// // void handle_OnConnect();
// // void handle_NotFound();

// // /* Put your SSID & Password */
// // const char* ssid = "ESP32";         // Enter SSID here
// // const char* password = "12345678";  // Enter Password here

// // /* Put IP Address details */
// // IPAddress local_ip(192,168,1,1);
// // IPAddress gateway(192,168,1,1);
// // IPAddress subnet(255,255,255,0);

// // WebServer server(80);

// // // int counter = 0;


// // void setup() {
// //   Serial.begin(115200);

// //   WiFi.softAP(ssid, password);
// //   WiFi.softAPConfig(local_ip, gateway, subnet);
// //   delay(100);
  
// //   server.on("/", handle_OnConnect);
// //   server.onNotFound(handle_NotFound);
  
// //   server.begin();
// //   Serial.println("HTTP server started");
// // }

// // void loop() {
// //   server.handleClient();
// // }

// // void handle_OnConnect() {
// // //   counter++;
// //   server.send(200, "text/html", "connected");
// // }

// // void handle_NotFound() {
// //   server.send(404, "text/plain", "Not found");
// // }

#include <Arduino.h>

#include "MenuSystem.h"
#include "Encoder.h"
#include "LCD.h"
#include "WiFiManager.h"

#define ENCODER_CLK 38
#define ENCODER_DT 37
#define ENCODER_SW 39
#define LCD_ADDRESS 0x27

LCDDisplay display(LCD_ADDRESS, 20, 4);
Encoder encoder(ENCODER_CLK, ENCODER_DT, ENCODER_SW);
MenuSystem menu;
WiFiManager wifi;

void setup() {
  Serial.begin(115200);
  
  display.init();
  encoder.init();
  menu.init();
  
  encoder.setBounds(0, menu.getCurrentSize() - 1);
  wifi.setMode(WIFI_MODE_STA);
  
  display.displayMenu(menu.getCurrentMenuItems(), 
                     menu.getCurrentSize(), 
                     menu.getCurrentIndex());
}

void loop() {
  // Check for button press
  if (encoder.buttonPressed()) {
    menu.handleSelection();
    
    // Update encoder bounds for new menu
    encoder.setPosition(menu.getCurrentIndex());
    
    // Set bounds based on menu level
    if (menu.getCurrentLevel() == INFO_VIEW) {
      // In info view, we scroll through info items not menu items
      encoder.setBounds(0, 10 - 1); // Adjust based on actual info count
    } else {
      encoder.setBounds(0, menu.getCurrentSize() - 1);
    }
    
    // Update display
    if (menu.getCurrentLevel() == INFO_VIEW) {
      // Display info view - we'll handle this in MenuSystem
      // For now, just trigger a refresh
    }
    
    display.displayMenu(menu.getCurrentMenuItems(), 
                       menu.getCurrentSize(), 
                       menu.getCurrentIndex());
  }
  
  // Check for encoder rotation
  static int lastPos = -1;
  int currentPos = encoder.getPosition();
  
  if (currentPos != lastPos) {
    lastPos = currentPos;
    menu.setCurrentIndex(currentPos);
    
    // Update display based on current level
    if (menu.getCurrentLevel() == INFO_VIEW) {
      // Display scrollable info view
      display.displsayInfo(menu.getInfoLabels(), 
                         menu.getInfoValues(), 
                         menu.getInfoItemsCount(), 
                         menu.getCurrentIndex());
    } else {
      // Display normal menu
      display.displayMenu(menu.getCurrentMenuItems(), 
                         menu.getCurrentSize(), 
                         menu.getCurrentIndex());
    }
  }
}