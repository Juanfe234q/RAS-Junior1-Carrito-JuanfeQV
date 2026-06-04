#include <Arduino.h>
/*Definición de los Parametros del Puente H y el Microcontrolador*/
#define IN1 33 
#define IN2 32
#define IN3 26
#define IN4 27
/*Definición de los Parametros del Sensor*/
#define TRIG_PIN 25
#define ECHO_PIN 19
#define DISTANCIA_MINIMA 10.0
/*Definimos las funciones de Movimiento*/
void Adelante(){ 
  digitalWrite(IN1,LOW);  
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH); 
}
void Atras(){ 
  digitalWrite(IN1,HIGH); 
  digitalWrite(IN2,LOW);  
  digitalWrite(IN3,HIGH); 
  digitalWrite(IN4,LOW);  
}
void Izquierda(){ 
digitalWrite(IN1,LOW);  
digitalWrite(IN2,HIGH); 
digitalWrite(IN3,HIGH); 
digitalWrite(IN4,LOW);  
}
void Derecha(){ 
digitalWrite(IN1,HIGH); 
digitalWrite(IN2,LOW);  
digitalWrite(IN3,LOW);  
digitalWrite(IN4,HIGH); 
}
void Detenerse(){ 
  digitalWrite(IN1,LOW);  
  digitalWrite(IN2,LOW);  
  digitalWrite(IN3,LOW);  
  digitalWrite(IN4,LOW);  
}
/*Ahora vamos co la función del sensor de distancia*/
/*Ojo: 
Acordarse que manda el Disparo y lo apaga 
y mira cuanto tiempo se demora en regresar
si la duracion es 0 pues retorna que no hay nada
y finalmente el return convierte el tiepo en cm*/
float leerDistancia(){
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duracion=pulseIn(ECHO_PIN, HIGH, 20000);
  if(duracion==0) 
  return 400;
  return(duracion*0.0343)/2.0;
}
void setup(){
  Serial.begin(115200); //Iniciamos la comunicacion a 115200 que es la del ESP32
  delay(2000);
  /*El PinMode va a mandar al Puente H como salidas*/
  pinMode(IN1,OUTPUT); 
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT); 
  pinMode(IN4,OUTPUT);
  pinMode(TRIG_PIN,OUTPUT);
  pinMode(ECHO_PIN,INPUT);
  /*Empezamos con todo apagado*/
  digitalWrite(IN1,LOW); 
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW); 
  digitalWrite(IN4,LOW);
  Serial.println("Hola Samu y Juanfe el Carrito esta listo!");
}
void loop(){
  float distancia=leerDistancia();
  /*Verificar que la distancia siempre cumpla, sino detewneerse*/
  if(distancia< DISTANCIA_MINIMA && distancia>0){
    Detenerse();
    Serial.println("Hay algo mas adelante --> FRENANDO!!!");
  }
  /*Cómo funciona el serial.available: Bueno verifica prácticamente que ROS halla mandado algún mensaje 
  - si no pues no ocurre nada */
  if(Serial.available()){
    char cmd = Serial.read(); //Variable que llega del comando (nodo) movimiento
    Serial.print("Comando: "); //Imprime el comando que llego
    Serial.println(cmd);
    

    if (distancia< DISTANCIA_MINIMA && cmd =='F'){ //Basicamente si hay algo impedimos el comando
      Serial.println("No se puede avanzar hay algo adelante");
      return;
    }
    
    //Switch-Case según la letra recibida mandamos la funcion
    switch(cmd) {
      case 'F': 
      Adelante();  
      break;
      case 'B': 
      Atras();     
      break;
      case 'L': 
      Izquierda(); 
      break;
      case 'R': 
      Derecha();  
      break;
      case 'S': 
      Detenerse(); 
      break;
      default:  
      break;
    }
  }
  delay(50);
}