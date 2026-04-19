#pragma once
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// --- THE RAW, OG OLED SETUP ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

// We create the global display object right here in the header!
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// The exact screen function that worked perfectly
void updateScreen(const char* text) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print(F("MORSE STATION ACTIVE")); 
  display.drawLine(0, 10, 128, 10, SSD1306_WHITE);
  display.setTextSize(2); 
  display.setCursor(0, 16);
  display.print(text);
  display.display(); 
}