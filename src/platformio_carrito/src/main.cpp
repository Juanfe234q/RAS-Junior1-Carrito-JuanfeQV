#include <Arduino.h> //Liberia del arduino
#include <ESP8266WiFi.h> //Liberia 
#include <WiFiUdp.h> //Liberria de conexion a la udp

/*Configuración del Wifi
AAqui el puerto UDP es el mismo que se puso en el NODO
*/
const char* SSID     = "JuanFe";      
const char* PASSWORD = "PerroRocco1";    
const int   UDP_PORT = 8274;               

//Pines del L298N
#define IN1 D7
#define IN2 D6
#define IN3 D5
#define IN4 D4

//Pines del Sensor de Movimiento
#define TRIG D3
#define ECHO D2

//Distancia minima en cm pa parar
#define DISTANCIA_MINIMA 20

WiFiUDP udp;

//Funciones del Movimiento
//Adelantee
void Adelante(){ 
  digitalWrite(IN1,HIGH); 
  digitalWrite(IN2,LOW);  
  digitalWrite(IN3,HIGH); 
  digitalWrite(IN4,LOW); 
}
//Atras
void Atras(){
   digitalWrite(IN1,LOW);
   digitalWrite(IN2,HIGH);
   digitalWrite(IN3,LOW);
   digitalWrite(IN4,HIGH);
}
//Izquierda
void Izquierda(){
  digitalWrite(IN1,LOW);  
  digitalWrite(IN2,HIGH); 
  digitalWrite(IN3,HIGH); 
  digitalWrite(IN4,LOW); 
}
//Derecha
void Derecha(){
   digitalWrite(IN1,HIGH);
    digitalWrite(IN2,LOW);
    digitalWrite(IN3,LOW);
    digitalWrite(IN4,HIGH);
}
//Deternes
void Detenerse(){ 
  digitalWrite(IN1,LOW);  
  digitalWrite(IN2,LOW);  
  digitalWrite(IN3,LOW);  
  digitalWrite(IN4,LOW); 
}

//Leer distancia a un objeto
float leerDistancia(){
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  long duracion = pulseIn(ECHO, HIGH);
  return duracion * 0.034 / 2;
}

void setup(){
  Serial.begin(115200); //Velocidad de Comunicacion

  //Configuración de los pines
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  Detenerse();

  //Pines del senor
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  //COnecta Wifi
  WiFi.begin(SSID, PASSWORD);
  Serial.print("Conectando al WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi Readyy!");
  Serial.print("IP del Wemos:");
  Serial.println(WiFi.localIP());

  //Iniciar  el UDP
  udp.begin(UDP_PORT);
  Serial.println("Todo listones! ");
}

void loop(){
  float distancia = leerDistancia();

  //Aqui empezamos por leer la distancia
  if (distancia < DISTANCIA_MINIMA && distancia > 0) {
    Detenerse();
    Serial.println("Obstaculo mas adelabte!! Parando...");
    return; 
  }

  // Revisar si llego un comando del topico
  int size = udp.parsePacket();
  if (size){
    char cmd = udp.read();
    Serial.print("Movimiento recibido!");
    Serial.println(cmd);

    if(cmd == 'F') 
    Adelante();
    else if 
    (cmd == 'B') 
    Atras();
    else if 
    (cmd == 'L') 
    Izquierda();
    else if 
    (cmd == 'R') 
    Derecha();
    else                 
    Detenerse();
  }
}