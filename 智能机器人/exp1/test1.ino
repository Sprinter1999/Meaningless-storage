void __ardublockDigitalWrite(int pinNumber, boolean status)
{
  pinMode(pinNumber, OUTPUT);
  digitalWrite(pinNumber, status);
}


void setup()
{
}

void loop()
{
  __ardublockDigitalWrite(13, HIGH);
  delay( 1000 );
  __ardublockDigitalWrite(13, LOW);
  delay( 1000 );
}


