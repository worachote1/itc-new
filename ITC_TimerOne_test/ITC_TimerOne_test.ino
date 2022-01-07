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
#define buzzer 8

#define debounce 500

int h = 12 ;
int m  = 56;
int s = 4;
String state = "OFF";
void setup() {
  OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  OLED.clearDisplay();
  
  Serial.begin(9600);
  pinMode(7, OUTPUT);

  
  //Timer1.initialize(1000000);  
  Timer1.initialize(500000); //ทดลองให้นับจากจํานวนครั้งที่ led จะติดครบ 60 ครั้งแล้วเป็น 1 นาที(60 วินาที) Timer1.initialize(500000); ตรงที่สุด
  Timer1.attachInterrupt(blinkRed);
  
  OLED.display();
}

int a, b, c;
int testCount = 0;
int lastPress = 0;
void loop() {
  OLED.clearDisplay();
  OLED.setCursor(24, 14);
  OLED.setTextColor(WHITE);
  OLED.setTextSize(2);
  
  OLED.display();
}

void blinkRed()
{
  if(state == "OFF")
  {
    state = "ON";
    digitalWrite(7,LOW);
  }
  else if(state == "ON")
  {
    state = "OFF";
    testCount += 1 ;
    Serial.println(testCount);
    digitalWrite(7,HIGH);
  }
  
}

void soundPoint()
{
  //  digitalWrite(buzzer,HIGH);
  //
  //  digitalWrite(buzzer,LOW);
  tone( buzzer, 250, 50);
}
