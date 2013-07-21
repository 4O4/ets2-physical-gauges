"""
  Simple telemetry client

  It's connecting to Telemetry Server and transferring received
  data to uController with Serial port COM3.

  PySerial module is required for program to work:
    http://pyserial.sourceforge.net/

  Telemetry Server by UltimateVision:
    http://eurotrucksimulator2.info/pl/forum/showthread.php?tid=5612


  Author: Pawelo, <pk.pawelo@gmail.com> (GitHub: https://github.com/4O4)
  License: MIT
"""

import serial
import socket

telemetry_server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
telemetry_server.connect(('127.0.0.1', 19000))              # Setting TCP/IP connection to telemetry server

arduino = serial.Serial('COM3', 115200, timeout=1)          # Setting Serial port connection

while 1:

    frame = telemetry_server.recv(512)                      # Receiving frame of data

    data = frame.split('#')
    telemetry = data[0].split('|')

    if len(telemetry) == 16:                                # To be sure that data is complete ("Index out of range" error may appear sometimes without it, that's the simplest walkaround)

        rpm = str(int(round(float(telemetry[6]))))          # Multiple conversions in order to round values
        speed = str(int(round(float(telemetry[14]) * 3.6)))
        arduino.write(rpm + '|' + speed + '#')
