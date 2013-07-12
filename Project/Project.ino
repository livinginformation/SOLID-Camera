#include <SPI.h>
#include <avr/pgmspace.h>


// Variables are in snake_case:
//     all_variables_are_written_like_this;

// Functions are in camelCase:
//     allFunctionsAreWrittenLikeThis();

// Global constants are in UPPERCASE
//     EXAMPLE



//  The Pin map below is copyright by the Arduino people. 
// ATMEL ATMEGA8 & 168 / ARDUINO
// ASSUMING THIS WORKS FOR ATMEL ATMEGA328P
// //
// //                  +-\/-+
// //            PC6  1|    |28  PC5 (AI 5)
// //      (D 0) PD0  2|    |27  PC4 (AI 4)
// //      (D 1) PD1  3|    |26  PC3 (AI 3)
// //      (D 2) PD2  4|    |25  PC2 (AI 2)
// // PWM+ (D 3) PD3  5|    |24  PC1 (AI 1)
// //      (D 4) PD4  6|    |23  PC0 (AI 0)
// //            VCC  7|    |22  GND
// //            GND  8|    |21  AREF
// //            PB6  9|    |20  AVCC
// //            PB7 10|    |19  PB5 (D 13)
// // PWM+ (D 5) PD5 11|    |18  PB4 (D 12)
// // PWM+ (D 6) PD6 12|    |17  PB3 (D 11) PWM
// //      (D 7) PD7 13|    |16  PB2 (D 10) PWM
// //      (D 8) PB0 14|    |15  PB1 (D 9) PWM
// //                  +----+
// 

const int LEDPIN = 13;       //This is just for the LED on the arduino.
const int BTNPIN = 2;        //This needs to be pin 2 for ext. interrupt 0.

int debounceTimer = 0;
boolean ledState = LOW;
volatile boolean button_flag = LOW;
int btnCount = 0;

/*********Rotary Encoder**********/
volatile int pin_a = 5;
volatile int pin_b = 7;
int power_pin = 3;
volatile int prev_a = 0;
volatile int prev_b = 0;
volatile int curr_a = 0;
volatile int curr_b = 0;
volatile int direction = 0;
volatile boolean changed = false;
int ticks = 0;
int line_selected = 0;

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
const int PIXEL_HEIGHT = 32;
const int PIXEL_WIDTH = 64;


/***********Pin and Interrupt Assignments************/
const int slave_select_pin = 10;
const int smart_switch_button_pin = 6;
int interrupt_pin = 0;
int interrupt_state = LOW;
int led_out = 4;

/******************Program Memory********************/

