#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>

const char* ssid     = "JuanFe";
const char* password = "PerroRocco1";
const int wemosPort  = 8274;

WiFiUDP udp;
char packetBuffer[10];

#define IN1 33
#define IN2 32
#define IN3 26
#define IN4 27

void Adelante() { digitalWrite(IN1,LOW);  digitalWrite(IN2,HIGH); digitalWrite(IN3,LOW);  digitalWrite(IN4,HIGH); }
void Atras()    { digitalWrite(IN1,HIGH); digitalWrite(IN2,LOW);  digitalWrite(IN3,HIGH); digitalWrite(IN4,LOW);  }
void Izquierda(){ digitalWrite(IN1,HIGH); digitalWrite(IN2,LOW);  digitalWrite(IN3,LOW);  digitalWrite(IN4,HIGH); }
void Derecha()  { digitalWrite(IN1,LOW);  digitalWrite(IN2,HIGH); digitalWrite(IN3,HIGH); digitalWrite(IN4,LOW);  }
void Detenerse(){ digitalWrite(IN1,LOW);  digitalWrite(IN2,LOW);  digitalWrite(IN3,LOW);  digitalWrite(IN4,LOW);  }

void setup() {
  Serial.begin(115200);
  
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  Detenerse();

  WiFi.begin(ssid, password);
  Serial.print("Conectando");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n¡Conectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  udp.begin(wemosPort);
  WiFi.setSleep(false);
  Serial.println("Listo!");
}

void loop() {
  int packetSize = udp.parsePacket();
  if (packetSize) {
    int len = udp.read(packetBuffer, 9);
    if (len > 0) packetBuffer[len] = '\0';
    char cmd = packetBuffer[0];
    Serial.print("Comando: ");
    Serial.println(cmd);
    switch(cmd) {
      case 'F': Adelante();  break;
      case 'B': Atras();     break;
      case 'L': Izquierda(); break;
      case 'R': Derecha();   break;
      default:  Detenerse(); break;
    }
  }
  yield();
  delay(10);
}