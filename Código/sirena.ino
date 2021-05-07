#define rojo 9
#define azul 8
#define zumb 7
void setup()
{
  pinMode(rojo,OUTPUT);
  pinMode(azul,OUTPUT);
  pinMode(zumb,OUTPUT);
}
void loop()
{
  digitalWrite(azul,HIGH);
  digitalWrite(rojo,LOW);
  tone(zumb,500);
  delay(250);
  tone(zumb,1000);
  digitalWrite(azul,LOW);
  digitalWrite(rojo,HIGH);
  delay(250);
}
