# client

import socket
import sys

if len(sys.argv) == 1:
    port = 7605
else:
    port = int(sys.argv[1])

address = ('127.0.0.1', port)
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(address)

print '---------Quit------'

s.send('Quit\n')

print 'send ------------'

data = s.recv(512)
print 'Return:', data


s.close()

