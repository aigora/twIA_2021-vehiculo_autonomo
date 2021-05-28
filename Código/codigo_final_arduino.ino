#include <LiquidCrystal.h>
#include <Servo.h>
//
// ultrasonidos
#define TRIGGER 9
#define ECHO 8
#define BUZZER 10
//
Servo servo1;
//
// medidor nivel agua 
LiquidCrystal lcd(42, 41, 35, 34, 33, 32);
//
// sirena emergencias
#define rojo 22
#define azul 23
#define zumb 24
#define tiempo 250
//
// sensor llama
#define LED_LL 2
#define LLAMA 4
#define ZUMBADOR 3
//
// motor paso a paso
#define IN1  21
#define IN2  20
#define IN3  19
#define IN4  18
//
// Servomotor
#define servo 9
//
//bomba de agua
#define bomb 8
//motor escalera
#define motor 10
//librerías
//
#include <Servo.h>
//
Servo servoMotor; //  variable de control del servo
//
int giro_izqda = 128;    //ya que 4096 es el número de pasos totales por vuelta; siendo este el tope máximo para evitar retorcer las conexiones de los otros módulos
int pasos = 0;            // Define el paso actual de la secuencia
boolean direcc = true;
//
// funciones motor paso a paso
void situardirecc();
int giro();
//

void setup() {


  // declaración de las salidas asignadas a la placa arduino
  //
  // Se definen las variables correspondientes a la dirección del vehículo
  int izqA = 11;              
  int izqB = 12; 
  int derA = 6; 
  int derB = 7; 
  int ledderecha, ledizquierda;
  //
  // CÓDIGO ASOCIADO AL MOVIMIENTO DEL VEHÍCULO
  pinMode(derA, OUTPUT);
  pinMode(derB, OUTPUT);
  pinMode(izqA, OUTPUT);
  pinMode(izqB, OUTPUT);
  pinMode(ECHO, INPUT);      // Se establece el pin 6 como entrada (echo) 
  pinMode(TRIGGER,OUTPUT);   // Se establece el pin 5 como salida (trigger)
  pinMode(BUZZER, OUTPUT);
  //
  servo1.attach(11,660,1400);    // Se asocia el servo1 al pin 11, y se definen el mínimo y el máximo del ancho del pulso 
  //
  // CÓDIGO CORRESPONDIENTE A LOS INTERMITENTES
  pinMode(ledderecha, OUTPUT);    // Se definen los pines como salidas
  pinMode(ledizquierda, OUTPUT);  
  //
  // sirena emergencias
  //
  pinMode(rojo,OUTPUT);
  pinMode(azul,OUTPUT);
  pinMode(zumb,OUTPUT);
  //
  // sensor nivel agua
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("capacidad:");
  lcd.setCursor(12,0);
  lcd.print("%");
  //
  // sensor llama
  pinMode(LED_LL, OUTPUT);
  pinMode(ZUMBADOR, OUTPUT);
  pinMode(LLAMA, INPUT);
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
    Serial.begin(57600);

}


void loop() {
  //
  // motores
   int ledderecha = 13;   
  int izqA = 11;              
  int izqB = 12; 
  int derA = 6; 
  int derB = 7; 
  int ledizquierda=14;
  int vel = 255;             // Hace referencia a la velocidad que tendrán los motores (0-255)
  char estado = 'c';         // Estado incial del vehículo (en reposo)      
  int duracion, distancia; 
  //
  //sensor llama
  int analogPin = A8; // KY-026 entrada análoga
  int digitalVal; // lectura digital
  int analogVal;
  //
  // variables dirección por teclado
  //
  int leer1; // variable para almacenamiento de caracteres (motor paso a paso)
  int i, bucleservo;

  leer1 = Serial.read(); //almacena la lectura del puerto serial en la variable "leer1" que es de tipo caracter(motor paso a paso)

  //sensor agua
  int nivel=0;
  int lvl=0;

  //  sensor llama
  // lectura entrada digital
  
  digitalVal = digitalRead(LLAMA);

            
  if(digitalVal == HIGH) { // si la llama es detectada
              
    digitalWrite(LED_LL, HIGH); // enciende el LED
    delay(100);
    digitalWrite(LED_LL, LOW); // enciende el LED
    delay(100);
    digitalWrite(ZUMBADOR, HIGH);  // Se activa el buzzer indicando la señal de fuego
    delay(100);
    digitalWrite(ZUMBADOR, LOW);   // Desactiva el buzzer
    delay(100);
  }
  else {
    digitalWrite(LED_LL, LOW); // apaga el LED
    digitalWrite(ZUMBADOR, LOW); // apaga el zumbador
  }


  switch(leer1){

     case 'a':   // Se desplazará hacia delante
            analogWrite(derB, 0);     
            analogWrite(izqB, 0); 
            analogWrite(derA, vel);  
            analogWrite(izqA, vel);      
              break;
              
      case 'b': // Se desplazará a la izquierda
            digitalWrite(ledizquierda, HIGH); 
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
   
            digitalWrite(ledderecha, HIGH);   
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
         
    case 'g':
            // --> sirena
            //
            for (int i=0; i<1000; i++) {
            digitalWrite(azul,HIGH);
            digitalWrite(rojo,LOW);
            tone(zumb,500);
            delay(tiempo);
            tone(zumb,1000);
            digitalWrite(azul,LOW);
            digitalWrite(rojo,HIGH);
            delay(tiempo);
            }

            break;
            
    case 'h':
            // --> sensor agua

            nivel=analogRead(A9);
            lvl=map(nivel,0,500,0,100);
            lcd.setCursor(10,0);
            lcd.print(lvl);
            delay(500);

            break;
            
    case 'i':
            // --> giro a la izquierda motor paso a paso

              while (giro_izqda > 0) { //bucle encargado de la rotación

                giro();     // Avanza un paso
                giro_izqda-- ;  // Un paso menos
                delay(1) ;
                }
                giro_izqda = 128;

            break;

    case 'j':
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


    case 'k':
            //bomba de agua-- encender

            digitalWrite(bomb,HIGH);

            break;

   case 'l':
            //bomba de agua-- apagar
            
            digitalWrite(bomb,LOW);

            break;

    }
}


//funciones

int giro()   {   //función del giro del motor paso a paso

  //definición de matriz para un giro mediante medios pulsos

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

  if (direcc)
    pasos++;
  else
    pasos--;

  pasos = ( pasos + 8 ) % 8 ;//por tratarse en este caso de un movimiento de medios pasos
}
