import serial
import time

ser = serial.Serial('COM5', 9600)

def interpret_color_data(data):

    red, green, blue = map(int, data.split())

    if red > 100 and green > 100 and red > blue and green > blue:
        return "Detected: Yellow"
    elif red > green and red > blue:
        return "Detected: Red"
    elif green > red and green > blue:
        if red + green + blue > 500:
            return "Detected: White"
        else:
            return "Detected: Green"
    elif red > blue and green > blue:
        return "Detected: Yellow"
    else:
        return "Cannot determine color"

try:
    while True:
        data = ser.readline().decode().strip()

        if data:
            color_result = interpret_color_data(data)
            print(color_result)

        time.sleep(1)

except KeyboardInterrupt:
    ser.close()
    print("Serial connection closed.")
