#include <Servo.h>

Servo servo_pin_9;
Servo servo_pin_11;
Servo servo_pin_12;
Servo servo_pin_13;

void setup()
{
  servo_pin_9.attach(9);
  servo_pin_11.attach(11);
  servo_pin_12.attach(12);
  servo_pin_13.attach(13);
}

void loop()
{
  if (( ( 1 ) == ( 1 ) ))
  {
    servo_pin_9.write( 0 );
    delay( 1000 );
    servo_pin_9.write( 90 );
    delay( 1000 );
    servo_pin_11.write( 0 );
    delay( 1000 );
    servo_pin_11.write( 90 );
    delay( 1000 );
    servo_pin_12.write( 0 );
    delay( 1000 );
    servo_pin_12.write( 90 );
    delay( 1000 );
    servo_pin_13.write( 0 );
    delay( 1000 );
    servo_pin_13.write( 90 );
    delay( 1000 );
  }
}


