import serial
import time

ser = serial.Serial('COM5', 9600)
print("Put your card to the reader...")

def read_rfid():
    ser.write(b'C')
    time.sleep(0.1)
    return ser.readline().decode().strip()

try:
    while True:
        rfid_data = read_rfid()
        if rfid_data == 'A':
            print("Access granted.")
        elif rfid_data == 'D':
            print("Access denied.")
        else:
            rfid_data = read_rfid()

except KeyboardInterrupt:
    print("Exiting the program.")

ser.close()
