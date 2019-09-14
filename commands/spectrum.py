
import json
import serial

ser = serial.Serial('/dev/ttyACM0', 9600)
ser.write(str(json.dumps({'theme': 'spectrum'})) + b'\n')
