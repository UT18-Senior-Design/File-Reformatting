import socket

HOST = "192.168.1.201"
PORT = 2368

soc = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
soc.bind(('', PORT))

#while True:
data = soc.recv(1248)
print(len(data))
raw_data = data[:-6]
for k in range(12):
	offset = k * 100
	print('Block #: {}'.format(k))
	azimuth = raw_data[2+offset] | (raw_data[3+offset] << 8)
	#print(raw_data)
	print('azimuth: {}'.format(azimuth/100))
	#for k in range(12):
	for i in range(2):
		for j in range(16):
			distance = raw_data[4+j*3+i*48+offset] | (raw_data[5+j*3+offset] << 8)
			#print(distance)
			distance = distance / 500
			reflectivity = data[6+j*3+offset]
			print('Channel {} distance: {}, reflectivity: {}'.format(j, distance, reflectivity))