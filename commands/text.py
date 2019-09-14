
import json
import serial
import sys

ser = serial.Serial('/dev/ttyACM0', 9600)
ser.write(str(json.dumps({'theme': 'text', 'message': ' '.join(sys.argv[1:])})) + b'\n')