PROGMEM const unsigned char LETTERA[8] = { 0b00111000, 0b01000100, 0b01000100, 0b01000100, 0b01111100, 0b01000100, 0b01000100, 0b00000000 };
PROGMEM const unsigned char LETTERB[8] = { 0b01111000, 0b01000100, 0b01000100, 0b01111000, 0b01000100, 0b01000100, 0b01111000, 0b00000000 };
PROGMEM const unsigned char LETTERC[8] = { 0b00111000, 0b01000100, 0b01000000, 0b01000000, 0b01000000, 0b01000100, 0b00111000, 0b00000000 };
PROGMEM const unsigned char LETTERD[8] = { 0b01110000, 0b01001000, 0b01000100, 0b01000100, 0b01000100, 0b01001000, 0b01110000, 0b00000000 };
PROGMEM const unsigned char LETTERE[8] = { 0b01111100, 0b01000000, 0b01000000, 0b01111000, 0b01000000, 0b01000000, 0b01111100, 0b00000000 };
PROGMEM const unsigned char LETTERF[8] = { 0b01111100, 0b01000000, 0b01000000, 0b01111000, 0b01000000, 0b01000000, 0b01000000, 0b00000000 };
PROGMEM const unsigned char LETTERG[8] = { 0b00111000, 0b01000100, 0b01000000, 0b01011100, 0b01000100, 0b01000100, 0b00111100, 0b00000000 };
PROGMEM const unsigned char LETTERH[8] = { 0b01000100, 0b01000100, 0b01000100, 0b01111100, 0b01000100, 0b01000100, 0b01000100, 0b00000000 };
PROGMEM const unsigned char LETTERI[8] = { 0b00111000, 0b00010000, 0b00010000, 0b00010000, 0b00010000, 0b00010000, 0b00111000, 0b00000000 };
PROGMEM const unsigned char LETTERJ[8] = { 0b00111000, 0b00010000, 0b00010000, 0b00010000, 0b00010000, 0b10010000, 0b01100000, 0b00000000 };
PROGMEM const unsigned char LETTERK[8] = { 0b01000100, 0b01001000, 0b01010000, 0b01100000, 0b01010000, 0b01001000, 0b01000100, 0b00000000 };
PROGMEM const unsigned char LETTERL[8] = { 0b01000000, 0b01000000, 0b01000000, 0b01000000, 0b01000000, 0b01000000, 0b01111110, 0b00000000 };
PROGMEM const unsigned char LETTERM[8] = { 0b01000100, 0b01101100, 0b01010100, 0b01010100, 0b01000100, 0b01000100, 0b01000100, 0b00000000 };
PROGMEM const unsigned char LETTERN[8] = { 0b01000100, 0b01000100, 0b01100100, 0b01010100, 0b01001100, 0b01000100, 0b01000100, 0b00000000 };
PROGMEM const unsigned char LETTERO[8] = { 0b00111000, 0b01000100, 0b01000100, 0b01000100, 0b01000100, 0b01000100, 0b00111000, 0b00000000 };
PROGMEM const unsigned char LETTERP[8] = { 0b01111000, 0b01000100, 0b01000100, 0b01111000, 0b01000000, 0b01000000, 0b01000000, 0b00000000 };
PROGMEM const unsigned char LETTERQ[8] = { 0b00111000, 0b01000100, 0b01000100, 0b01000100, 0b01010100, 0b01001000, 0b00110100, 0b00000000 };
PROGMEM const unsigned char LETTERR[8] = { 0b01111000, 0b01000100, 0b01000100, 0b01111000, 0b01010000, 0b01001000, 0b01000100, 0b00000000 };
PROGMEM const unsigned char LETTERS[8] = { 0b00111100, 0b01000000, 0b01000000, 0b00111000, 0b00000100, 0b00000100, 0b01111000, 0b00000000 };
PROGMEM const unsigned char LETTERT[8] = { 0b01111100, 0b00010000, 0b00010000, 0b00010000, 0b00010000, 0b00010000, 0b00010000, 0b00000000 };
PROGMEM const unsigned char LETTERU[8] = { 0b01000100, 0b01000100, 0b01000100, 0b01000100, 0b01000100, 0b01000100, 0b00111000, 0b00000000 };
PROGMEM const unsigned char LETTERV[8] = { 0b01000100, 0b01000100, 0b01000100, 0b01000100, 0b01000100, 0b00101000, 0b00010000, 0b00000000 };
PROGMEM const unsigned char LETTERW[8] = { 0b01000100, 0b01000100, 0b01000100, 0b01010100, 0b01010100, 0b01010100, 0b00101000, 0b00000000 };
PROGMEM const unsigned char LETTERX[8] = { 0b01000100, 0b01000100, 0b00101000, 0b00010000, 0b00101000, 0b01000100, 0b01000100, 0b00000000 };
PROGMEM const unsigned char LETTERY[8] = { 0b01000100, 0b01000100, 0b01000100, 0b00101000, 0b00010000, 0b00010000, 0b00010000, 0b00000000 };
PROGMEM const unsigned char LETTERZ[8] = { 0b01111100, 0b00000100, 0b00001000, 0b00010000, 0b00100000, 0b01000000, 0b01111100, 0b00000000 };
PROGMEM const unsigned char LETTERSPACE[8] = { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 };
PROGMEM const unsigned char NUMBER0[8] = { 0b00111100, 0b01100110, 0b01101110, 0b01111110, 0b01110110, 0b01100110, 0b00111100, 0b00000000 };

/*****************Global Image Buffer**************/


