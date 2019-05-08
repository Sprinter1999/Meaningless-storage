#include <Servo.h>

char _ABVAR_1_input = ' ' ;
Servo servo_pin_9;
Servo servo_pin_11;
Servo servo_pin_12;
Servo servo_pin_13;

void setup()
{
  Serial.begin(9600);
  servo_pin_9.attach(9);
  servo_pin_11.attach(11);
  servo_pin_12.attach(12);
  servo_pin_13.attach(13);
}

void loop()
{
  if (( ( Serial.available() ) == ( 1 ) ))
  {
    _ABVAR_1_input = Serial.read();
  }
  if (( ( _ABVAR_1_input ) == ('0') ))
  {
    servo_pin_9.write( 0 );
  }
  if (( ( _ABVAR_1_input ) == ('1') ))
  {
    servo_pin_9.write( 90 );
  }
  if (( ( _ABVAR_1_input ) == ('2') ))
  {
    servo_pin_11.write( 0 );
  }
  if (( ( _ABVAR_1_input ) == ('3') ))
  {
    servo_pin_11.write( 90 );
  }
  if (( ( _ABVAR_1_input ) == ('4') ))
  {
    servo_pin_12.write( 0 );
  }
  if (( ( _ABVAR_1_input ) == ('5') ))
  {
    servo_pin_12.write( 90 );
  }
  if (( ( _ABVAR_1_input ) == ('6') ))
  {
    servo_pin_13.write( 0 );
  }
  if (( ( _ABVAR_1_input ) == ('7') ))
  {
    servo_pin_13.write( 90 );
  }
}


