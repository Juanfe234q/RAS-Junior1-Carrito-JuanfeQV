#Importar Librerias 
#rcply --> Comunicacion con ROS 2
#Node --> Creacion de un Nodo
#Que es un nodo? --> Cada Nodo significa una Tarea
#Cuantos Nodos voy a usar: 2 - 1. Movimiento 2. Serial
#Cómo funciona la lógica --> Nodo Movimiento --> tópico --> mensaje --> NodoCOmunicacion se suscribe --> 
# manda por Serial --> ESP32 --> Carrito en Movimiento!
#geometry_msgs --> Paquete de ROS de movimiento
#Twist se importa del paquete --> Da lugar a la velocidad
#sys - tty - termios librerias de Python para capturaer las teclas

#Este es el Código del Comando de Movimiento

import rclpy
from rclpy.node import Node
from geometry_msgs.msg import Twist
import sys
import tty
import termios

class ComandoMovimiento(Node): #Crea la clase del Nodo
    def __init__(self): #Constructor del nodo
        super().__init__('comando_movimiento')  #Inicializar el nodo
        self.pub = self.create_publisher(Twist, 'topico_carrito_sj', 10) #Twist-Velocidad| Topico carrito-Nombre del topico|10 es el tamaño del Buffer
        #Aqui informacion que sale durante la ejecuacion
        self.get_logger().info('Teleop activo!')
        self.get_logger().info('Flecha arriba: Adelante')
        self.get_logger().info('Flecha abajo: Atras')
        self.get_logger().info('I: Izquierda')
        self.get_logger().info('D: Derecha')
        self.get_logger().info('Ctrl+C: Salir')

    def leer_tecla(self): #Funcion de leer la tecla
        #stdin --> Entrada del teclado
        #fileno --> Identificador numerico de la entrada
        fd = sys.stdin.fileno() 
        # Old guarda el estado original para despues
        old = termios.tcgetattr(fd)
        try:
            tty.setraw(fd) #cada tecla que presione sin necesidad de enter
            ch = sys.stdin.read(1) #lee un caracter del teclado
            #Como Funciona la logica de el If?
            #todas las teclas del teclado estan compuestas por x1b internamente
            #Si no detecta el x1b internamente la retorna
            
            if ch == '\x1b':
                ch2 = sys.stdin.read(2)
                return ch + ch2
            return ch
        finally:
            termios.tcsetattr(fd, termios.TCSADRAIN, old) #Aqui pase retorna a lo orginal

    def publicar(self, linear, angular): #FUncion de Publicar
        msg = Twist() #Aqui creamos el mensaje vacio de tipo twist para llenarlo
        msg.linear.x = linear #Le asignamos el valor de movimiento lineal
        msg.angular.z = angular #Le asignamos el valor de movimiento angular
        self.pub.publish(msg) #Manda el mensaje al topico

    def correr(self):
        while rclpy.ok(): #Aqui mientras este vigente la conexion a ROS
            tecla = self.leer_tecla() #Invoco la funcion leer tecla
            if tecla == '\x1b[A': #flecha de arriba
                self.get_logger().info('Adelante') #Cuando es Adelante - uso la funcion publicar
                self.publicar(1.0, 0.0) #Los valores de la funcion publicar es 1.0 lineal y 0 en angulo
            elif tecla == '\x1b[B': #flecha de abajo
                self.get_logger().info('Atras') #Misma logica con atras
                self.publicar(-1.0, 0.0) 
            elif tecla.lower() == 'i': #izquierda
                self.get_logger().info('Izquierda') #Misma logia con Izquierda
                self.publicar(0.0, 1.0)
            elif tecla.lower() == 'd': #derecha
                self.get_logger().info('Derecha') #Misma logica con Derecha
                self.publicar(0.0, -1.0)
            elif tecla == '\x03':      #Ctrl+C --> deternerlo
                break
            else:
                self.publicar(0.0, 0.0)  #parar

def main(args=None):
    rclpy.init(args=args) #Encender ROS
    node = ComandoMovimiento() #Establece el nodo
    node.correr() #Corre el Nodo
    rclpy.shutdown() #Ctrl + c termina el bucle
