# Serial2 TX = pin 21 on P9 header
# Serial2 RX = pin 22 on P9 header
import time

beaglebone = True
if beaglebone:
    from bbio import *


class SerialReader:

    def __init__(self):
        self.nodes = []
        self.data_buffer = ''
        self.status = 'Offline'

    def setup(self):
        print "Setting up serial communications..."
        if beaglebone:
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
        if self.data_buffer[0] == '0' and len(self.data_buffer) >= 6:
            self.handle_new_node(self.data_buffer[:6])
            self.data_buffer = self.data_buffer[6:]
        elif len(self.data_buffer) >= 6:
            id = 50000
            try:
                id = int(self.data_buffer[0])

            except:
                print "Something wrong with data"
            if id <= len(self.nodes):
                self.handle_node_message(self.data_buffer[:6])
                self.data_buffer = self.data_buffer[6:]
            else:
                self.data_buffer = self.data_buffer[6:]

    def handle_new_node(self, message):
        new_message = list(message)
        if len(self.nodes) == 0:
            current_id = 0
        else:
            current_id = len(self.nodes)
        new_message[0] = chr(current_id + 49) 
        self.nodes.append(Node(current_id + 1, 'Node ' + new_message[0], 'Registering'))
        return_message = ''.join(new_message)
        print "Returning " + return_message
        Serial2.write(return_message)

    def handle_node_message(self, message):
        id = message[0]
        node = self.nodes[id - 1]
        info = message[1]
        if info == '0': # Handle door
            self.handle_door(self, node, message[2:])
        elif info == '1': # Handle light
            self.handle_light(self, node, message[2:])
        elif info == '2': # Handle temp
            self.handle_temp(self, node, message[2:])

    def handle_door(self, node, data):
        node.data = data
        
    def handle_light(self, node, data):
        node.data = data

    def handle_temp(self, node, data):
        node.data = data


    def start(self):
        if beaglebone:
            run(self.setup, self.loop)


class Node:
    def __init__(self, id, name, status):
        self.id = id
        self.name = name
        self.status = status
        self.type = 'NA'
        self.last_receive = time.time()
        self.data = 0
        self.timer = none

