/**
 * @file MorseDecoder.ino
 * @brief Main execution file for the Receiver Arduino.
 */
#include <Arduino.h>
#include "Config.h"
#include "LightSensor.h"
#include "Display.h"
#include "Receiver.h"

LightSensor ldr(Config::ldrPin, Config::threshold); // Create Sensor
Display oled;                                       // Create OLED
Receiver receiver(ldr);                             // Create Receiver FSM

char msgBuf[15] = {0};                              // Raw array for OLED text
int msgLen = 0;                                     // Length of OLED text
bool halted = false;                                // Kill Switch flag

void setup() {
  Serial.begin(9600);
  ldr.begin();
  
  // Halt completely if OLED wiring is wrong
  if(!oled.begin()) {
    Serial.println(F("OLED failed to boot!"));
    for(;;); 
  }
  
  Serial.println(F("\n=== DECODER ONLINE ==="));
  oled.printMsg(""); // Clear the screen
}

void loop() {
  // If the kill switch is flipped, skip everything forever
  if (halted) return; 

  unsigned long now = millis();
  char ch = receiver.update(now); // Run receiver FSM
  
  // If the FSM handed us a completed letter...
  if (ch != '\0') {
    Serial.print(F(" ---> ["));
    Serial.print(ch);
    Serial.println(F("]"));
    
    // Check if it's the "End of Transmission" period
    if (ch == '.') {
      Serial.println(F("\n[!] End signal received. System Halted."));
      halted = true; // Flip the kill switch
      return; 
    }
    
    // Add character to the screen buffer
    if (msgLen < 12) {
      msgBuf[msgLen++] = ch;
      msgBuf[msgLen] = '\0';
    } else {
      // If screen is full, wipe it and start fresh with new character
      msgLen = 1;
      msgBuf[0] = ch;
      msgBuf[1] = '\0';
    }
    
    // Update the physical OLED
    oled.printMsg(msgBuf); 
  }
}