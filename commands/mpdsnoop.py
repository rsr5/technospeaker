from mpd import MPDClient
from select import select
import serial
import json

ser = serial.Serial('/dev/ttyACM0', 9600)

client = MPDClient()
client.connect("localhost", 6600)
client.send_idle('player')
while True:
    if select([client], [], [], 0)[0]:
        client.fetch_idle()
        print client.currentsong()
        ser.write(
            str(
                json.dumps(
                    {
                        'theme': 'text',
                        'message': '{track}: {artist}-{album}-{title}'.format(**client.currentsong())
                    }
                )
            ) + b'\n'
        )

        client.send_idle('player')
