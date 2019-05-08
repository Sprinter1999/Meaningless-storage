char _ABVAR_1_Char = ' ' ;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  while ( ( ( Serial.available() ) == ( 0 ) ) )
  {
  }

  _ABVAR_1_Char = Serial.read();
  Serial.print(_ABVAR_1_Char);
  Serial.println();
}


