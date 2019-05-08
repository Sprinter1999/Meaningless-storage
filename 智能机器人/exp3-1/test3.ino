#define Do  262
#define Re  294
#define Mi  330
#define Fa  350
#define Sol 393
#define La  441
#define Si  495
#define Doo 882
int buzzer=8;
int len=8;
int scale[]={Do,Re,Mi,Fa,Sol,La,Si,Doo};
float duration[]={2,1,1,1,2,1,1,1};

void setup() {
  pinMode(buzzer,OUTPUT);
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
