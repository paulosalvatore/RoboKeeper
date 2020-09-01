from serial import *
from threading import Thread
import sys
import glob


def receiving(ser):
	while True:
		leitura = ser.read(ser.inWaiting())

		leituraClean = leitura.decode("utf-8")

		if leituraClean != "":
			print("Recebimento na Porta Serial: {}".format(leituraClean))

"""
def serial_ports():
	 Lists serial port names

		:raises EnvironmentError:
			On unsupported or unknown platforms
		:returns:
			A list of the serial ports available on the system
	
	if sys.platform.startswith('win'):
		ports = ['COM%s' % (i + 1) for i in range(256)]
	elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
		# this excludes your current terminal "/dev/tty"
		ports = glob.glob('/dev/tty[A-Za-z]*')
	elif sys.platform.startswith('darwin'):
		ports = glob.glob('/dev/tty.*')
	else:
		raise EnvironmentError('Unsupported platform')

	result = []
	for port in ports:
		try:
			s = ser.Serial(port)
			s.close()
			result.append(port)
		except (OSError, ser.SerialException):
			pass
	return result
"""

if __name__ == '__main__':
	ser = Serial(
		port="COM19",
		baudrate=9600,
		bytesize=EIGHTBITS,
		parity=PARITY_NONE,
		stopbits=STOPBITS_ONE,
		timeout=0.1,
		xonxoff=0,
		rtscts=0,
		interCharTimeout=None
	)

	Thread(target=receiving, args=(ser,)).start()

	while True:
		numero = input()

		ser.write(numero.encode())
