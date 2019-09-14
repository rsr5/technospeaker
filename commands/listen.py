
from __future__ import unicode_literals
import logging
import json
from threading import Thread
from Queue import Queue
from time import sleep

from mopidy.core import CoreListener
import pykka
import serial

ser = serial.Serial('/dev/ttyACM0', 9600)
while True:
  print(ser.readline())

"""
    def volume_changed(self, volume):
        self.queue.put({'theme': 'volume', 'arg': volume})

    def track_playback_started(self, tl_track):
        self.queue.put({'theme': 'spinner'})

    def track_playback_ended(self, tl_track, time_position):
        self.queue.put({'theme': 'none'})

    def track_playback_resumed(self, tl_track, time_position):
        self.queue.put({'theme': 'spinner'})

    def playback_state_changed(self, old_state, new_state):
        if new_state in ['paused', 'stopped']:
            self.queue.put({'theme': 'none'})
"""
