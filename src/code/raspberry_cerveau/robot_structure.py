import serial
#import matplotlib.pyplot as plt
#import numpy as np
import time
import RPi.GPIO as GPIO
#import RPIO
from gpiozero import Servo


class Robot():

    def __init__(self, port_serial:str="/dev/ttyACM0", baud_serial:int = 9600) -> None:
        self.base_roulante = serial.Serial(port=port_serial, baudrate=baud_serial)

        # pin raspberry
        self.start_pin: int = 22
        self.start_pin_default_val: int = 0
        
        ## pami start pin
        self.launch_pami_pin: int = 27

        ## servomoteur
        self.servo_pin : int = 18
        #self.servomoteur = RPIO.PWM(self.servo_pin, 50)

        # game parametre
        self.running_time = None
        self.max_running_time = 110

        # stratégie
        ##bleu
        ### distance en cm et angle en radian

        #setup 
        self.setup()

    def setup(self) -> int:
        # type de numerotation des pins
        GPIO.setmode(GPIO.BCM)
        # setup des inputs
        GPIO.setup(self.start_pin, GPIO.IN)
        #setup des outputs
        GPIO.setup(self.launch_pami_pin, GPIO.OUT)
        GPIO.output(self.launch_pami_pin, GPIO.LOW)
        self.servomoteur = Servo(self.servo_pin, min_pulse_width=0.5/1000, max_pulse_width=2.5/1000)
        #GPIO.output(self.servo_pin, GPIO.HIGH)
        return 0

    def blue_strat(self):
        send_order("AVANCE", 5000, 15) #distance2pulse(810)
        send_order("TOURNEDROITE", 3000, 0) #angle2pulse(180)
        send_order("AVANCE", 200, 10) #distance2pulse(780)


    def read_data(self) -> str:
        data = self.base_roulante.readline()
        return data

    def write_data(self,
                order: str, 
		        nb_tick : int, 
		        distance_secu: float) -> int:
        
        x = f"{order} {nb_tick} {distance_secu}\n"
        self.base_roulante.write(bytes(x,  'utf-8'))
        return 0

    def send_order(self,
                    order: str, 
		            nb_tick : int, 
		            distance_secu: float
                ) -> list[float]:
        self.write_data(angle_rot,translation,front_col,coter_col,dist_secu)
        raw_data: str = None
        while raw_data == None:
            raw_data : str = self.read_data()
        data = raw_data.split("\n")[-1].split(" ")
        return data

    def wait_before_start(self) -> int:
        while  GPIO.input(self.start_pin) == self.start_pin_default_val:
            pass
        self.running_time = time.time()
        return 0

    def launch_pami(self) -> int:
        GPIO.output(self.launch_pami_pin, GPIO.HIGH)
        GPIO.output(self.launch_pami_pin, GPIO.LOW)
        return 1

    def servo_pos0(self):
        self.servomoteur.mid()
        time.sleep(6)
    
    def servo_pos_left(self):
        self.servomoteur.max()
        time.sleep(1)

    def servo_pos_right(self):
        self.servomoteur.min()
        time.sleep(1)


base = Robot()
print("lancement des pamis")
#base.launch_pami()
print("servi a gauche")
#base.servo_pos_left()
base.wait_before_start()
#base.write_data("AVANCE", 5000, 10)
base.blue_strat()
print("1")
#base.servo_pos_right()
#print("2")
#base.servo_pos0()
print("fin")
