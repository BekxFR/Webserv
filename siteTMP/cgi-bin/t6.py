#!/usr/bin/python3
from art import *

Art = text2art("SHEEP ART", font='block', chr_ignore=True)

print('Content-Type: text/html')
print('')
print('<!DOCTYPE html>')
print('<html>')
print('<head>')
print('<meta charset="UTF-8">')
print('<title>My Python CGI Test</title>')
print('<style>')
print('body {')
print('  font-family: Arial, sans-serif;')
print('  background-color: #fa94b6;')
print('}')
print('h1 {')
print('  text-align: center;')
print('}')
print('body {')
print('  color: #111;')
print('  margin: auto;')
print('  border: 2px solid #fff;')
print('  text-align: center;')
print('}')
print('</style>')
print('</head>')
print('<body>')
print('<h1>My Python CGI Test</h1>')
print("<pre id='art'>")
print(Art)
print("</pre>")
print('</body>')
print('</html>')