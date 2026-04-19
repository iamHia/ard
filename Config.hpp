#pragma once
#include <Arduino.h>

namespace Config {
  const int laserPin = 8;
  const int ldrPin = A0; 
  const int threshold = 800; 
  
  const unsigned long dotMs = 200;
  const unsigned long dashMs = dotMs * 3;
  const unsigned long elemGap = dotMs;
  const unsigned long letterGap = dotMs * 3;
  const unsigned long wordGap = dotMs * 7;
}