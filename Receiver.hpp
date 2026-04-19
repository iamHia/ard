#pragma once
#include <Arduino.h>
#include "Config.h"
#include "MorseDictionary.h"
#include "LightSensor.h"
#include "myString.h"

class Receiver {
  LightSensor& ldr; 
  unsigned long blockStart = 0;
  unsigned long gapStart = 0;
  bool blocked = false;
  myString morseBuf; 

public:
  Receiver(LightSensor& sensor) : ldr(sensor) {}
  
  char update(unsigned long now) {
    bool isDark = ldr.isBlocked(); 

    if (isDark && !blocked) { 
      blocked = true; 
      blockStart = now; 
    } 
    else if (!isDark && blocked) { 
      blocked = false; 
      gapStart = now;
      unsigned long blockDur = now - blockStart;
      
      if (blockDur > 20 && morseBuf.length() < 9) {
        if (blockDur < (Config::dotMs * 2)) {
          morseBuf.append('.'); 
          Serial.print(F("."));
        } else {
          morseBuf.append('-'); 
          Serial.print(F("-"));
        }
      }
    }

    if (!isDark && !blocked && morseBuf.length() > 0) {
      unsigned long gapDur = now - gapStart;
      if (gapDur > (Config::letterGap - 50)) {
        char ch = decodeMorse(morseBuf.c_str()); 
        morseBuf.clear(); 
        return ch;
      }
    }
    return '\0'; 
  }
};