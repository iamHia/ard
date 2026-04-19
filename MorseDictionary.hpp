#pragma once
#include <Arduino.h>

const char* const morseCodes[37] = {
  ".-","-...","-.-.","-..",".","..-.","--.","....","..",".---","-.-",".-..","--","-.","---",".--.","--.-",".-.","...","-","..-","...-",".--","-..-","-.--","--..", 
  "-----",".----","..---","...--","....-",".....","-....","--...","---..","----.",
  ".-.-.-" 
};

const char* getMorse(char ch) {
  if (ch >= 'A' && ch <= 'Z') return morseCodes[ch - 'A'];
  if (ch >= '0' && ch <= '9') return morseCodes[ch - '0' + 26];
  if (ch == '.') return morseCodes[36]; 
  return "";
}

char decodeMorse(const char* morse) {
  for (int i = 0; i < 37; i++) {
    if (strcmp(morse, morseCodes[i]) == 0) {
      if (i < 26) return 'A' + i;
      if (i < 36) return '0' + (i - 26);
      return '.'; 
    }
  }
  return '?';
}