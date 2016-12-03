# Serial2 TX = pin 21 on P9 header
# Serial2 RX = pin 22 on P9 header
import time
from bbio import *


class SerialReader:

    def __init__(self):
        self.nodes = []
        self.data_buffer = ''
        self.status = 'Offline'


    def setup(self):
        print "Setting up serial communications..."
        Serial2.begin(9600)

    def loop(self):
        data = ''
        if (Serial2.available()):
            # There's incoming data

            while(Serial2.available()):
                data += Serial2.read()
                delay(5)

            # Print what was sent:
            print "Data received:\n  '%s'" % data
            self.data_buffer += data
            self.parse_message()
        delay(200)

    def parse_message(self):
        # Handle new node register
        if message[0] == 0 and len(self.data_buffer) >= 5:
            handle_new_node(self.data_buffer[:5])
            self.data_buffer = data_buffer[5:]
        elif len(self.data_buffer) >= 12:
            handle_node_message(self.data_buffer[:12])
            self.data_buffer = self.data_buffer[12:]

    def handle_new_node(self, message):
        current_id = self.nodes[len(online_node_ids) - 1].id
        message[0] = chr(current_id + 1)
        self.nodes.append(Node(current_id + 1))
        Serial2.write(message)

    def handle_node_message(self, message):
        id = message[0]
        info = message[1]

    def start(self):
        run(setup, loop)


class Node:
    def __init__(self, id, name, status):
        self.id = id
        self.name = name
        self.status = status
        self.last_receive = time.time()

