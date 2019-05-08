#include <Servo.h>
#include <EEPROM.h>

int INPUT1 = 8;  
int INPUT2 = 7; 
int INPUT3 = 12; 
int INPUT4 = 13; 
int Input_Detect_Left = A3;
int Input_Detect_Right = A2;
int Echo = A5;
int Trig = A4;
float Ldistance;
int count = 0;
int v1 = 0;
int v2 = 0;
int v3 = 0;
#define MOTOR_GO_LEFT           \
  {                             \
    digitalWrite(INPUT1, LOW);  \
    digitalWrite(INPUT2, HIGH); \
    digitalWrite(INPUT3, LOW);  \
    digitalWrite(INPUT4, HIGH); \
  } //³µÌåÇ°½ø
#define MOTOR_GO_RIGHT          \
  {                             \
    digitalWrite(INPUT1, HIGH); \
    digitalWrite(INPUT2, LOW);  \
    digitalWrite(INPUT3, HIGH); \
    digitalWrite(INPUT4, LOW);  \
  } //³µÌåÇ°½ø

#define MOTOR_GO_FORWARD        \
  {                             \
    digitalWrite(INPUT1, LOW);  \
    digitalWrite(INPUT2, HIGH); \
    digitalWrite(INPUT3, HIGH); \
    digitalWrite(INPUT4, LOW);  \
  } //³µÌåÇ°½ø
#define MOTOR_GO_STOP          \
  {                            \
    digitalWrite(INPUT1, LOW); \
    digitalWrite(INPUT2, LOW); \
    digitalWrite(INPUT3, LOW); \
    digitalWrite(INPUT4, LOW); \
  } //³µÌåÇ°½ø
#define MOTOR_GO_BACK           \
  {                             \
    digitalWrite(INPUT1, HIGH); \
    digitalWrite(INPUT2, LOW);  \
    digitalWrite(INPUT3, LOW);  \
    digitalWrite(INPUT4, HIGH); \
  } //³µÌåÇ°½ø
int rec_taken = 0;
int index = 7;
char rec;
int IR_R;
int IR_L;
int Left_Speed_Hold = 200;  
int Right_Speed_Hold = 200; 
int ENA = 5;                
int ENB = 6;              
int follow_begin = 0;
int Left_Speed[8] = {
    60, 65,70,75,80,85,90,95}; 
int Right_Speed[8] = {
    60, 65,70,75,80,85,90,95}; 
void FollowLine()
{
  index = 7;
  Left_Speed_Hold = Left_Speed[index];
  Right_Speed_Hold = Right_Speed[index];
  analogWrite(ENB, Left_Speed_Hold);  
  analogWrite(ENA, Right_Speed_Hold); 
  IR_L = digitalRead(Input_Detect_Left);
  IR_R = digitalRead(Input_Detect_Right);
  if ((IR_L == LOW) && (IR_R == LOW))
  {
    MOTOR_GO_FORWARD;
    return;
  }
  if ((IR_L == LOW) && (IR_R == HIGH))
  {
    MOTOR_GO_RIGHT;
    return;
  }
  if ((IR_L == HIGH) && (IR_R == LOW))
  {
    MOTOR_GO_LEFT;
    return;
  }
  if ((IR_L == HIGH) && (IR_R == HIGH))
  {
    MOTOR_GO_STOP;
    return;
  }
}
void GetMessage()
{
  if (Serial.available())
  {
    rec = Serial.read();
    Serial.println(rec);
    rec_taken = 0;
  }
  Left_Speed_Hold = Left_Speed[index];
  Right_Speed_Hold = Right_Speed[index];
  analogWrite(ENB, Left_Speed_Hold);  //¸øL298Ê¹ÄÜ¶ËB¸³Öµ
  analogWrite(ENA, Right_Speed_Hold); //¸øL298Ê¹ÄÜ¶ËA¸³Öµ
  switch (rec)
  {
  case 'a':
    MOTOR_GO_LEFT;
    return;
  case 'd':
    MOTOR_GO_RIGHT;
    return;
  case 'w':
    MOTOR_GO_FORWARD;
    return;
  case 's':
    MOTOR_GO_STOP;
    return;
  case 'b':
    MOTOR_GO_BACK;
    return;
  case '+':
    if ((((rec_taken) == (0)) && ((index) < (10))))
    {
      index++;
      rec_taken = 1;
      return;
    }
  case '-':
    if ((((rec_taken) == (0)) && ((index) > (0))))
    {
      index--;
      rec_taken = 1;
      return;
    }
  case '1':
    follow_begin = 1;
    if (rec_taken == 0)
      Serial.println("follow begin"), rec_taken = 1;
    break;
  case '2':
    follow_begin = 0;
    if (rec_taken == 0)
      Serial.println("follow end"), rec_taken = 1;
    break;
  case 'p':
  {
    if (rec_taken == 0)
    {
      Serial.print("left: ");
      Serial.println(IR_L);
      Serial.print("right: ");
      Serial.println(IR_R);
      rec_taken = 1;
    }
    break;
  }
  case 'o':
  {
    if (rec_taken == 0)
    {
      Serial.print("distance: ");
      Serial.println(Ldistance);
      rec_taken = 1;
    }
    break;
  }
  default:
    break;
  }
}

char getdistance()
{
  digitalWrite(Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);
  Ldistance = pulseIn(Echo, HIGH);
  Ldistance = Ldistance / 5.8 / 3;
  Serial.println(Ldistance);
  return Ldistance;
}
void setup()
{
  pinMode(INPUT1, OUTPUT);
  pinMode(INPUT2, OUTPUT);
  pinMode(INPUT3, OUTPUT);
  pinMode(INPUT4, OUTPUT);
  Serial.begin(9600);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(Input_Detect_Left, INPUT);
  pinMode(Input_Detect_Right, INPUT);
  pinMode(Echo, INPUT);
  pinMode(Trig, OUTPUT);
}

void loop()
{
  index = 3;
  Left_Speed_Hold = Left_Speed[index];
  Right_Speed_Hold = Right_Speed[index];
  analogWrite(ENB, Left_Speed_Hold); 
  analogWrite(ENA, Right_Speed_Hold); 

  if (getdistance() <= 20)
  {
    MOTOR_GO_STOP;
  }
  else
    MOTOR_GO_FORWARD;
}
