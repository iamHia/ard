/**
 * @file MorseEncoder.ino
 * @brief Main execution file for the Transmitter Arduino.
 */
#include <Arduino.h>
#include "Config.h"
#include "Laser.h"
#include "Transmitter.h"

Laser laser(Config::laserPin);         // Create the laser hardware object
Transmitter transmitter(laser);        // Create the transmitter, hand it the laser

void setup() {
  Serial.begin(9600);                  // Open PC communication
  transmitter.begin();                 // Setup laser pins
  Serial.println(F("=== ENCODER ONLINE ==="));
  Serial.println(F("Type a message. IT MUST END WITH A '.'"));
}

void loop() {
  unsigned long now = millis();        // Get the current time
  
  // Check if the user typed something in the Serial Monitor
  if (Serial.available() > 0) {
    String input = Serial.readString();
    input.trim();                      // Remove invisible newline/enter characters
    
    if (input.length() > 0) {
      // Enforce the "Kill Switch" period rule!
      if (!input.endsWith(".")) {
        Serial.println(F(">>> ERROR: Message must end with a '.' <<<"));
      } else {
        Serial.print(F("Transmitting: "));
        Serial.println(input);
        transmitter.send(input.c_str()); // Send the safe, raw char array to transmitter
      }
    }
  }
  
  // Run the Finite State Machine (happens thousands of times a second)
  transmitter.update(now);
}