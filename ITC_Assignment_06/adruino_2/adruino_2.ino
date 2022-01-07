#include <Wire.h>
int x;
//นี้คือ AD2
void setup()
{
  Wire.begin(9);                // join i2c bus with address 
  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(requestEvent);
  Serial.begin(9600);           // start serial for output
}

void loop()
{
  delay(1000);
}

void receiveEvent(int howMany)
{
  while(1 < Wire.available()) // loop through all but the last
  {
    char c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
  }
  x = Wire.read();    // receive byte as an integer
  Serial.println(x);         // print the integer
  
}

void requestEvent(){
    x += 1;
  Wire.write("HELLO");
    Wire.write(x);
}
