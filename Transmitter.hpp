#pragma once
#include <Arduino.h>
#include "Config.hpp"
#include "MorseDictionary.hpp"
#include "Laser.hpp"
#include "myString.hpp"

class Transmitter {
  Laser& laser; 
  int state = 0;
  int seqIdx = 0;
  unsigned long timer = 0;
  myString buffer; 
  const char* seq = "";

public:
  Transmitter(Laser& l) : laser(l) {}
  
  void begin() { laser.begin(); }
  
  void send(const char* msg) {
    for(int i = 0; msg[i] != '\0'; i++) {
      char ch = msg[i];
      if (isalnum(ch) || ch == ' ' || ch == '.') {
        buffer.append(toupper(ch)); 
      }
    }
  }
  
  void update(unsigned long now) {
    if (now < timer) return; 

    if (state == 0 && buffer.length() > 0) { 
      char ch = buffer.popFront(); 
      if (ch == ' ') { 
          timer = now + Config::wordGap; 
          return; 
      }
      seq = getMorse(ch); 
      if (seq[0] == '\0') return; 
      seqIdx = 0; 
      state = 1;
    } 
    else if (state == 1) { 
      laser.turnOn(); 
      unsigned long activeMs = (seq[seqIdx] == '.') ? Config::dotMs : Config::dashMs;
      timer = now + activeMs;
      state = 2;
    } 
    else if (state == 2) { 
      laser.turnOff(); 
      seqIdx++;
      bool isDone = (seq[seqIdx] == '\0');
      unsigned long pauseMs = isDone ? Config::letterGap : Config::elemGap;
      timer = now + pauseMs;
      state = isDone ? 0 : 1; 
    }
  }
};