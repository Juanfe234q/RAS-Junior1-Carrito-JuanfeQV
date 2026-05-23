# Importar Librerias 
#rcply --> Comunicacion con ROS 2
#Node --> Creacion de un Nodo
#Que es un nodo? --> Cada Nodo significa una Tarea
#Cuantos Nodos voy a usar: 2 - 1. Movimiento 2. Wifi
#Cómo funciona la lógica --> Nodo Movimiento --> tópico --> mensaje --> NodoWifi
#Este el Código del Nodo de conexión Wifi
#Para que es el sokect? --> Es para enviar y recibir datos por red --> comunicar el Wemos con Python

import rclpy 
from rclpy.node import Node
from geometry_msgs.msg import Twist
import socket


WEMOS_IP   = '10.181.4.126' 
#El mismo para ROS Y el Wemos
WEMOS_PORT = 8274

class  Comando_wifi_carrito(Node): #Crear la Clase del Nodo
    def __init__(self): #INicialización
        super().__init__('comando_wifi_carrito') #iniciar el nodo
        self.sub = self.create_subscription( #Usar el Topic
            Twist, #Formato twists
            'topico_carrito_sj', #el topico que se va a usar donde recibe lo que paso en movimiento
            self.callback, #creacion del callback
            10 #buffer
        )
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) #UDP enviar mensajes wifi
        self.get_logger().info('Comunicación Lista!')

    def callback(self, msg): #FUncion cuando llega un mensaje al Topico
        lx = msg.linear.x #Adelante/Atras
        az = msg.angular.z #Angulo

        if   lx > 0.1:   cmd = 'F' #>0.1 va hacia adelante
        elif lx < -0.1:  cmd = 'B'#<-0.1 va hacia atras
        elif az > 0.1:   cmd = 'L'#>izquierda
        elif az < -0.1:  cmd = 'R'#< derecha
        else:            cmd = 'S'#parar

        self.sock.sendto(cmd.encode(), (WEMOS_IP, WEMOS_PORT)) #Enviar el comando por wifi al robot
        self.get_logger().info(f'Comando enviado: {cmd}') #Lo que el comando envio

def main(args=None):
    rclpy.init(args=args)
    rclpy.spin(Comando_wifi_carrito())
    rclpy.shutdown()

if __name__ == '__main__':
    main()

    #COMPROBAR FUNCIONAMIENTO DE LOS TÓPICOS
    #ros2 topic list
    #ros2 node list
    #CORRER LOS NODOS
    #ros2 run carrito_samu_juanfe comando_wifi_carrito
    #ros2 run carrito_samu_juanfe comando_movimiento
    #ROS IDENTIFICA EL PAQUETE?
    #cd ~/Documents/Junior1
    #colcon build --symlink-install