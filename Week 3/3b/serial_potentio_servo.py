import serial
import time

serial_port = "COM7"
baud_rate = 9600

ser = serial.Serial(serial_port, baud_rate)
try:
    while True:
        user_input = input("Enter a command ('H' to halt, 'Q' to quit): ")
        user_input = user_input.upper()  # Convert input to uppercase for case insensitivity
        
        if user_input == 'H':
            ser.write(b'H')  # Send 'H' to halt the Arduino
            user_input = input("Enter 'R' to restart: ")
            user_input = user_input.upper()
            if user_input == 'R':
                ser.write(b'R')  # Send 'R' to restart the Arduino
                time.sleep(0.1)
                a = ser.readline()      # read a '\n' terminated line
                strn = a.decode()
                angle = strn.rstrip()    # remove any white spaces at the end of the string
                print("Angle: " + angle + " degree.")
            else:
                print("Invalid command.")
        elif user_input == 'Q':
            break  # Exit the loop and quit the script
        else:
            print("Invalid command. Try 'H' or 'Q'.")
        
        time.sleep(0.1)
    print("Thank You!")

except KeyboardInterrupt:
    ser.close()
    print("Serial connection closed.")

