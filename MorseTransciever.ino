#include <Arduino.h>
#include "Config.h"
#include "Laser.h"
#include "Transmitter.h"
#include "LightSensor.h"
#include "Receiver.h"
#include "Display.h" 

// --- HARDWARE OBJECTS ---
Laser laser(Config::laserPin);         
Transmitter tx(laser);        
LightSensor ldr(Config::ldrPin, Config::threshold); 
Receiver rx(ldr);                             

// --- STATE VARIABLES ---
char msgBuf[15] = {0};                              
int msgLen = 0;                                     
bool rxHalted = false; 

void setup() {
  Serial.begin(9600);
  
  tx.begin();
  ldr.begin();
  
  // The 'display' object is safely pulled from Display.h
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED failed to boot! Check wiring."));
    for(;;); 
  }
  
  Serial.println(F("\n=== SYSTEM ONLINE ==="));
  Serial.println(F("Type a message to send (Must end with a '.')"));
  
  // The updateScreen function is safely pulled from Display.h
  updateScreen("STATION ONLINE"); 
}

void loop() {
  unsigned long now = millis(); 

  // --- TRANSMITTER ---
  if (Serial.available() > 0) {
    String input = Serial.readString();
    input.trim(); 
    
    if (input.length() > 0) {
      if (!input.endsWith(".")) {
        Serial.println(F(">>> ERROR: Message must end with a '.' <<<"));
      } else {
        Serial.print(F("Transmitting: "));
        Serial.println(input);
        tx.send(input.c_str()); 
      }
    }
  }
  tx.update(now); 

  // --- RECEIVER ---
  if (!rxHalted) { 
    char c = rx.update(now); 
    
    if (c != '\0') {
      Serial.print(F(" ---> Received: ["));
      Serial.print(c);
      Serial.println(F("]"));
      
      if (c == '.') {
        Serial.println(F("\n[!] End signal received. Incoming comms halted."));
        rxHalted = true; 
      } else {
        // Add character to the screen buffer
        if (msgLen < 12) {
          msgBuf[msgLen++] = c;
          msgBuf[msgLen] = '\0';
        } else {
          msgLen = 1;
          msgBuf[0] = c;
          msgBuf[1] = '\0';
        }
        updateScreen(msgBuf); 
      }
    }
  }
}