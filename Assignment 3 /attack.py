import socket




def run():
    ipaddress = '192.168.1.177'
    port = 8888
    mySocket = socket.socket()
    mySocket.connect((ipaddress,port))
    message = input(" -> ")

    while message != 'q':
            mySocket.send(message.encode())
            data = mySocket.recv(1024).decode()
            print ('Received from server: ' + data)
            message = input(" -> ")
    mySocket.close()

if __name__ == '__main__':
        run()
