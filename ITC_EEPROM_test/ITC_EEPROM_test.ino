#include <EEPROM.h>
#include <TimerOne.h>

#define ledPin 13
#define buttonPin 12
#define resetButtonpin 11
#define debounce 150

byte lastButtonState = 1;
int lastPress = 0;
int counter = 0,reset_round = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(resetButtonpin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);

  counter = EEPROM.read(0);
  reset_round = EEPROM.read(sizeof(counter));
//  EEPROM.write(0, 0);
}

int red,reset;
//Once out of power or stop running , counter still be the lasted value as it'been press

void loop()
{
//  
//  red = digitalRead(buttonPin);
//  reset = digitalRead(resetButtonpin);
    red = digitalRead(resetButtonpin);
    reset = digitalRead(buttonPin);
  if (millis() - lastPress >= debounce)
  {
    lastPress = millis();
    //press to count
    if (red == 0 && lastButtonState == 1)
    {
      counter++;
      EEPROM.put(0, counter);
      //lastPress = millis();
      digitalWrite(ledPin, HIGH);
      lastButtonState = 0;

      Serial.print("Counter = ");
      EEPROM.get(0, counter);
      Serial.println(counter);
    }

    else if(red == 1 && lastButtonState == 0)
    {
      digitalWrite(ledPin, LOW);
      lastButtonState = 1;
    }

    //reset section
    else if (reset == 0)
    {
      // lastPress = millis();
       counter=0;
       reset_round++;
       Serial.print("my reset round = ");
       Serial.println(reset_round);
       
       EEPROM.put(0, counter);
       EEPROM.get(0, counter);
       
       EEPROM.put(sizeof(counter), reset_round);
       EEPROM.get(sizeof(counter), reset_round);

       Serial.println(sizeof(counter));
    }
  }

}
