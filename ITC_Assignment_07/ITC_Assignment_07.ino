
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
Adafruit_SSD1306 OLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#include <EEPROM.h>
#include <TimerOne.h>

//for sound
#define buzzer 2

#define debounce 50

#define ok_button 12

#define countTime_button 11
#define alarm_button 10

#define add_minute_button 9
#define add_hour_button 8

//for working with Dark Mode , Light Mode using LDR module
#define ldr A0


int h = 1 ;
int m  = 0;
int s = 4;
void setup() {
  OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  OLED.clearDisplay();
  //drawArea();
  Serial.begin(9600);

  //No matter which part of the program's running , if reach to 1 second excute timePass().
  Timer1.initialize(1000000);
  Timer1.attachInterrupt(timePass);


  //
  pinMode(ok_button, INPUT_PULLUP);
  pinMode(countTime_button, INPUT_PULLUP);
  pinMode(alarm_button, INPUT_PULLUP);
  pinMode(add_minute_button, INPUT_PULLUP);
  pinMode(add_hour_button, INPUT_PULLUP);

  //sound
  pinMode(buzzer,OUTPUT);
  
  h = EEPROM.read(0);
  m = EEPROM.read(sizeof(h));
  s = EEPROM.read(sizeof(m) * 2);
    

  OLED.display();
}

int ok, countTime, alarm, minute, hour;//button variable
int lastPress = 0;

int alarm_h = 0     , alarm_m = 0;
int countTime_m = 0 , countTime_s = 0;

String alarm_state = "OFF" , countTime_state = "OFF";
String sound_state = "OFF";

//resistor value inverse with lux
//more light less R , less light more R
int resistor_value ;

void loop() {
  OLED.clearDisplay();

  // button variable
  ok = digitalRead(ok_button);
  countTime = digitalRead(countTime_button);
  alarm = digitalRead(alarm_button);
  minute = digitalRead(add_minute_button);
  hour = digitalRead(add_hour_button);

  // LDR variable
  resistor_value = analogRead(ldr);
  //Serial.println(resistor_value);

  //select Dark Mode or Light Mode depend on lux_value
  if(resistor_value >= 440)
  {
    display_light_mode();
  }
  else
  {
    display_dark_mode();  
  }
  
  if (millis() - lastPress >= debounce )
  {
    lastPress = millis();
    if (hour == 0)
    {
      Serial.println("hour");
      if (alarm_state == "ON") // if there any press on this button while alarm_state == "ON" --> alarm_h ++
      {
        alarm_h += 1 ;
        if(alarm_h > 23)
        {
          alarm_h = 0;
        }
        
        sound_add_time();
      }
    }
    else if (minute == 0)
    {
      Serial.println("minute");
      if (alarm_state == "ON") // if there any press on this button while alarm_state == "ON" --> alarm_m ++
      {
        alarm_m += 1 ;
        if(alarm_m > 59)
        {
          alarm_h += 1;
          alarm_m = 0;
        }
        
        sound_add_time();
      }
    }

    else if (countTime == 0)
    {
      if (countTime_state == "OFF")
      {
        Serial.println("CountTime now Turn ON ");
        countTime_state = "ON";
      }
      else  // countTime_state == "ON"
      {
        Serial.println("CountTime now Turn OFF ");
        countTime_m = 0 ;
        countTime_s = 0;
        countTime_state = "OFF";
      }
    }
    else if (alarm == 0)
    {
      if (alarm_state == "OFF")
      {
        alarm_h = 0 ;
        alarm_m = 0;
        Serial.println("Alarm now Turn ON ");
        alarm_state = "ON";
      }
//      else // alarm_state == "ON"
//      {
//        Serial.println("alarm now Turn OFF ");
//      }
    }

    else if (ok == 0)
    { 
      ok_saveTime();
      Serial.println("ok");
      Serial.println("Alarm now Turn OFF ");
      alarm_state = "OFF";
      sound_state = "OFF";
    }

  }

  // Display Section
  
  // display normal mode ,if not press countTime_button or alarm_button
  if (alarm_state == "OFF" && countTime_state == "OFF")
  {
    display_normal();
  }

  //display countTime mode , if countTime_button has been pressed
  else if (countTime_state == "ON")
  {
    Serial.println("run display_countTime");
    display_countTime();
  }

  //display alarm mode , if alarm_button has been pressed
  else if (alarm_state == "ON")
  {
    Serial.println("run display_alarm ");
    display_alarm();
  }

  //check if sound should be played ?
  if(sound_state == "ON")
  {
    sound_alarm();                                    //-----------------------------------
  }

//  //save to EEPROM
//  EEPROM.put(0, h);
//  EEPROM.get(0, h);
//
//  EEPROM.put(sizeof(h), m);
//  EEPROM.get(sizeof(h), m);
//
//  EEPROM.put(sizeof(m) * 2, s);
//  EEPROM.get(sizeof(m) * 2, s);
  
  OLED.display();
}

