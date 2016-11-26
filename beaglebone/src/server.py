from flask import Flask
from simple_page.simple_page import simple_page
from comms import serial_reader
import thread

app = Flask(__name__)
app.register_blueprint(simple_page)
# Blueprint can be registered many times
app.register_blueprint(simple_page, url_prefix='/pages')

thread.start_new_thread(serial_reader.start, ())

if __name__=='__main__':
  app.run(host='0.0.0.0')