#!/usr/bin/env python3

import os
import cgi
import sys

arg1 = sys.argv[1]

UPLOAD_DIR = "/mnt/nfs/homes/chillion/"

form = cgi.FieldStorage(fp=sys.stdin, environ=os.environ, keep_blank_values=True)

if arg1 not in form:
    print("Content-Type: text/plain")
    print()
    print("No file specified")
else:
    fileitem = form[arg1]

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
