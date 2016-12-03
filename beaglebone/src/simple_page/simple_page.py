from flask import Blueprint, render_template, abort, send_from_directory
from jinja2 import TemplateNotFound

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
        return send_from_directory('simple_page\\templates\\pages\\assets\\', path)
    except:
        abort(404)


@simple_page.route('/<page>')
def show(page):
    try:
        return render_template('pages/%s' % page)
    except TemplateNotFound:
        abort(404)


