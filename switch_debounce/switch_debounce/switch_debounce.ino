//////////////////////////////////////////////////
//Switch debouncer using External Interrupts.
//Solid Camera
//7/3/13
//Jesse Kent
///////////////////////////////////////////////////

const int LEDPIN = 13;       //This is just for the LED on the arduino.
const int BTNPIN = 2;        //This needs to be pin 2 for ext. interrupt 0.

int debounceTimer = 0;
boolean ledState = LOW;

int btnCount = 0;

void setup()
{
  TCCR2B = 0x00;        //Disbale Timer2 for beginning of setup
  Serial.begin(9600);
  
  pinMode(LEDPIN, OUTPUT);
  pinMode(BTNPIN, INPUT);
  digitalWrite(BTNPIN, HIGH);

  EICRA |= 0b00000011;
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
    TIMSK2 = 0x00; // turn off timer 2
    btnCount++;
    Serial.println(btnCount);
    debounceTimer = 2;
    ledState = !ledState;
    digitalWrite(LEDPIN, ledState);
    if( digitalRead(BTNPIN) == LOW)
    {
      Serial.println("Button Pressed");
    }
    else
    {
      Serial.println("Button Released");
    }
    TIMSK2 |= 0b00000110; //enables output compare match A, B interrupt for timer/counter 2, initially 00000000, datasheet page 159
  }
}


