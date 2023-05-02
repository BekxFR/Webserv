#!/usr/bin/env python

from wsgiref.handlers import CGIHandler
from flask import Flask

import sys
from flask import request

app = Flask(__name__)

@app.route('/')
def list_files():
    import os
    files = os.listdir(".")
    file_list = "<ul>\n"
    for f in files:
        file_list += "<li><a href=\"" + f + "\">" + f + "</a></li>\n"
    file_list += "</ul>"
    return render_template_string('''
        <html>
        <head>
            <title>Liste des fichiers dans le répertoire courant</title>
        </head>
        <body>
            <h1>Liste des fichiers dans le répertoire courant :</h1>
            {{ file_list|safe }}
        </body>
        </html>
    ''', file_list=file_list)

# CGIHandler().run(app)