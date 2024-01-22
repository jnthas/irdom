#pragma once

#include <Arduino.h>


#define CHAR_ARR_SIZE 27

namespace Display7 {

  const uint8_t latchPin = 12;
  const uint8_t clockPin = 14;
  const uint8_t dataPin = 13;

  const uint8_t digit1 = 15;
  const uint8_t digit2 = 1;
 


const uint8_t DisplayChars[CHAR_ARR_SIZE][2] = {
    {0b00000000,' '},
    {0b00111111,'0'},
    {0b00000110,'1'},
    {0b01011011,'2'},
    {0b01001111,'3'},
    {0b01100110,'4'},
    {0b01101101,'5'},
    {0b01111101,'6'},
    {0b00000111,'7'},
    {0b01111111,'8'},
    {0b01101111,'9'},
    
    {0b01110111,'A'},
    {0b01111100,'B'},
    {0b00111001,'C'},
    {0b01011110,'D'},
    {0b01111001,'E'},
    {0b01110001,'F'},

    {0b01110011,'P'},
    {0b00010000,'i'},
    {0b01010100,'n'},
    {0b01011000,'c'},
    {0b01110100,'h'},
    
    {0b00011100,'u'},
    {0b01111000,'t'},
    
    {0b01000000,'-'},
    {0b00000001,'^'},
    {0b00001000,'_'}
};


      
  
  void setup();
  void loop();
  void writeDisplay(uint8_t a, uint8_t b);
  void refreshDisplay();
  uint8_t find_char(char digit);

};
