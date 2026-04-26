#include <Arduino.h>
#include "Config.hpp"
#include "Laser.hpp"
#include "Transmitter.hpp"
#include "LightSensor.hpp"
#include "Receiver.hpp"
#include "Display.hpp"

Laser laser(Config::laserPin);
Transmitter tx(laser);
LightSensor ldr(Config::ldrPin, Config::threshold);
Receiver rx(ldr);

char prevChar = '\0';
char activeChar = '\0';
char msgBuf[22] = {0};
int msgLen = 0;

void setup() {
  Serial.begin(9600);

  tx.begin();
  ldr.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED failed to boot! Check wiring."));
    for (;;);
  }

  Serial.println(F("\n=== SYSTEM ONLINE ==="));
  Serial.println(F("Type a message to send..."));

  updateScreen(' ', ' ', "HELLO ");
}

void loop() {
  unsigned long now = millis();

  if (Serial.available() > 0) {
    String input = Serial.readString();
    input.trim();

    if (input.length() > 0) {
      Serial.print(F("Transmitting: "));
      Serial.println(input);
      tx.send(input.c_str());
    }
  }

  tx.update(now);

  char c = rx.update(now);

  if (c != '\0') {
    Serial.print(F(" ---> Received: ["));
    Serial.print(c);
    Serial.println(F("]"));

    prevChar = activeChar; 
    activeChar = c;        

    if (msgLen >= 10) {
      for (int i = 0; i < 9; i++) {
        msgBuf[i] = msgBuf[i+1];
      }
      msgBuf[9] = c;
    } else {
      msgBuf[msgLen++] = c;
      msgBuf[msgLen] = '\0';
    }
    
    updateScreen(prevChar, activeChar, msgBuf);
  }
}