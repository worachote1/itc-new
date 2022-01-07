

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
Adafruit_SSD1306 OLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int random_value;

//for player
int moveToRight;
int moveToLeft;

int player_x = 64;
int player_y = 29;
int player_width = 25;
int score = 0;

//for the ball
int ball_x = 64;
int ball_y = 2;
int ball_radius = 2.5;

void drawArea();
void endGame();

//for sound
int buzzer = 8;
void soundPoint();
void soundHitWall();

void setup() {
  OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  OLED.clearDisplay();
  //drawArea();
  Serial.begin(9600);
  pinMode(13, INPUT_PULLUP); // button for move right
  pinMode(12, INPUT_PULLUP);// button for move left
  pinMode(buzzer,OUTPUT);
  //for randomseed
  randomSeed(analogRead(A0));

  //OLED.setTextColor(WHITE, BLACK);
  //OLED.setCursor(15, 1);
  //OLED.setTextSize(1);
  //OLED.println("");
  //OLED.setCursor(10, 18);
  //OLED.setTextSize(2);
  //OLED.println("");
  
  OLED.display();
}

int debounce = 150, time = 0 ;
int new_player_x , before_player_x;

int new_ball_x , new_ball_y;
int ball_change_x = 1, ball_change_y = 1;
int ball_time = 0;
int new_score = 0;

bool stop_the_game = false;
void loop() {
  
  random_value = random(1, 10);
  moveToRight = digitalRead(13);
  moveToLeft = digitalRead(12);

  
  if (!stop_the_game)
  {
  drawArea();
  //soundPoint();
    Serial.println("game is running");
    //for player
    if (moveToRight == 0 && millis() - time > debounce) {
      Serial.println("player move right");
      Serial.println(random_value);
      before_player_x = player_x;
      new_player_x = player_x + 4;
      player_x =  new_player_x ;

      time = millis();
      //delay(300);
    }
    if (moveToLeft == 0 && millis() - time > debounce) {
      Serial.println("player move left");
      before_player_x = player_x;
      new_player_x = player_x - 4 ;
      player_x =  new_player_x ;
      time = millis();
      //delay(300);
    }
    //เช็คเงื่อนไข ไม่ให้ play ตกขอบขวา
    if (player_x + player_width > 127) {
      player_x = 127 - player_width;
    }
    //เช็คเงื่อนไขไม่ให้ player ตกขอบซ้าย
    if (player_x < 1) {
      player_x = 1;
    }

    //for the ball
    if (millis() - ball_time > 35) {
      new_ball_x = ball_x - ball_change_x ;
      new_ball_y = ball_y + ball_change_y;
      //ถ้าบอลตกลงมาแล้วโดน player จะเพิ่ม score แล้วเด้งบอลกลับ มีเสียงเพลงด้วย
      if (new_ball_y >= player_y && new_ball_x >= player_x && new_ball_x <= player_x + player_width )
      {
        ball_change_y = -(ball_change_y);
        new_ball_y += ball_change_y;
        score += 1;
        soundPoint();
      }
      //ถ้าบนชนหลังคาจะเด้งกลับลงมา
      if (new_ball_y <= 1) {
        ball_change_y = -(ball_change_y);
        ball_change_x = -(ball_change_x);
        soundHitWall();
      }
      //ถ้าชนขอบซ้ายหรือขวาจะเด้งกลับ
      if (new_ball_x <= 1 || new_ball_x >= 127) {
        ball_change_x = -(ball_change_x);
        soundHitWall();
      }
    }

    //ถ้าบอลตกลงไปถึงขอบล่างจะ game over
    if (new_ball_y >= 31)
    {
      endGame();
    }
    ball_time = millis();
    //draw the ball movement
    OLED.drawPixel(ball_x, ball_y, BLACK);
    delay(50);
    OLED.drawPixel(new_ball_x, new_ball_y, WHITE);

  }

  new_score = score;
  ball_x = new_ball_x;
  ball_y = new_ball_y;

  //draw player movement
  OLED.drawFastHLine(before_player_x, player_y, player_width, BLACK);
  OLED.drawFastHLine(player_x, player_y, player_width, WHITE); //สําหรับวาดกระดานเพื่อใช้เป็นตัวผู้เล่น

  OLED.display();
}
void drawArea() {
  OLED.drawRect(0, 0, 128, 32, WHITE);
}
void endGame() {
  OLED.fillScreen(WHITE);
  OLED.setCursor(20, 4);
  OLED.setTextColor(BLACK);
  OLED.setTextSize(1);
  OLED.print("GAME OVER");

  OLED.setCursor(20, 15);
  OLED.setTextColor(BLACK);
  OLED.setTextSize(1);
  OLED.print("Your score = ");
  OLED.print(new_score);
  OLED.display();
  Serial.println("the game is over");
  stop_the_game = true;
}

void soundPoint() 
{
//  digitalWrite(buzzer,HIGH);
//  
//  digitalWrite(buzzer,LOW);
 tone( buzzer, 250, 50);
}

void soundHitWall(){
  tone(buzzer, 500, 50);
}
