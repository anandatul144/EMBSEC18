import socket
import sys

# create dgram udp socket
try:
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    print ('Sucessfull socket Connection ')
except socket.error:
    print ('Failed to create socket')
    sys.exit()

host = '192.168.1.177';
port = 8888;

while(1) :
    try :
        #sending below code to overflow
        s.sendto(b'288#33ed000000585200', (host, port))
        # receive data from client (data, addr)
        d = s.recvfrom(4096)
        reply = d[0]
        addr = d[1]
        print ('Server reply : ' + reply)
    except socket.error:
        print ('Error Code : ' + str(socket.error))
        sys.exit()
