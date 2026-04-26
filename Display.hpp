#pragma once
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "MorseDictionary.hpp" 

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void updateScreen(char prevChar, char activeChar, const char* gatheredText) {
  display.clearDisplay();
  

  display.setTextSize(1); 
  

  if (prevChar != ' ' && prevChar != '\0') {

    display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
    display.setCursor(2, 0);               
    display.print(getMorse(prevChar)); 
    
    display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
    display.setCursor(116, 0);             
    display.print(prevChar);
  }

  if (activeChar != ' ' && activeChar != '\0') {

    display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
    display.setCursor(2, 8);               
    display.print(getMorse(activeChar));
    
    display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
    display.setCursor(116, 8);             
    display.print(activeChar);
  }


  display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
  display.setTextSize(1); 
  display.setCursor(0, 16); 
  display.print("_____________________");
  
  display.setTextSize(1); 
  display.setCursor(0, 24); 
  display.print(gatheredText);

  display.display(); 
}