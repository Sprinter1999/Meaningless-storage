char _ABVAR_1_input = ' ' ;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  if (( ( Serial.available() ) == ( 1 ) ))
  {
    _ABVAR_1_input = Serial.read();
  }
  if (( ( _ABVAR_1_input ) == ('1') ))
  {
    tone(8, 262, 1000);
  }
  if (( ( _ABVAR_1_input ) == ('2') ))
  {
    tone(8, 294, 1000);
  }
  if (( ( _ABVAR_1_input ) == ('3') ))
  {
    tone(8, 330, 1000);
  }
  if (( ( _ABVAR_1_input ) == ('4') ))
  {
    tone(8, 349, 1000);
  }
  if (( ( _ABVAR_1_input ) == ('5') ))
  {
    tone(8, 392, 1000);
  }
  if (( ( _ABVAR_1_input ) == ('6') ))
  {
    tone(8, 440, 1000);
  }
  if (( ( _ABVAR_1_input ) == ('7') ))
  {
    tone(8, 494, 1000);
  }
}


