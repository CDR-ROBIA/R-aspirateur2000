import serial 
import matplotlib.pyplot as plt
import numpy as np
import time
import RPi.GPIO as GPIO



class Robot():

    def __init__(self, port_serial:str="/dev/ttyACM0", baud_serial:int = 9600) -> None:
        self.base_roulante = serial.Serial(port=port_serial, baudrate=baud_serial)

        # pin raspberry 
        self.start_pin: int = ... 
        self.start_pin_default_val: int = ... 

        ## pami start pin 
        self.launch_pami_pin: int = ...   

        # game parametre 
        self.running_time = None
        self.max_running_time = 110

    def setup(self) -> int:
        # type de numerotation des pins
        GPIO.setmode(GPIO.BCM) 
        # setup des inputs
        GPIO.setup(self.start_pin, GPIO.IN)
        #setup des outputs
        GPIO.setup(self.launch_pami_pin, GPIO.OUT)
        return 0

    def read_data(self) -> str:
        data = self.base_roulante.readline()
        return data

    def write_data(self, 
                   angle_rot:float,
                   translation:float,
                   front_col:int,
                   coter_col:int,
                   dist_secu:float) -> int:
        # vérification des type en entrées
        if (type(angle_rot) != float):
            raise TypeError("angle_rot n'est pas un float")
        
        if (type(translation) != float):
            raise TypeError("translation n'est pas un float")
        
        if (type(dist_secu) != float):
            raise TypeError("dist_secu n'est pas un float")
        
        if front_col not in [0,1]:
            raise ValueError("front col doit être 1 ou 0")
        
        if coter_col not in [0,1]:
            raise ValueError("coter col doit être 1 ou 0")
     
        
        x = f"{angle_rot},{translation},{front_col},{coter_col},{dist_secu}\n"
        self.base_roulante.write(bytes(x,  'utf-8'))
        return 0
    
    def send_order(self,
                   angle_rot:float,
                   translation:float,
                   front_col:bool,
                   coter_col:bool,
                   dist_secu:bool) -> list[float]:
        self.write_data(angle_rot,translation,front_col,coter_col,dist_secu)
        raw_data: str = None
        while raw_data == None:
            raw_data : str = self.read_data()
        data = raw_data.split(",")
        return data

    def wait_before_start(self) -> int:
        while  GPIO.input(self.start_pin) == self.start_pin_default_val:
            pass 
        self.running_time = time.time()
        return 0 
    
    def launch_pami(self) -> int:
        GPIO.output(self.start_pin, GPIO.HIGH)
        return 1