#include <SPI.h>
#include <avr/pgmspace.h>


// Variables are in snake_case:
//     all_variables_are_written_like_this;

// Functions are in camelCase:
//     allFunctionsAreWrittenLikeThis();

// Global constants are in UPPERCASE
//     EXAMPLE

/******************Global Constants******************/

const int SENDBMP = 0x55;  //Intelliswitch's display command
const int COLOR = 0x40;  //Intelliswitch's set color command
const int BRIGHTNESS = 0x41;  //Intelliswitch's set brightness command
const int RESET = 0x5E;  //Intelliswitch's reset command

const int RED    = 0b11000011;
const int ORANGE = 0b11010011;
const int YELLOW = 0b11110011;
const int GREEN  = 0b00110011;
const int TEAL   = 0b00111111;
const int BLUE   = 0b00001111;
const int PURPLE = 0b11001111;


/***********Pin and Interrupt Assignments************/
const int slave_select_pin = 10;
const int smart_switch_button_pin = 6;
int interrupt_pin = 0;
int interrupt_state = LOW;
int led_out = 4;

/******************Program Memory********************/

PROGMEM const unsigned char LETTERA[8] = { 
  0b00111000, 0b01000100, 0b01000100, 0b01000100, 0b01111100, 0b01000100, 0b01000100, 0b00000000 };
PROGMEM const unsigned char LETTERB[8] = { 
  0b01111000, 0b01000100, 0b01000100, 0b01111000, 0b01000100, 0b01000100, 0b01111000, 0b00000000 };
PROGMEM const unsigned char LETTERC[8] = { 
  0b00111000, 0b01000100, 0b01000000, 0b01000000, 0b01000000, 0b01000100, 0b00111000, 0b00000000 };
PROGMEM const unsigned char LETTERD[8] = { 
  0b01110000, 0b01001000, 0b01000100, 0b01000100, 0b01000100, 0b01001000, 0b01110000, 0b00000000 };
PROGMEM const unsigned char LETTERE[8] = { 
  0b01111100, 0b01000000, 0b01000000, 0b01111000, 0b01000000, 0b01000000, 0b01111100, 0b00000000 };
PROGMEM const unsigned char LETTERF[8] = { 
  0b01111100, 0b01000000, 0b01000000, 0b01111000, 0b01000000, 0b01000000, 0b01000000, 0b00000000 };
PROGMEM const unsigned char LETTERG[8] = { 
  0b00111000, 0b01000100, 0b01000000, 0b01011100, 0b01000100, 0b01000100, 0b00111100, 0b00000000 };
PROGMEM const unsigned char LETTERH[8] = { 
  0b01000100, 0b01000100, 0b01000100, 0b01111100, 0b01000100, 0b01000100, 0b01000100, 0b00000000 };
PROGMEM const unsigned char LETTERI[8] = { 
  0b00111000, 0b00010000, 0b00010000, 0b00010000, 0b00010000, 0b00010000, 0b00111000, 0b00000000 };
PROGMEM const unsigned char LETTERJ[8] = { 
  0b00111000, 0b00010000, 0b00010000, 0b00010000, 0b00010000, 0b10010000, 0b01100000, 0b00000000 };
PROGMEM const unsigned char LETTERK[8] = { 
  0b01000100, 0b01001000, 0b01010000, 0b01100000, 0b01010000, 0b01001000, 0b01000100, 0b00000000 };
PROGMEM const unsigned char LETTERL[8] = { 
  0b01000000, 0b01000000, 0b01000000, 0b01000000, 0b01000000, 0b01000000, 0b01111110, 0b00000000 };
PROGMEM const unsigned char LETTERM[8] = { 
  0b01000100, 0b01101100, 0b01010100, 0b01010100, 0b01000100, 0b01000100, 0b01000100, 0b00000000 };
PROGMEM const unsigned char LETTERN[8] = { 
  0b01000100, 0b01000100, 0b01100100, 0b01010100, 0b01001100, 0b01000100, 0b01000100, 0b00000000 };
PROGMEM const unsigned char LETTERO[8] = { 
  0b00111000, 0b01000100, 0b01000100, 0b01000100, 0b01000100, 0b01000100, 0b00111000, 0b00000000 };
