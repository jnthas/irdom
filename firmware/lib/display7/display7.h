#pragma once

#include <Arduino.h>


#define CHAR_ARR_SIZE (28)
#define DISPLAY_FRAMERATE (15)

#define DISP_LATCH_PIN (12)
#define DISP_DATA_PIN  (13)
#define DISP_CLOCK_PIN (14)

#define DISP_DIG1_PIN (15)
#define DISP_DIG2_PIN (9)

namespace Display7 {

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
    {0b00001000,'_'},
    {0b10000000,'.'}
};


      
  
  void setup();
  void loop();
  
  
  //void print(uint8_t indexChar1, uint8_t indexChar2); //print('I', 'n')
  void print(const char* content);  //print("In")
  void print(const char* content, uint8_t size, uint16_t delay);  //print("Input", 5, 200)
  
  void progressing(uint16_t  delay);
  void bufferControl();
  void clear();

  void renderContent(uint8_t char1, uint8_t char2);
  uint8_t getCharCode(const char digit);

};