unsigned char frame_buffer[32][8] =
{
    { 0b00100000, 0b10000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 },     //Line #1
    { 0b00010001, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 },     //Line #2
    { 0b00111111, 0b11000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 },     //Line #3
    { 0b01101110, 0b11000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 },     //Line #4
    { 0b11111111, 0b11100000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 },     //Line #5
    { 0b10111111, 0b10100000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 },     //Line #6
    { 0b10100000, 0b10100000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 },     //Line #7
    { 0b00011011, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 },     //Line #8
    { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 },     //Line #9
    { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 },     //Line #10
    { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 },     //Line #11
    { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 },     //Line #12
    { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 },     //Line #13
    { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 },     //Line #14
    { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 },     //Line #15
    { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 },     //Line #16
    { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 },     //Line #17
    { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 },     //Line #18
    { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 },     //Line #19
    { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 },     //Line #20
    { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 },     //Line #21
    { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 },     //Line #22
    { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 },     //Line #23
    { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 },     //Line #24
    { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 },     //Line #25
    { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 },     //Line #26
    { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 },     //Line #27
    { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 },     //Line #28
    { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 },     //Line #29
    { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 },     //Line #30
    { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 },     //Line #31
    { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 },     //Line #32
};

unsigned char boot_logo_solid[32][8] =
{    
    { 0b00000000, 0b00111111, 0b11100000, 0b00001111, 0b11001111, 0b11111110, 0b01111001, 0b11111000 },
    { 0b00001111, 0b00011000, 0b11000111, 0b10001100, 0b01001000, 0b00000010, 0b01001001, 0b00001100 },
    { 0b00011001, 0b10001001, 0b10011100, 0b11100110, 0b01001000, 0b00000010, 0b01001001, 0b00000110 },
    { 0b00110000, 0b11001001, 0b00010000, 0b00100010, 0b01001000, 0b00000010, 0b01001001, 0b00000011 },
    { 0b00100000, 0b01001001, 0b00110000, 0b00110010, 0b01001000, 0b00000010, 0b01001001, 0b00000001 },
    { 0b00110000, 0b01111011, 0b00100000, 0b00010011, 0b01001000, 0b00000010, 0b01001001, 0b00000001 },
    { 0b00011100, 0b00000010, 0b01100000, 0b00011001, 0b01001000, 0b00000010, 0b01001001, 0b00000001 },
    { 0b10000111, 0b11000010, 0b01000000, 0b00001001, 0b01001000, 0b00000010, 0b01001001, 0b00000001 },
    { 0b11100000, 0b01110010, 0b01000000, 0b00001001, 0b01001000, 0b00000010, 0b01001001, 0b00000001 },
    { 0b10111000, 0b00011010, 0b01000000, 0b00001001, 0b01001000, 0b00000010, 0b01001001, 0b00000001 },
    { 0b10001111, 0b00001010, 0b01000000, 0b00001001, 0b01001000, 0b00000010, 0b01001001, 0b00000001 },
    { 0b11000001, 0b11001011, 0b01000000, 0b00001011, 0b01001000, 0b00000010, 0b01001001, 0b00000001 },
    { 0b01100000, 0b01101101, 0b01100000, 0b00011010, 0b01001000, 0b00000010, 0b01001001, 0b00000001 },
    { 0b00100000, 0b00100101, 0b00100000, 0b00010010, 0b01001000, 0b00000010, 0b01001001, 0b00000001 },
    { 0b00110000, 0b01100101, 0b10110000, 0b00110110, 0b01001000, 0b00000010, 0b01001001, 0b00000011 },
    { 0b00011000, 0b11001100, 0b10011100, 0b11100100, 0b01001111, 0b11111010, 0b01001001, 0b00000110 },
    { 0b00001111, 0b10011000, 0b11000111, 0b10001100, 0b01000000, 0b00001010, 0b01001001, 0b00001100 },
    { 0b00000000, 0b00011111, 0b11000000, 0b00001111, 0b11000000, 0b00001110, 0b01111001, 0b11111000 },
    { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 },
    { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 },
    { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 },
    { 0b00011101, 0b11010011, 0b10110000, 0b01100100, 0b01110011, 0b10110001, 0b00000000, 0b00000000 },
    { 0b00010001, 0b01010001, 0b00101000, 0b10001010, 0b10101010, 0b00101010, 0b10000000, 0b00000000 },
    { 0b00011101, 0b01010001, 0b00101000, 0b10001110, 0b10101011, 0b00110011, 0b10000000, 0b00000000 },
    { 0b00000101, 0b01010001, 0b00101000, 0b10001010, 0b10101010, 0b00101010, 0b10000000, 0b00000000 },
    { 0b00011101, 0b11011011, 0b10110000, 0b01101010, 0b10101011, 0b10101010, 0b10000000, 0b00000000 },
    { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 },
    { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b11001110, 0b10100110, 0b00001100 },
    { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b10101000, 0b10100010, 0b00000100 },
    { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b11001100, 0b10100010, 0b00000100 },
    { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b10101000, 0b10100010, 0b00000100 },
    { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b10101110, 0b01000111, 0b01001110 }
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
  TCCR2B = 0x00;         // Disable Timer2 for the beginning of setup()
  PCICR  |= 0b00000100;  // 
  PCMSK2 |= 0b10100000;
  EICRA  |= 0b00000011;
  EIMSK  |= 0b00000001;
  TIMSK2 |= 0b00000110;  // Enable output compare match A, B interrupt for timer/counter 2, initially 00000000, datasheet page 159
  TCCR2A |= 0b10000000;  // Set clear interrupt flag on compare match for timer/counter 2 control register A, initially 0, datasheet page 155
  OCR2B   = 125;         // Set output compare register A for counter 2 to 125. At 16MHz and 128 prescaler, the flag triggers every 1 ms
  
  Serial.begin(9600);
  //attachInterrupt(interrupt_pin, buttonPressed, CHANGE);
  //pinMode(2, INPUT);
  SPI.begin();                  //begin SPI communications
  SPI.setDataMode(SPI_MODE2);   //the smartswitch triggers on the falling edge of a normally low clock
  SPI.setBitOrder(MSBFIRST);    //the smartswitch takes in data MSB first
  //Serial.println("SPI has begun");
  pinMode(led_out, OUTPUT);
  pinMode(slave_select_pin, OUTPUT);
  pinMode(smart_switch_button_pin, INPUT);
  pinMode(pin_a, INPUT);
  pinMode(pin_b, INPUT);
  pinMode(power_pin, OUTPUT);
  digitalWrite(power_pin, HIGH);
  digitalWrite(2, HIGH);
  digitalWrite(smart_switch_button_pin, HIGH);  //this is just so we don't need an external pullup resistor or whatever.
  digitalWrite(slave_select_pin, HIGH);        //slaveselect is active low, so it should be asserted high when it's not in use

 
  resetLCD();
  delay(5);
  setBrightness(0xDF);        //set lcd brightness to max
  delay(5);
  setColor(ORANGE);       //set initial lcd color
  delay(5);
  Serial.println("LCD initialized");
  bootLoader();
  delay(30000);
  TCCR2B |= 0b00000101;
  sei();
  
  writeString(0, "ONE");
  writeString(1, "TWO");
  writeString(2, "THREE");
  writeString(3, "FOUR");
  invertRow(0);
  displayImage();
}