PROGMEM const unsigned char LETTERP[8] = { 
  0b01111000, 0b01000100, 0b01000100, 0b01111000, 0b01000000, 0b01000000, 0b01000000, 0b00000000 };
PROGMEM const unsigned char LETTERQ[8] = { 
  0b00111000, 0b01000100, 0b01000100, 0b01000100, 0b01010100, 0b01001000, 0b00110100, 0b00000000 };
PROGMEM const unsigned char LETTERR[8] = { 
  0b01111000, 0b01000100, 0b01000100, 0b01111000, 0b01010000, 0b01001000, 0b01000100, 0b00000000 };
PROGMEM const unsigned char LETTERS[8] = { 
  0b00111100, 0b01000000, 0b01000000, 0b00111000, 0b00000100, 0b00000100, 0b01111000, 0b00000000 };
PROGMEM const unsigned char LETTERT[8] = { 
  0b01111100, 0b00010000, 0b00010000, 0b00010000, 0b00010000, 0b00010000, 0b00010000, 0b00000000 };
PROGMEM const unsigned char LETTERU[8] = { 
  0b01000100, 0b01000100, 0b01000100, 0b01000100, 0b01000100, 0b01000100, 0b00111000, 0b00000000 };
PROGMEM const unsigned char LETTERV[8] = { 
  0b01000100, 0b01000100, 0b01000100, 0b01000100, 0b01000100, 0b00101000, 0b00010000, 0b00000000 };
PROGMEM const unsigned char LETTERW[8] = { 
  0b01000100, 0b01000100, 0b01000100, 0b01010100, 0b01010100, 0b01010100, 0b00101000, 0b00000000 };
PROGMEM const unsigned char LETTERX[8] = { 
  0b01000100, 0b01000100, 0b00101000, 0b00010000, 0b00101000, 0b01000100, 0b01000100, 0b00000000 };
PROGMEM const unsigned char LETTERY[8] = { 
  0b01000100, 0b01000100, 0b01000100, 0b00101000, 0b00010000, 0b00010000, 0b00010000, 0b00000000 };
PROGMEM const unsigned char LETTERZ[8] = { 
  0b01111100, 0b00000100, 0b00001000, 0b00010000, 0b00100000, 0b01000000, 0b01111100, 0b00000000 };

/*****************Global Image Buffer**************/


unsigned char left_frame_buffer[32][8] =
{
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #1
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #2
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #3
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #4
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #5
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #6
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #7
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #8
  { 
    0b00111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111000   }
  ,     //Line #9
  { 
    0b00100000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00001000   }
  ,     //Line #10
  { 
    0b00100000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00001000   }
  ,     //Line #11
  { 
    0b00100000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00001000   }
  ,     //Line #12
  { 
    0b00100000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00001000   }
  ,     //Line #13
  { 
    0b00100000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00001000   }
  ,     //Line #14
  { 
    0b00100000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00001000   }
  ,     //Line #15
  { 
    0b00100000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00001000   }
  ,     //Line #16
  { 
    0b00100000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00001000   }
  ,     //Line #17
  { 
    0b00100000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00001000   }
  ,     //Line #18
  { 
    0b00100000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00001000   }
  ,     //Line #19
  { 
    0b00100000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00001000   }
  ,     //Line #20
  { 
    0b00100000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00001000   }
  ,     //Line #21
  { 
    0b00100000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00001000   }
  ,     //Line #22
  { 
    0b00111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111000   }
  ,     //Line #24
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #23
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #25
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #26
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #27
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #28
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #29
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #30
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #31
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #32
};

unsigned char right_frame_buffer[32][8] =
{
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #1
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #2
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #3
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #4
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #5
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #6
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #7
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #8
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #9
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #10
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #11
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #12
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #13
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #14
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #15
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #16
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #17
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #18
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #19
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #20
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #21
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #22
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #23
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #24
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #25
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #26
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #27
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #28
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #29
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #30
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #31
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #32
};

