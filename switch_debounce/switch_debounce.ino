//////////////////////////////////////////////////
//Switch motherfucking debouncer
//Solid Camera
//7/3/13
//Jesse Kent
///////////////////////////////////////////////////

const int LEDPIN = 13;
const int BTNPIN = 2;

int debounceTimer = 0;
boolean ledState = LOW;

void setup()
{
  TCCR2B = 0x00;        //Disbale Timer2 for beginning of setup
  Serial.begin(9600);
  
  pinMode(LEDPIN, OUTPUT);
  pinMode(BTNPIN, INPUT);

  EICRA |= 0b00000010;
  EIMSK |= 0b00000001;
  
  TIMSK2 |= 0b00000110; //enables output compare match A, B interrupt for timer/counter 2, initially 00000000, datasheet page 159
  TCCR2A |= 0b10000000; //sets clear interrupt flag on compare match for timer/counter 2 control register A, initially 0, datasheet pg 155
  OCR2B = 125; //sets output compare register A for counter 2 to 125. @ 16MHz clock and 128 prescaler, flag triggers every 1 ms. 
  TCCR2B |= 0b00000101; //sets clock scaling to /128, initially 0, pg 158
}

void loop()
{

}

/////////////////////////////////////////////////////////
//FUNCTIONS
/////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////
//ISR
/////////////////////////////////////////////////////////

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
    Serial.println(debounceTimer);
    TIMSK2 = 0x00; // turn off timer 2
    Serial.println("Button Pressed");
    debounceTimer = 50;
    ledState = !ledState;
    digitalWrite(LEDPIN, ledState);
    TIMSK2 |= 0b00000110; //enables output compare match A, B interrupt for timer/counter 2, initially 00000000, datasheet page 159
  }
}
