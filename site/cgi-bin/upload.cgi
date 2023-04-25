#!/usr/bin/env python3

import os
import cgi

UPLOAD_DIR = "/usr/share/nginx/html/upload/"

form = cgi.FieldStorage()

if "file" not in form:
    print("Content-Type: text/plain")
    print()
    print("No file specified")
else:
    fileitem = form["file"]

    if fileitem.filename == "":
        print("Content-Type: text/plain")
        print()
        print("No file selected")
    else:
        filename = os.path.basename(fileitem.filename)
        with open(UPLOAD_DIR + filename, "wb") as f:
            while True:
                chunk = fileitem.file.read(100000)
                if not chunk:
                    break
                f.write(chunk)

        print("Content-Type: text/plain")
        print()
        print("File uploaded successfully")