#define TRIGGER 5
#define ECHO 6
#define BUZZER 9

void setup() {
  
  Serial.begin(9600);
  
  pinMode(ECHO, INPUT);                               
  pinMode(TRIGGER, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  char cdistancia[7];
}
 
void loop() {
 
  iniciarTrig();                                           // Preparamos el sensor de ultrasonidos
 
  
  float distancia = ObtenerDist();                         // Obtenemos la distancia
 
  if (distancia <= 60)
  {
    
    pitido(distancia);                                     //Activamos los pitidos
  } 
}

void pitido(int distancia,char cdistancia)
{
  sprintf(cdistancia, "%d", distancia);                     //Paso la distancia a una cadena para enviarla al visual
  for (int i=0;i<str.length();i++)                          //Recorrido de la cadena
 {
 Serial.print(str.charAt(i));                               //Envío carácter a carácter
 delay(50);
 }
 Serial.println();                               
  
  float frecuencia = -7.5 * distancia + 900;                //La frecuencia tiene una relación lineal con la distancia
  float duracion = 5 * distancia + 200;                     //La duración también tiene una relación lineal con la distancia
  tone(BUZZER,frecuencia , duracion); 
}
//Estas relaciones lineales las hemos obtenido para que se ajusten a la capacidad de los sensores y a nuestras preferencias
 

// Funcion que inicia la secuencia del Trigger 
void iniciarTrig()
{
  
  digitalWrite(TRIGGER, LOW);                              // Ponemos el Trigger en estado bajo y esperamos 2 ms
  delayMicroseconds(2);
 
  
  digitalWrite(TRIGGER, HIGH);                             // Ponemos el pin Trigger a estado alto y esperamos 10 ms
  delayMicroseconds(10);
 
 
  digitalWrite(TRIGGER, LOW);                              // Terminamos poniendo el pin Trigger en estado bajo
}


int ObtenerDist()                                          // Funcion que devuelve una variable tipo float que contiene la distancia
{
  unsigned long tiempo = pulseIn(ECHO, HIGH);              //Mide el tiempo que tarda en recibir la onda el transductor ECHO
  // Obtenemos la distancia en cm
  int distancia = tiempo * 0.01715;                        //La distancia es igual al tiempo por la velocidad del sonido en cm por microsegundo
 
  return distancia;
}
