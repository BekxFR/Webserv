#!/usr/bin/python3

import os
import sys
import cgi

from art import *
Art=text2art("SHEEP ART",font='block',chr_ignore=True)

print("Content-Type: text/html")
print("")
print("<!DOCTYPE html>")
print("<html>")
print("<head>")
print("<title>Mon titre</title>")
print("<style>")
print("pre {font-family: monospace;}")
print("</style>")
print("</head>")
print("<body>")
print("<p>This is my best test for try python cgi!</p>")
print("<pre>")
print(Art)
print("</pre>")
print("</body>")
print("</html>")