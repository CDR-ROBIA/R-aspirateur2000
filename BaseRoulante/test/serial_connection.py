import serial 
import matplotlib.pyplot as plt
import numpy as np


arduino = serial.Serial(port="/dev/ttyACM0", baudrate=9600)


def read_data():
    data = arduino.readline()
    return data

def write_data(angle_rot,translation,front_col,coter_col,dist_secu):
    x = f"{angle_rot},{translation},{front_col},{coter_col},{dist_secu}\n"
    arduino.write(bytes(x,  'utf-8'))

Ls = []
Rs = []

while True:
    print("send data")
    write_data("90,180,1,1,50\n")


"""
with open("encodeur_value.txt","w+") as file:
    data = str(Ls)+";"+str(Rs)
    file.write(data)"""


with open("encodeur_value_mps.txt", "r") as file:
    data = file.read()


L, R = data.split(";")
print(L)
L = [np.float64(i) for i in L[2:-3].split(",")]#[np.float64(i.replace("'","")) for i in L[1:-1].split(",")]
R = [np.float64(i) for i in R[2:-3].split(",")]#[np.float64(i.replace("'","")) for i in R[1:-1].split(",")]


x=[20+j for j in range(len(R))]
"""plt.figure()
plt.plot( x,L, label="L")
plt.plot( x,R,label="R")
plt.legend()
plt.show()"""

coeffs = np.polyfit(R, L, 1)  # degré 1 = linéaire
a, b = coeffs
print(f"Relation estimée : R ≈ {a:.4f} * L + {b:.4f}")

# Vérification visuelle :
print(type(a), type(b), set([type(i) for i in L]))
R_fit = [a * xi + b for xi in R]
plt.plot(x, L, label='L')
plt.plot(x, R, label='R')
plt.plot(x, R_fit, '--', label='Fit R(L)')
plt.legend()
plt.show()