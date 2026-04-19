#pragma once
#include <Arduino.h>

class LightSensor {
  int pin;
  int threshold;
public:
  LightSensor(int p, int thresh) : pin(p), threshold(thresh) {}
  void begin() { pinMode(pin, INPUT); }
  
  bool isBlocked() { return analogRead(pin) < threshold; }
};