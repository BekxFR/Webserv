import os
from wsgiref.handlers import CGIHandler
from flask import Flask, request

app = Flask(__name__)

@app.route("/")
def list_files():
    directory = request.args.get('dir')

    if not directory:
        return "<p>Please provide a 'dir' parameter in the query string.</p>"

    try:
        files = os.listdir(directory)
        response = "<h1>Files and directories in '{}':</h1><ul>".format(directory)
        for file in files:
            response += "<li>{}</li>".format(file)
        response += "</ul>"
        return response
    except Exception as e:
        return "<p>Error: {}</p>".format(e)

CGIHandler().run(app)
