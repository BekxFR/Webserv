#!/usr/bin/python3

import os
import sys
import cgi
from art import *

# Récupérer les données POST
form = cgi.FieldStorage()
text = form.getvalue("text")

# Si aucune donnée POST n'a été envoyée, afficher le formulaire
if not text:
    print("Content-Type: text/html")
    print("")
    print("<!DOCTYPE html>")
    print("<html>")
    print("<head>")
    print("<meta charset='UTF-8'>")
    print("<title>Mon titre</title>")
    print("<style>")
    print("pre {font-family: monospace;}")
    print("</style>")
    print("</head>")
    print("<body>")
    print("<p>Saisissez une chaîne de caractères :</p>")
    print("<form method='post'>")
    print("<input type='text' name='text'>")
    print("<input type='submit' value='Valider'>")
    print("</form>")
    print("</body>")
    print("</html>")
else:
    # Afficher le texte en mode text2art
    Art = text2art(text, font='block', chr_ignore=True)
    print("Content-Type: text/html")
    print("")
    print("<!DOCTYPE html>")
    print("<html>")
    print("<head>")
    print("<meta charset='UTF-8'>")
    print("<title>Mon titre</title>")
    print("<style>")
    print("pre {font-family: monospace;}")
    print("</style>")
    print("</head>")
    print("<body>")
    print("<p>Voici votre texte en mode text2art :</p>")
    print("<pre>")
    print(Art)
    print("</pre>")
    print("</body>")
    print("</html>")