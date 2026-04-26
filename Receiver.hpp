#pragma once
#include <Arduino.h>
#include "Config.hpp"
#include "MorseDictionary.hpp"
#include "LightSensor.hpp"
#include "myString.hpp"

class Receiver {
  LightSensor& ldr;
  unsigned long pulseStart = 0;
  unsigned long gapStart = 0;
  bool receivingPulse = false;
  myString morseBuf;

public:
  Receiver(LightSensor& sensor) : ldr(sensor) {}

  char update(unsigned long now) {
    bool laserActive = ldr.isLaserDetected();

    if (laserActive && !receivingPulse) {
      receivingPulse = true;
      pulseStart = now;
    } else if (!laserActive && receivingPulse) {
      receivingPulse = false;
      gapStart = now;

      unsigned long pulseDur = now - pulseStart;

      if (pulseDur > 20 && morseBuf.length() < 9) {
        if (pulseDur < (Config::dotMs * 2)) {
          morseBuf.append('.');
          Serial.print(F("."));
        } else {
          morseBuf.append('-');
          Serial.print(F("-"));
        }
      }
    }

    if (!laserActive && !receivingPulse && morseBuf.length() > 0) {
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