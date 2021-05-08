
#define TRIGGER 5
#define ECHO 6
#define BUZZER 9
 
void setup() {
  // Iniciamos el monitor serie
  Serial.begin(9600);
  
  pinMode(ECHO, INPUT);
  pinMode(TRIGGER, OUTPUT);
  pinMode(BUZZER, OUTPUT);
}
 
void loop() {
  // Preparamos el sensor de ultrasonidos
  iniciarTrig();
 
  // Obtenemos la distancia
  float distancia = ObtenerDist();
 
  if (distancia <= 60)
  {
    //Activamos los pitidos
    pitido(distancia);
  } 
}

void pitido(float distancia)
{
  //La frecuencia tiene una relación lineal con la distancia
  float frecuencia = -7.5 * distancia + 900;
  float duracion = 5 * distancia + 200;        //La duración también tiene una relación lineal con la distancia
  tone(BUZZER,frecuencia , duracion); 
}
//Estas relaciones lineales las hemos obtenido para que se ajusten a la capacidad de los sensores y a nuestras preferencias
 

// Funcion que inicia la secuencia del Trigger 
void iniciarTrig()
{
  // Ponemos el Trigger en estado bajo y esperamos 2 ms
  digitalWrite(TRIGGER, LOW);
  delayMicroseconds(2);
 
  // Ponemos el pin Trigger a estado alto y esperamos 10 ms
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10);
 
  // Terminamos poniendo el pin Trigger en estado bajo
  digitalWrite(TRIGGER, LOW);
}


float ObtenerDist() // Funcion que devuelve una variable tipo float que contiene la distancia
{
  unsigned long tiempo = pulseIn(ECHO, HIGH);//Mide el tiempo que tarda en recibir la onda el transductor ECHO
 
  // Obtenemos la distancia en cm
  float distancia = tiempo * 0.01715;  //La distancia es igual al tiempo por la velocidad del sonido en cm por microsegundo
  Serial.print(distancia); //dado que "pulse in" mide el tiempo en microsegundos
  Serial.print("cm");
  Serial.println();
  delay(500);
 
  return distancia;
}
 
