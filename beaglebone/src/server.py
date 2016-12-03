from flask import Flask, Blueprint, render_template, abort, send_from_directory
from jinja2 import TemplateNotFound

# import socketio
# import eventlet
# import eventlet.wsgi
from comms import serial_reader
import thread

#sio = socketio.Server()

comms = serial_reader.SerialReader()
thread.start_new_thread(comms.start, ())

app = Flask(__name__)

simple_page = Blueprint('simple_page', __name__,
                        template_folder='templates')


@simple_page.route('/', defaults={'page': 'index'})
def show_index(page):
    try:
        return render_template('pages/%s.html' % page, name='Harley')
    except TemplateNotFound:
        abort(404)


@simple_page.route('/assets/<path:path>')
def serve_css(path):
    print(path)
    try:
        return send_from_directory('templates\\pages\\assets\\', path)
    except Exception:
        print Exception.message
        abort(404)


@simple_page.route('/<page>')
def show(page):
    try:
        return render_template('pages/%s' % page, nodes=comms.nodes)
    except TemplateNotFound:
        abort(404)

app.register_blueprint(simple_page)
# Blueprint can be registered many times
app.register_blueprint(simple_page, url_prefix='/pages')


if __name__=='__main__':
    app.run(host='0.0.0.0')
  # app = socketio.Middleware(sio, app)
  #
  # eventlet.wsgi.server(eventlet.listen(('0.0.0.0', 8000)), app)



