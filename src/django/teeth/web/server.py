import socket
import sys

# Create a TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# Bind the socket to the port
server_address = ('0.0.0.0', 2121)
print >>sys.stderr, 'starting up on %s port %s' % server_address
sock.bind(server_address)
sock.listen(1)

while True:
    # Wait for a connection
    print >>sys.stderr, 'waiting for a connection'
    connection, client_address = sock.accept()

    try:
    	print >>sys.stderr, 'connection from', client_address

		# Receive the data in small chunks and retransmit it
        f=open("data.dat","w")
        while True:
			data = connection.recv(8388608)
			if data:
				print "%s" % data
				f.write("%s" % data)
			else:
				print >>sys.stderr, 'no more data from', client_address
				break
        f.close()				

    finally:
		# Clean up the connection
		connection.close()