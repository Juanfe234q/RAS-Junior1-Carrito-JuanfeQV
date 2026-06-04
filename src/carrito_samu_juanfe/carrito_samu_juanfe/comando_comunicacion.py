import rclpy #Libreria de ROS2
from rclpy.node import Node #Base de creación de los Nodos
from geometry_msgs.msg import Twist #Twist es el formato que llega por el Topico
import serial #COnunicacion Serial
import time

SERIAL_PORT = '/dev/ttyUSB0' #Puerto al que siempre esta conectado el ESP32 
BAUD_RATE = 115200 #Velocidad del ESP32

class Comando_Comunicacion(Node): #Creamos el nodo de Comunicacion
    def __init__(self):
        super().__init__('comando_comunicacion') #Inicializamos el Nodo
        self.sub = self.create_subscription( #Aqui se suscribe al Topico y recibe los parametros
            Twist,
            'topico_carrito_sj',
            self.callback, #Cada vez que el nodo de movimiento publique un mensaje se ejecuta el self.callback
            10
        )
        self.ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1) #Aqui hace la conexión serial con el puerto, vel y un timeout
        time.sleep(2)
        self.get_logger().info('Comunicación Serial Lista!')

    #Para la definición del callback recibe el mensaje Twist del Topico, lee los valores y los convierte en una dirección
    def callback(self, msg):
        lx = msg.linear.x
        az = msg.angular.z

        if   lx > 0.1:   cmd='F'
        elif lx < -0.1:  cmd='B'
        elif az > 0.1:   cmd='L'
        elif az < -0.1:  cmd='R'
        else:            cmd='S'

        self.ser.write(cmd.encode()) #Mandamos la Letra por el Cabole Serial
        self.ser.flush() #Asegura que se envia sin quedarse en el buffer
        self.get_logger().info(f'Comando enviado: {cmd}') #Básicamente se comprueba lo q se envio por la terminal



def main(args=None):
    rclpy.init(args=args) #Encendido de ROS
    rclpy.spin(Comando_Comunicacion()) #Prende el Nodo de comunicacion y lo mantiene
    rclpy.shutdown() #Al final cuando se apaga
