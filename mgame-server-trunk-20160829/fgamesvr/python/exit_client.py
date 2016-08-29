# client

import socket
import sys

if len(sys.argv) >= 2:
    port=int(sys.argv[1])
else:
    port=7604

print "gameserver port: %d " % (port)

address = ('127.0.0.1', port)
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(address)

print '---------Quit------'

s.send('Quit\n')

print 'send ------------'

data = s.recv(512)
print 'Return:', data

s.close()