unsigned char frame_buffer[32][8] =
{
  { 
    0b00100000, 0b10000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #1
  { 
    0b00010001, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #2
  { 
    0b00111111, 0b11000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #3
  { 
    0b01101110, 0b11000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #4
  { 
    0b11111111, 0b11100000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #5
  { 
    0b10111111, 0b10100000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #6
  { 
    0b10100000, 0b10100000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #7
  { 
    0b00011011, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #8
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #9
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #10
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #11
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #12
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #13
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #14
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #15
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #16
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #17
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #18
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #19
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #20
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #21
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #22
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #23
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #24
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #25
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #26
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #27
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #28
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #29
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #30
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #31
  { 
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000   }
  ,     //Line #32
};


/******************Physical Registers*****************/

char cSREG;


/*******************DEBUG********************/
boolean DEBUG = HIGH;
int interrupt_count = 0;
volatile int led_state = LOW;
volatile int count = 1;

void setup() {   
  cSREG = SREG;

  Serial.begin(9600);
  attachInterrupt(interrupt_pin, buttonPressed, RISING);
  pinMode(2, INPUT);
  SPI.begin();                  //begin SPI communications
  SPI.setDataMode(SPI_MODE2);   //the smartswitch triggers on the falling edge of a normally low clock
  SPI.setBitOrder(MSBFIRST);    //the smartswitch takes in data MSB first
  Serial.println("SPI has begun");
  pinMode(led_out, OUTPUT);
  pinMode(slave_select_pin, OUTPUT);
  pinMode(smart_switch_button_pin, INPUT);



  digitalWrite(smart_switch_button_pin, HIGH);  //this is just so we don't need an external pullup resistor or whatever.
  digitalWrite(slave_select_pin, HIGH);        //slaveselect is active low, so it should be asserted high when it's not in use

  resetLCD();
  delay(5);
  setBrightness(0xDF);        //set lcd brightness to max
  delay(5);
  setColor(GREEN);       //set initial lcd color
  delay(5);
  Serial.println("LCD initialized");

  writeString(2, "BUTT");
  writeString(3, "CANNON");
  displayImage();
}

//Color test
void loop() {
  scrollRightByChar(0);
  displayImage();
  delay(1000);
  Serial.println("a");
  //digitalWrite(led_out, led_state);
}

/*void loop() {
 
 }*/


/********************Functions***********************/


// writeString takes a row number and a string, and prints it on the LED Button
void writeString(int row, char* string) {
  // 0 <= row <= 3
  // string length <= 8 (extra characters are truncated)

  for(int i = 0; (string[i] != NULL) && (i < 8); i++) {
    writeChar(i, row, string[i]); 
  }
}

void writeChar(int x, int y, char z) {
  Serial.println(y);
  char array_pos_1;
  char array_pos_2;
  char array_pos_3;
  char array_pos_4;
  char array_pos_5;
  char array_pos_6;
  char array_pos_7;
  char array_pos_8;
  char* pgm_mem_holder;
  int y_offset = y*8;
  switch(z) {
  case 'A':
    pgm_mem_holder = (char*)LETTERA;
    break;
  case 'B':
    pgm_mem_holder = (char*)LETTERB;
    break;
  case 'C':
    pgm_mem_holder = (char*)LETTERC;
    break;
  case 'D':
    pgm_mem_holder = (char*)LETTERD;
    break;
  case 'E':
    pgm_mem_holder = (char*)LETTERE;
    break;
  case 'F':
    pgm_mem_holder = (char*)LETTERF;
    break;
  case 'G':
    pgm_mem_holder = (char*)LETTERG;
    break;
  case 'H':
    pgm_mem_holder = (char*)LETTERH;
    break;
  case 'I':
    pgm_mem_holder = (char*)LETTERI;
    break;
  case 'J':
    pgm_mem_holder = (char*)LETTERJ;
    break;
  case 'K':
    pgm_mem_holder = (char*)LETTERK;
    break;
  case 'L':
    pgm_mem_holder = (char*)LETTERL;
    break;
  case 'M':
    pgm_mem_holder = (char*)LETTERM;
    break;
  case 'N':
    pgm_mem_holder = (char*)LETTERN;
    break;
  case 'O':
    pgm_mem_holder = (char*)LETTERO;
    break;
  case 'P':
    pgm_mem_holder = (char*)LETTERP;
    break;
  case 'Q':
    pgm_mem_holder = (char*)LETTERQ;
    break;
  case 'R':
    pgm_mem_holder = (char*)LETTERR;
    break;
  case 'S':
    pgm_mem_holder = (char*)LETTERS;
    break;
  case 'T':
    pgm_mem_holder = (char*)LETTERT;
    break;
  case 'U':
    pgm_mem_holder = (char*)LETTERU;
    break;
  case 'V':
    pgm_mem_holder = (char*)LETTERV;
    break;
  case 'W':
    pgm_mem_holder = (char*)LETTERW;
    break;
  case 'X':
    pgm_mem_holder = (char*)LETTERX;
    break;
  case 'Y':
    pgm_mem_holder = (char*)LETTERY;
    break;
  case 'Z':
    pgm_mem_holder = (char*)LETTERZ;
    break;
  }
  array_pos_1 = pgm_read_byte_near(pgm_mem_holder + 0);
  array_pos_2 = pgm_read_byte_near(pgm_mem_holder + 1);
  array_pos_3 = pgm_read_byte_near(pgm_mem_holder + 2);
  array_pos_4 = pgm_read_byte_near(pgm_mem_holder + 3);
  array_pos_5 = pgm_read_byte_near(pgm_mem_holder + 4);
  array_pos_6 = pgm_read_byte_near(pgm_mem_holder + 5);
  array_pos_7 = pgm_read_byte_near(pgm_mem_holder + 6);
  array_pos_8 = pgm_read_byte_near(pgm_mem_holder + 7);
  frame_buffer[y_offset+0][x] = array_pos_1;
  frame_buffer[y_offset+1][x] = array_pos_2;
  frame_buffer[y_offset+2][x] = array_pos_3;
  frame_buffer[y_offset+3][x] = array_pos_4;
  frame_buffer[y_offset+4][x] = array_pos_5;
  frame_buffer[y_offset+5][x] = array_pos_6;
  frame_buffer[y_offset+6][x] = array_pos_7;
  frame_buffer[y_offset+7][x] = array_pos_8;
}


void scrollRightByChar(int row_number) {
  row_number = row_number*8;
  char temp;
  for(int i = 0; i < 8; i++) {
    temp = frame_buffer[row_number+i][7];
    for(int column = 7; column > 0; column--) {
      frame_buffer[row_number+i][column] = frame_buffer[row_number+i][column-1];
    }
    frame_buffer[row_number+i][0] = temp;
  }
}
/********************Jesse's Code********************/
void setColor(byte colorByte)
{
  digitalWrite(slave_select_pin, LOW);

  SPI.transfer(COLOR);
  SPI.transfer(colorByte);

  digitalWrite(slave_select_pin, HIGH);
}

void setBrightness(byte color_byte)
{
  digitalWrite(slave_select_pin, LOW);

  SPI.transfer(BRIGHTNESS);
  SPI.transfer(color_byte);

  digitalWrite(slave_select_pin, HIGH);
}

void resetLCD()
{
  digitalWrite(slave_select_pin, LOW);

  SPI.transfer(0x5E);
  SPI.transfer(0b00000011);

  digitalWrite(slave_select_pin, HIGH);
}

void displayImage()
{
  digitalWrite(slave_select_pin, LOW);
  SPI.transfer(SENDBMP);

  SPI.setBitOrder(LSBFIRST);                           //set the SPI bus to transmit the least significant bit first, to turn it flipways. 

  for (int i = 0; i < 32; i++)                         //cycle through columns
  {
    for (int j = 7; j >= 0; j--)                       //cycle through the rows by byte, 8 rows at a time. (from most sig. byte to least)
    {
      SPI.transfer(frame_buffer[i][j]);  //write out the current byte of the image
    }
  }
  SPI.setBitOrder(MSBFIRST);                            //put everything back where you found it
  digitalWrite(slave_select_pin, HIGH);
}


/****************Interrupt Routines***************/

void buttonPressed() {
  Serial.println(digitalRead(2));
  count++;
  if (count > 3) {
    count = 1; 
  }

  /*if (DEBUG) interruptCount++;
   if (DEBUG) Serial.println(interruptCount);*/
  led_state = !led_state;
}

