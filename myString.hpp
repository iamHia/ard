#pragma once
#include <Arduino.h>

class myString {
  static const int MAX_LEN = 60; 
  char buffer[MAX_LEN];
  int len;

public:
  myString() { clear(); }

  void clear() {
    len = 0;
    buffer[0] = '\0';
  }

  bool append(char ch) {
    if (len < MAX_LEN - 1) { 
      buffer[len++] = ch;
      buffer[len] = '\0'; 
      return true;
    }
    return false; 
  }

  char popFront() {
    if (len == 0) return '\0';
    char first = buffer[0];
    for (int i = 0; i < len; i++) buffer[i] = buffer[i + 1];
    len--;
    return first;
  }

  int length() { return len; }
  const char* c_str() { return buffer; }
};