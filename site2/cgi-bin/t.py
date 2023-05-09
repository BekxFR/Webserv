def application(environ, start_response):
    status = '200 OK'
    headers = [('Content-type', 'text/plain')]
    start_response(status, headers)
    
    # Lire les données d'entrée et les afficher
    data = environ['wsgi.input'].read()
    return [data]
