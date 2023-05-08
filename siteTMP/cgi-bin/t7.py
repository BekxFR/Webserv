#!/usr/bin/env python

from wsgiref.handlers import CGIHandler
from flask import Flask

# Define the Flask application object
app = Flask(__name__)

# Define a route for the home page
@app.route('/')
def home():
    return 'Hello, world!'

# Run the Flask application using the CGI handler
if __name__ == '__main__':
    CGIHandler().run(app)
