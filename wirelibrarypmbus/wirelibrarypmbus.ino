///////////////////////////////////////////////////////////////
//PMBUS over Wire Test code
//SOLID CAMERA
//Jesse Kent
//7/3/13
///////////////////////////////////////////////////////////////

//Libraries////////////////////////////////////////////////////
#include <Wire.h>

//Constants////////////////////////////////////////////////////
const int moduleAddr = 54;          //the address of the pmbus module should be 54
  

//Setup////////////////////////////////////////////////////
void setup()
{
  Serial.begin(9600);
  Serial.println("beginning setup");
  
  Wire.begin(moduleAddr);
  
  
}
//Loop////////////////////////////////////////////////////
void loop()
{


}
  

//Functions////////////////////////////////////////////////////








uint16_t readRegister(int cmd, int reg)
{
  uint16_t dIn = 0;
  Wire.beginTransmission(moduleAddr);
  Wire.write(cmd);
  Wire.write(reg);
  Wire.endTransmission();

  byte n = Wire.requestFrom(moduleAddr, 8, false);
  Serial.print(n);
  Serial.print(" ");
  
  while (Wire.available())    // slave may send less than requested
  {
    char c = Wire.read();    // receive a byte as character
    Serial.print(c);         // print the character
  }
  return dIn;
}

void outputOn()
{
  Wire.beginTransmission(moduleAddr);
  Wire.write(0x01);           //operation code
  Wire.write(0b10000000);     //bit 7 sets the output on or off
  Wire.endTransmission();
  return;
}

void outputOff()
{
  Wire.beginTransmission(moduleAddr);
  Wire.write(0x01);           //operation code
  Wire.write(0b00000000);     //bit 7 sets the output on or off
  Wire.endTransmission();
  return;
}
