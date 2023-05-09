from wsgiref.handlers import CGIHandler

def application(environ, start_response):
    # Prepare the HTTP response header
    response_header = [('Content-type', 'text/html')]
    start_response('200 OK', response_header)

    # Build the HTML content with environment variables as an HTML list
    content = "<h1>Environment Variables</h1><ul>"
    for key, value in sorted(environ.items()):
        content += f"<li>{key}: {value}</li>"
    content += "</ul>"

    return [content.encode('utf-8')]

# Run the WSGI application using the CGIHandler
CGIHandler().run(application)
