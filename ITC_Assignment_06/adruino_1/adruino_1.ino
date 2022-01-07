#include <Wire.h>
#define AD2 9
#define AD3 10
//นี้คือ master
void setup()
{
  Wire.begin(); 
  Serial.begin(9600);
}

int x = 1; //byte

void loop()
{
  Wire.beginTransmission(AD2); // transmit to device #2
  Wire.write("HELLO");        // sends five bytes
  Wire.write(x);              // sends one byte  
  Wire.endTransmission();    // stop transmitting
  
  Wire.requestFrom(AD2,6);//6
  while( 1 < Wire.available()){
    char c = Wire.read();
    Serial.print(c);
  }
  x = Wire.read();
  Serial.println(x);
  
  Wire.beginTransmission(AD3); // transmit to device #3
  Wire.write("HELLO");        // sends five bytes
  Wire.write(x);              // sends one byte  
  Wire.endTransmission();
  
  Wire.requestFrom(AD3,6);
  while( 1 < Wire.available()){
    char c = Wire.read();
    Serial.print(c);
  }
  x = Wire.read();
  Serial.println(x);
  
  if(x == 9) x = 1;
  
  delay(1000);
}
