#pragma once
#include <Arduino.h>

class Laser {
  int pin;
public:
  Laser(int p) : pin(p) {}
  void begin() { pinMode(pin, OUTPUT); turnOff(); }
  void turnOn() { digitalWrite(pin, LOW); }
  void turnOff() { digitalWrite(pin, HIGH); }
};