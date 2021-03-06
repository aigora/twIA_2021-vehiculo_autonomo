#include <Servo.h>                // Se incluyen las librerías 
#include <LiquidCrystal.h> 


//sensor ultrasonido
#define TRIGGER 5
#define ECHO 6
#define BUZZER 9

// motor paso a paso
#define IN1  4
#define IN2  5
#define IN3  6
#define IN4  7
//
// Servomotor
#define servo 9
//
//bomba de agua
#define bomb 8 
//motor escalera
#define motor 10

#define rojo 9
#define azul 8
#define zumb 7
#define tiemp 250


Servo servo1;                                                 //Variables de control de los objetos
Servo servoMotor;                                   
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);           
  


void iniciarTrig();                                           //Prototipos de las funciones
int ObtenerDist();
void situardirecc();
int giro();



void setup()  { 

  
  int izqA = 5;              // Se definen las variables correspondientes a la dirección del vehículo
  int izqB = 6; 
  int derA = 9; 
  int derB = 10; 
 

  int ledderecha, ledizquierda;


  // CÓDIGO ASOCIADO AL MOVIMIENTO DEL VEHÍCULO
  
  pinMode(derA, OUTPUT);
  pinMode(derB, OUTPUT);
  pinMode(izqA, OUTPUT);
  pinMode(izqB, OUTPUT);
  
  pinMode(ECHO, INPUT);      // Se establece el pin 6 como entrada (echo) 
  pinMode(TRIGGER,OUTPUT);   // Se establece el pin 5 como salida (trigger)
  pinMode(13,OUTPUT);
  pinMode(BUZZER, OUTPUT);

  servo1.attach(11,660,1400);    // Se asocia el servo1 al pin 11, y se definen el mínimo y el máximo del ancho del pulso 




// CÓDIGO CORRESPONDIENTE A LOS INTERMITENTES

   pinMode(ledderecha, OUTPUT);    // Se definen los pines como salidas
   pinMode(ledizquierda, OUTPUT);    



 
 // CÓDIGO CORRESPONDIENTE A SISTEMA ESCALERA-MANGUERA
  
  // declaración de las salidas asignadas a la placa arduino
  //
  // motor paso a paso 
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  //
  // Servomotor
  servoMotor.attach(servo);
  // Inicializamos al ángulo 0 el servomotor
  //
  //bomba de agua
  pinMode(bomb,OUTPUT) ; 
  //motor escalera
  pinMode(motor,OUTPUT) ;




 // CÓDIGO CORRESPONDIENTE A SENSOR DE AGUA Y SIRENA
  
  pinMode(rojo,OUTPUT);
  pinMode(azul,OUTPUT);
  pinMode(zumb,OUTPUT);
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("capacidad:");
  lcd.setCursor(12,0);
  lcd.print("%");



 // CÓDIGO ASOCIADO AL SENSOR DE LLAMA

  int led = 13; // LED pin
  int digitalPin = 2; // KY-026 entrada digital 
  int zumbador = 9;// lectura análoga
  
  pinMode(led, OUTPUT);
  pinMode(zumbador, OUTPUT);
  pinMode(digitalPin, INPUT);

  
  Serial.begin(57600); 
  
} 






