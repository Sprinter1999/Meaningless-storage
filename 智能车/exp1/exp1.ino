#include <Servo.h>
#include <EEPROM.h>

int rec_taken = 0 ;
int index = 0 ;
int INPUT1 = 8;//电机接口1
int INPUT2 = 7;//电机接口2
int INPUT3 = 12;//电机接口3
int INPUT4 = 13;//电机接口4
#define MOTOR_GO_FORWARD {digitalWrite(INPUT1,LOW);digitalWrite(INPUT2,HIGH);digitalWrite(INPUT3,HIGH);digitalWrite(INPUT4,LOW);}    //车体前进
#define MOTOR_GO_BACK {digitalWrite(INPUT1,HIGH);digitalWrite(INPUT2,LOW);digitalWrite(INPUT3,LOW);digitalWrite(INPUT4,HIGH);}    //车体前进
#define MOTOR_GO_RIGHT    {digitalWrite(INPUT1,HIGH);digitalWrite(INPUT2,LOW);digitalWrite(INPUT3,HIGH);digitalWrite(INPUT4,LOW);}    //车体前进

#define MOTOR_GO_LEFT {digitalWrite(INPUT1,LOW);digitalWrite(INPUT2,HIGH);digitalWrite(INPUT3,LOW);digitalWrite(INPUT4,HIGH);}    //车体前进
char rec;
int IR_L;
int IR_R;
float Ldistance;
int Left_Speed_Hold = 200;//定义左侧速度变量
int Right_Speed_Hold = 200;//定义右侧速度变量
int ENA = 5;//L298使能A
int ENB = 6;//L298使能B
int follow_begin = 0;
int Left_Speed[8]={
  10,20,30,40,50,55,60,65};//左侧速度档位值
int Right_Speed[8]={
  10,20,30,40,50,55,60,65};//右侧速度档位值
void GetMessage(){
  if(Serial.available()){
    rec = Serial.read();
    Serial.println(rec);
    rec_taken = 0;
  }
  Left_Speed_Hold = Left_Speed[index];
  Right_Speed_Hold = Right_Speed[index];
  analogWrite(ENB,Left_Speed_Hold);//给L298使能端B赋值
  analogWrite(ENA,Right_Speed_Hold);//给L298使能端A赋值 
  switch(rec){
  case '+':
    if (( ( ( rec_taken ) == ( 0 ) ) && ( ( index ) < ( 10 ) ) ))
    {
      index++;
      rec_taken = 1 ;
      return;
    }
  case '-':
    if (( ( ( rec_taken ) == ( 0 ) ) && ( ( index ) > ( 10 ) ) ))
    {
      index--;
      rec_taken = 1 ;
      return;
    }
  case 'w':
    MOTOR_GO_FORWARD;
    delay( 300 );
    return;
  case 's':
    MOTOR_GO_BACK;
    delay( 300 );
    return;
  case 'a':
    MOTOR_GO_RIGHT;
    delay( 300 );
    return;
  case 'd':
    MOTOR_GO_LEFT;
    delay( 300 );
    return;    
  case '1':  
    follow_begin = 1; 
    if(rec_taken == 0) Serial.println("follow begin"),rec_taken = 1; 
    break;
  case '2':  
    follow_begin = 0; 
    if(rec_taken == 0) Serial.println("follow end"),rec_taken = 1; 
    break;
  case 'p':
    {
      if(rec_taken == 0)
      {
        Serial.print("left: ");
        Serial.println(IR_L);
        Serial.print("right: ");
        Serial.println(IR_R);
        rec_taken = 1;
      }
      break;
    }
  case'o':
    {
      if(rec_taken == 0)
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

void setup()
{
  pinMode(INPUT1,OUTPUT);
  pinMode(INPUT2,OUTPUT);
  pinMode(INPUT3,OUTPUT);
  pinMode(INPUT4,OUTPUT);
  Serial.begin(9600);
  pinMode(ENA,OUTPUT);
  pinMode(ENB,OUTPUT);
}

void loop()
{
  GetMessage();
}
