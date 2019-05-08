#define Do  262
#define Re  294
#define Mi  330
#define Fa  350
#define Sol 393
#define La  441
#define Si  495
#define Doo 882 

//音名记法
#define C  262
#define D  294
#define E  330
#define F  350
#define G 393
#define A  441
#define B  495
#define CC 525
#define DD 589
#define EE 661
#define AA 882
/**
 * 1=C
 */

int buzzer=8; //蜂鸣器口，PWM
int scale[]={G,A,EE,A,G,A,G,A,EE,A,G,
             A,EE,A,G,A,E,
             G,D,E,G,A,B,
             A,EE,A,G,A,G,
             A,EE,B,CC,B,CC,B,A,E,
             D,E,G,A,B,A,EE,A,G,A,
             G,A,EE,A,G,A,EE,A,G,A,
             E,G,D,E,G,A,B,A,EE,A,G,A,
             G,A,EE,B,CC,B,CC,DD,EE,AA};  //旋律
float duration[]={2,1,1,1,1,7,1,1,1,1,1,
                  1,1,1,1,3,1,
                  3,1,1,1,1,1,
                  1,1,1,1,7,1,
                  1,1,1,1,1,1,1,1,6,
                  1,1,1,1,1,1,1,1,1,7,
                  1,1,1,1,1,1,1,1,1,3,
                  1,3,1,1,1,1,1,1,1,1,1,7,
                  1,1,1,1,1,1,1,1,1,4}; //音符时值 
int len=0;  //该变量后面用于统计音符个数               
void setup() {
  pinMode(buzzer,OUTPUT);
  len=sizeof(scale)/sizeof(scale[0]);
}

void loop() {
  for(int i=0;i<len;i++){ 
    tone(buzzer,scale[i]);
    delay(250*duration[i]);
    noTone(buzzer);
    delay(100);
  }
  delay(1000);

}