void loop()
{

  int izqA = 5;              // Se definen las variables correspondientes a la dirección del vehículo
  int izqB = 6; 
  int derA = 9; 
  int derB = 10; 
  int vel = 255;             // Hace referencia a la velocidad que tendrán los motores (0-255)
  char estado = 'c';         // Estado incial del vehículo (en reposo)
         
  int duracion, distancia; 


  
// CÓDIGO ASOCIADO AL MOVIMIENTO DEL VEHÍCULO


  if (Serial.available()>0)        // Intervienen el módulo Bluetooth y el puerto Serial
 {
      estado = Serial.read();
 }
      switch (estado) {
      case 'a':   // Se desplazará hacia delante
            analogWrite(derB, 0);     
            analogWrite(izqB, 0); 
            analogWrite(derA, vel);  
            analogWrite(izqA, vel);       
              break;
      case 'b': // Se desplazará a la izquierda
            analogWrite(derB, 0);
            analogWrite(izqB, 0);
            analogWrite(derA, 0);
            analogWrite(izqA, vel);
              break;
      case 'c':   // Se detendrá el vehículo
            analogWrite(derB, 0);     
            analogWrite(izqB, 0); 
            analogWrite(derA, 0);    
            analogWrite(izqA, 0); 
              break;
      case 'd': // Se desplazará hacia la derecha
            analogWrite(derB, 0);     
            analogWrite(izqB, 0);
            analogWrite(izqA, 0);
            analogWrite(derA, vel);  
              break;
      case 'e':   // Se desplazará hacia atrás
            analogWrite(derA, 0);    
            analogWrite(izqA, 0);
            analogWrite(derB, vel);  
            analogWrite(izqB, vel);      
              break;
      case 'f':  // El vehículo avanzará de manera automática
            digitalWrite(TRIGGER, HIGH);   // Se envía por el trigger un pulso de 1 milisegundo
            delay(1);
            digitalWrite(TRIGGER, LOW);
     
            duracion = pulseIn(ECHO, HIGH);              // Se obtiene el tiempo del Echo
            distancia = (duracion/2) / 29;              // Se calcula la distancia (en cm)
            delay(10); 

             
            if (distancia <= 15 && distancia >=2){     // Se establece la condición: si la distancia es menor de 15 cm y mayor que 2 cm:
                
                analogWrite(derB, 0);                  // Se detendrán los motores durante 200 milisegundos
                analogWrite(izqB, 0); 
                analogWrite(derA, 0);    
                analogWrite(izqA, 0); 
                delay (200);
                
                analogWrite(derB, vel);               // El vehículo retrocederá durante 1 segundo
                analogWrite(izqB, vel); 
                delay(1000);           
                
                analogWrite(derB, 0);                // El vehículo girará hacia un lado durante 800 milisegundos (en este caso hacia la izquierda)   
                analogWrite(izqB, 0); 
                analogWrite(derA, 0);  
                analogWrite(izqA, vel);  
                delay(800);
                
                digitalWrite(13,LOW);
            }

            else{                                   // En caso de no detectar obstáculos, el vehículo continuará su marcha 
                 analogWrite(derB, 0);     
                 analogWrite(izqB, 0); 
                 analogWrite(derA, vel);  
                 analogWrite(izqA, vel); 
            }
              break;
      }



// CÓDIGO CORRESPONDIENTE A LOS INTERMITENTES


// Se colocarán dos leds que actuarán como intermitentes del vehículo


  if(estado=='c'){                    // El estado 'c' corresponde al movimiento hacia la derecha del vehículo
  
   int ledderecha = 13;              // En este caso el led de la derecha se ha colocado en el pin 13
   
   digitalWrite(ledderecha, HIGH);   
   delay(100);                       // Se le atribuye al intermitente el valor de 100 milisegundos
   digitalWrite(ledderecha, LOW);
   delay(100);
 }

 if(estado=='e'){                     // El estado 'e' corresponde al movimiento hacia la izquierda del vehículo
  
   int ledizquierda = 12;             // En este caso el led de la izquierda se ha colocado en el pin 12
   
   digitalWrite(ledizquierda, HIGH);   
   delay(100);                         // Se le atribuye al intermitente el valor de 100 milisegundos
   digitalWrite(ledizquierda, LOW);
   delay(100);
 }




 // CÓDIGO CORRESPONDIENTE A SENSOR ULTRASONIDO Y BUZZER
 
  iniciarTrig();   // Preparamos el sensor de ultrasonidos
  char cdistancia[10];
  
  //float distancia = ObtenerDist();                                 // Obtenemos la distancia
 
  if (ObtenerDist() <= 60)
  {
    sprintf(cdistancia, "%d", ObtenerDist());                          // Paso la distancia a una cadena para enviarla al visual
  for (int i=0;i<10;i++)                          // Recorrido de la cadena
 {
 Serial.print(cdistancia[i]);                               //Envío carácter a carácter
 
 }
 Serial.println();                               
  
  float frecuencia = -7.5 * ObtenerDist() + 900;                       // La frecuencia tiene una relación lineal con la distancia
  float duracion = 5 * ObtenerDist() + 200;                            // La duración también tiene una relación lineal con la distancia
  tone(BUZZER,frecuencia , duracion); 
 }

//Estas relaciones lineales las hemos obtenido para que se ajusten a la capacidad de los sensores y a nuestras preferencias                                      


// CÓDIGO CORRESPONDIENTE A SENSOR DE AGUA Y SIRENA
  
  int nivel=0;
  int lvl=0;

  digitalWrite(azul,HIGH);
  digitalWrite(rojo,LOW);
  tone(zumb,500);
  delay(tiemp);
  tone(zumb,1000);
  digitalWrite(azul,LOW);
  digitalWrite(rojo,HIGH);
  delay(tiemp);

 
 nivel=analogRead(A0);
 lvl=map(nivel,0,500,0,100);
 lcd.setCursor(10,0);
 lcd.print(lvl);


// CÓDIGO ASOCIADO AL SENSOR DE LLAMA

  int led = 13; // LED pin
  int digitalPin = 2; // KY-026 entrada digital 
  int analogPin = A0; // KY-026 entrada análoga 
  int digitalVal; // lectura digital 
  int analogVal;
  int zumbador = 9;// lectura análoga

  // lectura entrada digital
  digitalVal = digitalRead(digitalPin); 
  if(digitalVal == HIGH) // si la llama es detectada
  {
    digitalWrite(led, HIGH); // enciende el LED
    delay(100);
    digitalWrite(led, LOW); // enciende el LED
    delay(100);
    digitalWrite(zumbador, HIGH);  // Se activa el buzzer indicando la señal de fuego 
    delay(100);
    digitalWrite(zumbador, LOW);   // Desactiva el buzzer
    delay(100);

  }
  else
  {
    digitalWrite(led, LOW); // apaga el LED
    digitalWrite(zumbador, LOW); // apaga el zumbador
  }

  // lectura entrada análoga
  analogVal = analogRead(analogPin); 
  Serial.println(analogVal); // imprime valor analogico, ver en monitor serial

  delay(100);
  

// CÓDIGO CORRESPONDIENTE A SISTEMA ESCALERA-MANGUERA

  
  // variables dirección por teclado
  //
  
  int giro_izqda = 128;    //ya que 4096 es el número de pasos totales por vuelta; siendo este el tope máximo para evitar retorcer las conexiones de los otros módulos
  int leer1; // variable para almacenamiento de caracteres (motor paso a paso)
  int i;
  boolean direcc = true; 

  leer1 = Serial.read(); //almacena la lectura del puerto serial en la variable "leer1" que es de tipo caracter(motor paso a paso)
 
   int pasos = 0; 



  switch(leer1){
    
    case 'i': 
            // --> giro a la izquierda motor paso a paso
              
              while (giro_izqda > 0) //bucle encargado de la rotación
              { 
                
                giro();     // Avanza un paso
                giro_izqda-- ;  // Un paso menos
                delay(1) ;
                } 
                giro_izqda = 128;

            break;
            
    case 'd': 
            // --> giro a la derecha motor paso a paso

            direcc = ! direcc;

            while (giro_izqda > 0) { //bucle encargado de la rotación
                
                giro();     // Avanza un paso
                giro_izqda-- ;  // Un paso menos
                delay(1) ;
                } 
                giro_izqda = 128;

                direcc = ! direcc;
            break;
          
            
    case 'b': 
            //bomba de agua  
            
            digitalWrite(bomb,HIGH);

            break;
            
   case 'c': 
            
            digitalWrite(bomb,LOW);
            
            break;
            
   case 'm': 
            //motor escalera
            digitalWrite(motor,HIGH);
            
            break;
            
   case 'n': 
            
            digitalWrite(motor,LOW);
            
            break;
    
    }

}

