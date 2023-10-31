import serial
from time import sleep
import matplotlib.pyplot as plt

serial_port = "COM7"
baud_rate = 9600
dt = 0.05

# -- open the serial port
ser = serial.Serial(serial_port, baud_rate)

if ser.is_open:
    print(f"Serial port {ser.name} is open.")
    data = []
    N = 30                      # number of data
    for k in range(N):
        b = ser.readline()      # read a '\n' terminated line
        strn = b.decode()
        str1 = strn.rstrip()    # remove any white spaces at the end of the string
        flt = float(str1)
        print("Potentiometer Value:", flt)
        data.append(flt)        # add 'flt' to 'data' list
        sleep(dt)               # delay
        
    # -- plot data
    plt.plot(data)
    plt.xlabel("time, s")
    plt.ylabel("Potentiometer Value")
    plt.show()

    # -- close the serial port
    ser.close()

if not ser.is_open:
    print(f"Serial port {ser.name} is closed.")
