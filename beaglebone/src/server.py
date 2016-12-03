from flask import Flask
from simple_page.simple_page import simple_page

# import socketio
# import eventlet
# import eventlet.wsgi
from comms import serial_reader
import thread

#sio = socketio.Server()

app = Flask(__name__)
app.register_blueprint(simple_page)
# Blueprint can be registered many times
app.register_blueprint(simple_page, url_prefix='/pages')

comms = serial_reader.SerialReader()
thread.start_new_thread(comms.start, ())


# @sio.on('connect', namespace='/chat')
# def connect(sid, environ):
#     print("connect ", sid)
#
#
# @sio.on('chat message', namespace='/chat')
# def message(sid, data):
#     print("message ", data)
#     sio.emit('reply', room=sid)
#
#
# @sio.on('disconnect', namespace='/chat')
# def disconnect(sid):
#     print('disconnect ', sid)

if __name__=='__main__':
    app.run(host='0.0.0.0')
  # app = socketio.Middleware(sio, app)
  #
  # eventlet.wsgi.server(eventlet.listen(('0.0.0.0', 8000)), app)


