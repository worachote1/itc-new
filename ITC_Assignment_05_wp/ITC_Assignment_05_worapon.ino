#define LED_W_R 5
#define LED_W_Y 6
#define LED_W_G 7
#define WEST_BUTTON_PIN 2
#define LED_S_R 8
#define LED_S_Y 9
#define LED_S_G 10
#define SOUTH_BUTTON_PIN 3
#define LED_WALK_R 12
#define LED_WALK_G 11
#define WALK_BUTTON_PIN 4
#define goS 0
#define waitS 1
#define goW 2
#define waitW 3
#define goWalk 4
#define waitWalk 5

struct State {
unsigned long ST_Out; // 6-bit pattern to street output 
unsigned long Time; // delay in ms units
unsigned long Next[8];}; // next state for inputs 0,1,2,3
typedef const struct State SType;
SType FSM[6]={
{B10100001,3000,{goS,waitS,goS,waitS,waitS,waitS,waitS,waitS}},
{B10100010,1000,{goW,goWalk,goW,goWalk,goW,goW,goW,goW}},
{B10001100,3000,{goW,waitW,waitW,waitW,goW,waitW,waitW,waitW}},
{B10010100,1000,{goWalk,goWalk,goS,goWalk,goWalk,goWalk,goS,goWalk}},
{B01100100,3000,{goWalk,goWalk,waitWalk,waitWalk,waitWalk,waitWalk,waitWalk,waitWalk}},
{B01100100,100,{goS,goS,goS,goS,goW,goW,goS,goS}}
};
unsigned long S=0; // index to the current state 
void setup() {
pinMode(LED_W_R, OUTPUT);
pinMode(LED_W_Y, OUTPUT);
pinMode(LED_W_G, OUTPUT); 
pinMode(WEST_BUTTON_PIN, INPUT); 
pinMode(LED_S_R, OUTPUT);
pinMode(LED_S_Y, OUTPUT);
pinMode(LED_S_G, OUTPUT); 
pinMode(SOUTH_BUTTON_PIN, INPUT); 
pinMode(LED_WALK_R, OUTPUT);
pinMode(LED_WALK_G, OUTPUT);
pinMode(WALK_BUTTON_PIN, INPUT);
Serial.begin(9600);
}
int input,input1, input2, input3;
void loop() {
digitalWrite(LED_W_R, FSM[S].ST_Out & B00100000);
digitalWrite(LED_W_Y, FSM[S].ST_Out & B00010000);
digitalWrite(LED_W_G, FSM[S].ST_Out & B00001000);
digitalWrite(LED_S_R, FSM[S].ST_Out & B00000100);
digitalWrite(LED_S_Y, FSM[S].ST_Out & B00000010);
digitalWrite(LED_S_G, FSM[S].ST_Out & B00000001);
  if(S==5){
    for(int i = 0; i<3; i++){
    digitalWrite(LED_WALK_G, FSM[S].ST_Out & B00000000);
    delay(150);
  digitalWrite(LED_WALK_G, FSM[S].ST_Out & B01000000);
    delay(150);    
    }
  }
  else
    digitalWrite(LED_WALK_G, FSM[S].ST_Out & B01000000);
digitalWrite(LED_WALK_R, FSM[S].ST_Out & B10000000);
delay(FSM[S].Time);
input1 = digitalRead(WEST_BUTTON_PIN); Serial.println(input1);
input2 = digitalRead(SOUTH_BUTTON_PIN);
input3 = digitalRead(WALK_BUTTON_PIN);
input = input1*4+input2*2+input3;
S = FSM[S].Next[input]; 
}