//save time , if ok button pressed
void ok_saveTime()
{
  //save to EEPROM
  EEPROM.put(0, h);
  EEPROM.get(0, h);

  EEPROM.put(sizeof(h),m );
  EEPROM.get(sizeof(h), m);

  EEPROM.put(sizeof(m) * 2, s);
  EEPROM.get(sizeof(m) * 2, s);
}

//Dark Mode and Light Mode
void display_dark_mode()
{
  OLED.setTextColor(WHITE);
  OLED.setTextSize(2);
}
void display_light_mode()
{
  OLED.fillScreen(WHITE);
  OLED.setTextColor(BLACK);
  OLED.setTextSize(2);
}

void display_normal()  //normal mode function
{
  //Normal Mode display section
  OLED.setCursor(24, 14);
//  OLED.setTextColor(WHITE);
//  OLED.setTextSize(2);
  if (h < 10)
  {
    OLED.print("0");
  }
  OLED.print(h);
  OLED.print(":");

  if (m < 10)
  {
    OLED.print("0");
  }

  OLED.print(m);
  OLED.print(":");

  if (s < 10)
  {
    OLED.print("0");
  }
  OLED.print(s);
}

int countTime_pass = 0;
void display_countTime() // countTime mode function
{
  //CountTime Mode display section
  OLED.setCursor(28, 14);
//  OLED.setTextColor(WHITE);
//  OLED.setTextSize(2);

  if (countTime_m < 10)
  {
    OLED.print("0");
  }

  OLED.print(countTime_m);
  OLED.print(":");

  if (countTime_s < 10)
  {
    OLED.print("0");
  }
  if (countTime_s > 59)
  {
    countTime_m += 1;
    countTime_s = 0;
  }
  OLED.print(countTime_s);

  // countTime_s += 1; put this line of code in timePass instead , because of Timer1.attachInterrupt(timePass);
}

void display_alarm()      // Alarm mode function
{
  //Alarm Mode display section
  OLED.setCursor(28, 14);
//  OLED.setTextColor(WHITE);
//  OLED.setTextSize(2);
  if (alarm_h < 10)
  {
    OLED.print("0");
  }

  OLED.print(alarm_h);
  OLED.print(":");

  if (alarm_m < 10)
  {
    OLED.print("0");
  }
 
  OLED.print(alarm_m);
}

//play sound function

// play sound when alarm end (will be use when sound_state == "ON")
void sound_alarm(){
  tone(buzzer, 500, 50);
  Serial.println("play sound alarm");
 }
//play sound when minute or hour button pressed
void sound_add_time()
{
  tone( buzzer, 250, 50);
  Serial.println("play sound add time");
} 

void timePass()
{
  //OLED.clearDisplay();
  s += 1;

  if (h == 24)
  {
    h = 0;
  }
  if (m > 59)
  {
    h++;
    m = 0;
  }
  if (s > 59)
  {
    m++;
    s = 0;
  }

  if (countTime_state == "ON") //add each 1 second in CountTime Mode
  {
    countTime_s += 1;
  }

  if ( h == alarm_h && m == alarm_m && alarm_state == "OFF") // wake up , get to work !!!
  {
    alarm_h = 0;
    alarm_m = 0;
    sound_state = "ON";
    Serial.println("Alarm sucess !!!");
  }
}