//funciones

int giro()   {   //función del giro del motor paso a paso 
  
  //definición de matriz para un giro mediante medios pulsos
  
  int pasos = 0;  // Define el paso actual de la secuencia 
  int pulso [ 8 ][ 4 ] = { 
    
    {1, 0, 0, 0},
    {1, 1, 0, 0},
    {0, 1, 0, 0},
    {0, 1, 1, 0},
    {0, 0, 1, 0},
    {0, 0, 1, 1},
    {0, 0, 0, 1},
    {1, 0, 0, 1}
    
    };

  digitalWrite( IN1, pulso[pasos][ 0] );
  digitalWrite( IN2, pulso[pasos][ 1] );
  digitalWrite( IN3, pulso[pasos][ 2] );
  digitalWrite( IN4, pulso[pasos][ 3] );

  situardirecc();
}

void situardirecc() // sunción encargada de ajustar la dirección del motor paso a paso
{
  int pasos = 0;  // Define el paso actual de la secuencia 
  boolean direcc = true; 

  if (direcc)
    pasos++;
  else
    pasos--;

  pasos = ( pasos + 8 ) % 8 ;//por tratarse en este caso de un movimiento de medios pasos
  
  }

   

// Funcion que inicia la secuencia del Trigger 

void iniciarTrig()
{
  
  digitalWrite(TRIGGER, LOW);                                      // Ponemos el Trigger en estado bajo y esperamos 2 ms
  delayMicroseconds(2);
 
  
  digitalWrite(TRIGGER, HIGH);                                     // Ponemos el pin Trigger a estado alto y esperamos 10 ms
  delayMicroseconds(10);
 
 
  digitalWrite(TRIGGER, LOW);                                      // Terminamos poniendo el pin Trigger en estado bajo
 }


int ObtenerDist()                                                  // Funcion que devuelve una variable tipo float que contiene la distancia
{
  unsigned long tiempo = pulseIn(ECHO, HIGH);                      // Mide el tiempo que tarda en recibir la onda el transductor ECHO
  // Obtenemos la distancia en cm
  
  int distancia = tiempo * 0.01715;                                // La distancia es igual al tiempo por la velocidad del sonido en cm por microsegundo
 
  return distancia;
}
