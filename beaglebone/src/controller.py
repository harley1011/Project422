# Serial2 TX = pin 21 on P9 header
# Serial2 RX = pin 22 on P9 header

import SimpleHTTPServer
import SocketServer
import thread

from bbio import *

online_node_ids = []
data_buffer = ''

def setup():
  Serial2.begin(9600)
  PORT = 8000
  Handler = SimpleHTTPServer.SimpleHTTPRequestHandler

  httpd = SocketServer.TCPServer(("", PORT), Handler)
  print "serving at port", PORT
  thread.start_new_thread(httpd.serve_forever, ())

def loop():
  if (Serial2.available()):
    # There's incoming data
    
    while(Serial2.available()):
      data += Serial2.read()
      delay(5)

    # Print what was sent:
    print "Data received:\n  '%s'" % data
    data_buffer += data
    parse_message()
    
  delay(200)


def parse_message():
	# Handle new node register
	if message[0] == 0:
		# Message is expected to be 5 bytes
		handle_new_node(data_buffer[:5])
		data_buffer = data_buffer[5:]
		
	else:
		handle_node_message(data_buffer[:12])
		data_buffer = data_buffer[12:]
		
def handle_new_node(message):
	current_id = online_node_ids[len(online_node_ids) - 1]
	message[0] = chr(current_id + 1)
	Serial2.write(message)

def handle_node_message(message):
	id = message[0]
	info = message[1]

run(setup, loop)

