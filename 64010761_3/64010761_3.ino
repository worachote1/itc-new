//[9] for handle display of number 1,2,...,9
//[7] for 1,0 of each segment
boolean handle_led [9][7] = {
  1, 0, 0, 1, 1, 1, 1
  , 0, 0, 1, 0, 0, 1, 0
  , 0, 0, 0, 0, 1, 1, 0
  , 1, 0, 0, 1, 1, 0, 0
  , 0, 1, 0, 0, 1, 0, 0
  , 0, 1, 0, 0, 0, 0, 0
  , 0, 0, 0, 1, 1, 0, 1
  , 0, 0, 0, 0, 0, 0, 0
  , 0, 0, 0, 0, 1, 0, 0
};
//define button
int btn_a, btn_b;
long debounce = 200 , time = 0;
//row : [0] = equal,[1] = greater , [2] = less
boolean compare_led [3][7] = {
  0, 0, 0, 0, 0, 0, 1
  , 0, 1, 0, 0, 0, 0, 1
  , 1, 1, 1, 0, 0, 0, 1
};

int random_value;

void setup() {
  Serial.begin(9600);
  //for 7 segment
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);

  //for button
  pinMode(12, INPUT); //button-A(pull down is 0 if not pressed)
  pinMode(11, INPUT); //button-B(pull down is 0 if not pressed)


  //for randomseed
  randomSeed(analogRead(A0));
  random_value = random(1, 10);

  //segment start
  for (int i = 2 ; i <= 9 ; i++ ) {

    digitalWrite(i , 1);
  }

}

//for count number in segment
int count = 0;

void loop() {
  //show led number 0 to 9 when press button-A
  btn_a = digitalRead(12);
  btn_b = digitalRead(11);

  if (btn_a == 0 && millis() - time > debounce)
  {
    /* for(int i=0 ; i<=8 ; i++)
          {
            display_led(i);
          }
    */
    if (count == 9) {
      count = 0;
    }
    display_led(count);
    count += 1;
    time = millis();
  }
  if (btn_b == 0)
  {
    //will get random value later....

    Serial.print("Random = ");
    Serial.print(random_value);
    Serial.print(" Count = ");
    Serial.println(count);
    delay(100);

    //ทายถูกเฉย!!
    if (random_value == (count)) {

      Serial.print("random_value equal segment of number = ");
      Serial.println(random_value);
      display_compare(0);
      count = 0;
      random_value = random(1, 10);
      delay(100);
    }
    //ทายแล้ว ได้มากกว่า เลขของsegment
    else if (random_value > (count)) {
      display_compare(1);
      delay(100);
    }
    //ทายแล้ว ได้น้อยกว่า เลขของsegment
    else if (random_value < (count)) {
      display_compare(2);
      delay(100);
    }
  }
}

void display_led(int n)
{
  for (int i = 2 ; i <= 8 ; i++ ) {

    digitalWrite(i , handle_led [n][i - 2]);
  }
  delay(100);
}

void display_compare(int m)
{
  for (int i = 2 ; i <= 8 ; i++ )
  {
    digitalWrite(i , compare_led [m][i - 2]);
  }
  delay(100);
}
