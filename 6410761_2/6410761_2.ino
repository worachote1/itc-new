// C++ code

int red;
int yellow;
int green;

long redtime, greentime;

int stage = LOW, stage_g = LOW;
int pre_red = LOW, pre_green = HIGH;
long debounce = 250, time = 0, time_g = 0;


void setup()
{
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(12, INPUT);
  pinMode(11, INPUT);
  pinMode(10, INPUT_PULLUP);
}

void loop()
{
  red = digitalRead(12);
  yellow = digitalRead(11);
  green = digitalRead(10);
  
  if (red == HIGH && pre_red == LOW && millis() - time > debounce)
  {
    redtime = millis();
    if (stage == LOW)
      stage = HIGH;
    else
      stage = LOW;
    time = millis();
    digitalWrite(4, stage);
  }
  pre_red = red;

  if (millis() == redtime + 3000)
  {
    stage = LOW;
    digitalWrite(4, stage);
  }

  if (green == LOW && pre_green == HIGH && stage == LOW && millis() - time_g > debounce)
  {
    greentime = millis();
    if (stage_g == LOW)
      stage_g = HIGH;
    else
      stage_g = LOW;
    time_g = millis();
    digitalWrite(2, stage_g);
  }
  pre_green = green;
  if (millis() == greentime + 3000)
  {
    stage_g = LOW;
    digitalWrite(2, stage_g);
  }

  if (yellow == LOW && stage_g == LOW && stage == LOW)
  {
    for (int i = 1; i <= 4; i++) {
      digitalWrite(3, HIGH);
      delay(250);
      digitalWrite(3, LOW);
      delay(250);
    }
  }

}
