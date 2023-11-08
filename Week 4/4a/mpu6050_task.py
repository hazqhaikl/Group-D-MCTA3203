import serial
from time import sleep

ser = serial.Serial('COM5', 9600)
dt=0.1
count=0

if ser.is_open:
    print("Serial port is open")
    try:
        while True:
            data = ser.readline().decode('utf-8').strip()
            input_list = data.split()
            
            if len(input_list) == 2:
                dataX, dataY = map(int, input_list)
                
                if count != 1 and dataY < 8:
                    print("Object moved to the left")
                    count = 1
                elif count != 2 and dataY > 10:
                    print("Object moved to the right")
                    count = 2
                elif count != 3 and dataX < 8:
                    print("Object moved to the back")
                    count = 3
                elif count != 4 and dataX > 10:
                    print("Object moved to the front")
                    count = 4

                sleep(dt)
    except KeyboardInterrupt:
        pass
    finally:
        ser.close()
else:
    print("Serial port is closed")

