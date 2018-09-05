import serial



ser = serial.Serial('/dev/ttyUSB0')

print(ser.name)

print(ser.read(1000))
