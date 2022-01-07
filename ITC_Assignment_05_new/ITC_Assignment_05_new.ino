// C++ code
//#define
//for people
int pG = 4 ;
int pR = 5 ;

//for south
int  sG =  6 ;
int sY =  7 ;
int sR =  8 ;

//for west
int  wG = 9 ;
int  wY = 10 ;
int wR  = 11 ;

//switch for west
int  swW = 12 ; 
//switch for south
int  swS = 2 ;
//switch for people
int swP = 3 ;

//state
int  waitwest = 1 ;
int west =  2 ;
int waitsouth =  3 ;
int  south =  4 ;
int   waitwalk = 5 ;
int  walk  = 6 ;


class LedTraffic
{
  public:
    int commandlist[6][8] =
    {
  //  0         1          2           3           4              5              6              7
      //south 4
      {south    , waitwest , south     , waitwest  , waitwalk     , waitwalk     , waitwalk     , waitwalk},
      //waitsouth 3
      {south    , south    , south     , south     , waitwalk     , waitwalk     , waitwalk     , waitwalk},
      //west 2
      {west     , west     , waitsouth , waitsouth , waitwalk     , waitwalk     , waitwalk     , waitwalk},
      //waitwest 1
      {west     , west     , west      , west      , waitwalk     , waitwalk     , waitwalk     , waitwalk},
      //walk 6
      {walk    , west     , south     , south     , walk         , walk         , walk         , walk},
      //waitwalk 5
      {walk     , walk     , walk      , walk      , walk         , walk         , walk         , walk}
    };
    int command = south;

    int stat = 0;
    //strcpy (command,String(commandlist[1][1]));
    //memcpy(command,command, sizeof(command));

    int oldcommand = south;
    int delaytime = 2000;

    void Control()
    {
      //Serial.println(command);
      if (command == west)
      {
        led("green", "red", "red");
        delaytime = 2000;
      }
      if (command == waitwest)
      {
        led("red", "yellow", "red");
        delaytime = 500;
      }

      if (command == south)
      {
        led("red", "green", "red");
        delaytime = 2000;
      }
      if (command == waitsouth)
      {
        led("yellow", "red", "red");
        delaytime = 500;
      }

      if (command == walk)
      {
        if (stat == 0)
        {
          led("red", "red", "green");
          delaytime = 1000;
        }
        if (stat == 1)
        {
          led("red", "red", "off");
          delaytime = 200;
        }
        if (stat == 2)
        {
          led("red", "red", "green");
          delaytime = 200;
        }
        if (stat == 3)
        {
          led("red", "red", "off");
          delaytime = 200;
        }
        if (stat == 4)
        {
          led("red", "red", "green");
          delaytime = 200;
        }
        if (stat == 5)
        {
          led("red", "red", "off");
          delaytime = 200;
        }
        if (stat == 6)
        {
          led("red", "red", "green");
          delaytime = 200;
        }

      }
      if (command == waitwalk)
      {
        
        if (oldcommand == south)
        {
          Serial.println(oldcommand);
          led("red", "yellow", "red");
        }
        else if (oldcommand == west)
        {
          led("yellow", "red", "red");
        }
        else
        {
          led("red", "red", "red");
        }
        delaytime = 500;
      }
    }

    int commandtoInt()
    {
      //char scommand[20]=command;
      //char scommand[20]=String(command);
      if (command == south)
      {
        return 0;
      }
      if (command == waitsouth)
      {
        return 1;
      }
      if (command == west)
      {
        return 2;
      }
      if (command == waitwest)
      {
        return 3;
      }
      if (command == walk)
      {
        return 4;
      }
      if (command == waitwalk)
      {
        return 5;
      }
      else
      {
        return 99;
      }
    }
    int flip(int num)
    {
      if (num == 1)
        return 0;
      else
        return 1;
    }
    void SwtoCommand()
    {
      //Serial.println(stat);
      int westsw = digitalRead(swW);
      int southsw = digitalRead(swS);
      int walksw = digitalRead(swP);

      walksw = flip(walksw);
      southsw = flip(southsw);
      westsw = flip(westsw);

//      Serial.print("s:");
//      Serial.println(southsw );
//      Serial.print("w:");
//      Serial.println(westsw);
//      Serial.print("p:");
//      Serial.println(walksw );
//      Serial.print("sum:");
//      Serial.println(southsw + westsw * 2 + walksw * 4);
//      Serial.println();

      if (stat < 7 && command == walk && walksw == 0)
      {
        stat++;
      }
      else if (walksw == 1 && command == walk)
      {
        stat = 0;
      }
      else
      {
        oldcommand = command;
        command = commandlist[commandtoInt()][southsw + westsw * 2 + walksw * 4];
        stat = 0;
      }
    }

    void led(char westl[10], char southl[10], char walkl[10])
    {
      if (walkl == "red")
      {
        digitalWrite(pG, 0);
        digitalWrite(pR, 1);
      }
      else if (walkl == "off")
      {
        digitalWrite(pG, 0);
        digitalWrite(pR, 0);
      }
      else
      {
        digitalWrite(pG, 1);
        digitalWrite(pR, 0);
      }
      if (westl == "red")
      {
        digitalWrite(wG, 0);
        digitalWrite(wY, 0);
        digitalWrite(wR, 1);
      }
      else if (westl == "yellow")
      {
        digitalWrite(wG, 0);
        digitalWrite(wY, 1);
        digitalWrite(wR, 0);
      }
      else
      {
        digitalWrite(wG, 1);
        digitalWrite(wY, 0);
        digitalWrite(wR, 0);
      }
      if (southl == "red")
      {
        digitalWrite(sG, 0);
        digitalWrite(sY, 0);
        digitalWrite(sR, 1);
      }
      else if (southl == "yellow")
      {
        digitalWrite(sG, 0);
        digitalWrite(sY, 1);
        digitalWrite(sR, 0);
      }
      else
      {
        digitalWrite(sG, 1);
        digitalWrite(sY, 0);
        digitalWrite(sR, 0);
      }
    }
};
void setup() {
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(2, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);

  Serial.begin(9600);
}

LedTraffic play;

void loop()
{
  play.Control();
  delay(play.delaytime);
  play.SwtoCommand();
}
