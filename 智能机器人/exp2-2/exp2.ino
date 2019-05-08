char _ABVAR_1_input = ' ' ;
void __ardublockDigitalWrite(int pinNumber, boolean status)
{
  pinMode(pinNumber, OUTPUT);
  digitalWrite(pinNumber, status);
}


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
  if (( ( _ABVAR_1_input ) == ('A') ))
  {
    __ardublockDigitalWrite(13, HIGH);
  }
  if (( ( _ABVAR_1_input ) == ('B') ))
  {
    __ardublockDigitalWrite(13, LOW);
  }
}