//Color test
void loop() {
    if(button_flag) {
      Serial.println(line_selected+1);
      button_flag = LOW;
    }
    if(changed) {
    changed = false;
    if(curr_a == 1 && curr_b == 1) {
      if(prev_a == 0 && prev_b == 1) {
        ticks++;

      }else{
        ticks--;

      }
    }
    if (ticks != line_selected) {
        if(ticks > (PIXEL_HEIGHT/8 -1)) ticks = 0;
        if(ticks < 0) ticks = (PIXEL_HEIGHT/8 -1);
        invertRow(line_selected);
        line_selected = ticks;
        invertRow(line_selected);
        displayImage();
    }

  }
//  scrollString(0, 500, "SOLID CAMERA RULES");

 //     invertRow(3);

/*  digitalWrite(led_out, led_state);
  delay(75);
  attachInterrupt(interrupt_pin, buttonPressed, CHANGE);
}*/
}

/*void loop() {
   
}*/


/********************Functions***********************/

void invertRow(int row) {
  int y_offset = row*8;
  for(int i = 0; i < 8; i++) {
    for(int j = 0; j < 8; j++) {
      frame_buffer[y_offset+i][j] = (frame_buffer[y_offset+i][j]) ^ (0b11111111);
    }
  }
}

// writeString takes a row number and a string, and prints it on the LED Button
void writeString(int row, char* string) {
  // 0 <= row <= 3
  // string length <= 8 (extra characters are truncated)
  
  for(int i = 0; (string[i] != NULL) && (i < 8); i++) {
    writeChar(i, row, string[i]); 
  }
}

