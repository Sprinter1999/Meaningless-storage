#include <Servo.h>

char _ABVAR_1_input = ' ' ;
Servo servo_pin_12;
Servo servo_pin_9;

void setup()
{
  Serial.begin(9600);
  servo_pin_12.attach(12);
  servo_pin_9.attach(9);
}

void loop()
{
  if (( ( Serial.available() ) == ( 1 ) ))
  {
    _ABVAR_1_input = Serial.read();
  }
  if (( ( _ABVAR_1_input ) == ('1') ))
  {
    servo_pin_12.write( 90 );
    delay( 1000 );
    servo_pin_9.write( 150 );
    delay( 1000 );
    servo_pin_9.write( 0 );
    delay( 1000 );
    servo_pin_12.write( 160 );
    delay( 1000 );
  }
  if (( ( _ABVAR_1_input ) == ('0') ))
  {
    servo_pin_12.write( 90 );
    delay( 1000 );
  }
}


