int LdrPin = A0;
int ledPin=16;//D0
int sensorValue=0;
void setup()
{
  Serial.begin(9600);
  pinMode( ledPin, OUTPUT );
}
void loop()
{
  sensorValue=analogRead(LdrPin);
  Serial.println(sensorValue);
  if(sensorValue<300)
  {
    digitalWrite(ledPin, HIGH); 
    Serial.println("Light On");
  }
  else
  {
    digitalWrite(ledPin, LOW);
    Serial.println("Light Off");                                           
  }
}