void writeChar(int x, int y, char z) {
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
    case '0':
      pgm_mem_holder = (char*)NUMBER0;
      break;
    case ' ':
        pgm_mem_holder = (char*)LETTERSPACE;
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

void scrollLeftByChar(int row_number) {
  row_number = row_number*8;
  char temp;
  for(int i = 0; i < 8; i++) {
    temp = frame_buffer[row_number+i][0];
    for(int column = 0; column < 7; column++) {
      frame_buffer[row_number+i][column] = frame_buffer[row_number+i][column+1];
    }
    frame_buffer[row_number+i][7] = 0b00000000;
  }
}

void scrollString(int row_number, int delay_ms, char* string) {
  writeString(row_number, "        ");
  for(int i = 0; (string[i] != NULL); i++) {
    delay(delay_ms);
    scrollLeftByChar(row_number);
    writeChar(7, row_number, string[i]); 
    displayImage();
  }
  for(int i = 0; i < 8; i++) {
    delay(delay_ms);
    scrollLeftByChar(row_number);
    displayImage();
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

/*void displayImage()
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
*/

void displayImage()
{
  digitalWrite(slave_select_pin, LOW);
  SPI.transfer(SENDBMP);
  
  SPI.setBitOrder(LSBFIRST);                           //set the SPI bus to transmit the least significant bit first, to turn it flipways. 
  
  for (int i = 0; i < PIXEL_HEIGHT; i++)                         //cycle through columns
  {
    for (int j = (PIXEL_WIDTH/8 -1); j >= 0; j--)                       //cycle through the rows by byte, 8 rows at a time. (from most sig. byte to least)
    {
      SPI.transfer(frame_buffer[i][j]);  //write out the current byte of the image

      //Serial.println(boot_logo_solid[i][j]);
      //Serial.println("-----");M
    }
  }
  SPI.setBitOrder(MSBFIRST);                            //put everything back where you found it
  digitalWrite(slave_select_pin, HIGH);
}

void bootLoader()
{
  digitalWrite(slave_select_pin, LOW);
  SPI.transfer(SENDBMP);
  
  SPI.setBitOrder(LSBFIRST);                           //set the SPI bus to transmit the least significant bit first, to turn it flipways. 
  
  for (int i = 0; i < PIXEL_HEIGHT; i++)                         //cycle through columns
  {
    for (int j = (PIXEL_WIDTH/8 -1); j >= 0; j--)                       //cycle through the rows by byte, 8 rows at a time. (from most sig. byte to least)
    {
      SPI.transfer(boot_logo_solid[i][j]);  //write out the current byte of the image

      //Serial.println(boot_logo_solid[i][j]);
      //Serial.println("-----");M
    }
  }
  SPI.setBitOrder(MSBFIRST);                            //put everything back where you found it
  digitalWrite(slave_select_pin, HIGH);
}


/****************Interrupt Routines***************/

void buttonPressed() {
  count++;
  if (count > 3) {
    count = 1; 
  } 
  /*if (DEBUG) interruptCount++;
  if (DEBUG) Serial.println(interruptCount);*/  
  led_state = !led_state;
}

ISR(PCINT2_vect) {
  prev_a = curr_a;
  prev_b = curr_b;
  curr_a = digitalRead(pin_a);
  curr_b = digitalRead(pin_b);
  changed = true;
}

ISR(TIMER2_COMPB_vect)
{
  TIMSK2 = 0x00; // turn off timer
  if (debounceTimer > 0)
  {
    debounceTimer--;
  }

  TCNT2 = 0; //sets timer 2 to 0    
  TIFR2 = 0x00; //clear timer 2 output compare A flag
  TIMSK2 |= 0b00000110; //re-enable timer
}


//Switch interrupt routine (triggers on falling edge)
ISR(INT0_vect)
{
  if (debounceTimer == 0)
  {
    TIMSK2 = 0x00; // turn off timer 2
    btnCount++;
    debounceTimer = 2;
    ledState = !ledState;
    digitalWrite(LEDPIN, ledState);
    if( digitalRead(BTNPIN) == LOW)
    {
        //empty
    }
    else
    {
      //always trigger, set button flag
      button_flag = HIGH;
    }
    TIMSK2 |= 0b00000110; //enables output compare match A, B interrupt for timer/counter 2, initially 00000000, datasheet page 159
  }
}

