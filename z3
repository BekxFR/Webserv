[0;34m###[0mCreation du fichier objects/server_response.o[0;34m###[0;33m
c++ -std=c++98 -Wall -Wextra -Werror -MMD -MP -Isources/ -g3 -c sources/server_response.cpp -o objects/server_response.o
[0m
[0;34m###[0mCreation du fichier webserv[0;34m###[0;33m
c++ objects/main.o objects/server_configuration.o objects/server_request.o objects/server_response.o objects/server_location_configuration.o objects/ErrorCorresp.o -std=c++98 -Wall -Wextra -Werror -MMD -MP -Isources/ -g3 -o webserv
[0m
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes ./webserv sources/server.conf2
Can't open file error.html using default error page
Can't open file e using default error page
Ports 0 : 8083
Ports 0 : 8087
Ports 0 : 8080
Ports 0 : 8083
Ports 0 : 8084
conn_sock : 9
events[n].data.fd : 3
events[n].data.fd : 3
events[n].data.fd : 3
events[n].data.fd : 3
events[n].data.fd : 3
Request :
GET /download/ HTTP/1.1
Host: localhost:8083
User-Agent: insomnia/2022.7.0
webserv:
Accept: */*



body_start : 99 body_end : 18446744073709551615

Méthode : GET
Chemin : /download/
Type : html
Version : HTTP/1.1
Host : localhost:8083
Connection : 
MIME : */*
Content Type : 
Content Length : 

Body : 

PORT TEST : 8083
server_response int Constructor called
BIG TEST 0: /download/
BIG TEST : /download
RealPath : ./site/download
RealPathIndex : ./site/download/
PathToStore : ./site/
 BOOL TRUE is_dir 1
FinalPath : ./site/download/
status code 403
JE SUIS DANS CLIENT
TEST : html

SERVER RESPONSE CONSTRUITE -> 
HTTP/1.1 403 Forbidden
Content-Type: text/html
Content-Length: 335

<html><head><meta name="viewport" content="width=device-width, minimum-scale=0.1"><title>403 Forbidden</title></head><body style="background: #0e0e0e; height: 100%;text-align:center;color:white;"><h1>403 Forbidden</h1><img src="https://http.cat/403" style="display: block;margin: auto;" alt="403 Forbidden"><p>webserv</p></body></html>


server_request Destructor called
server_response Destructor called
events[n].data.fd : 9
events[n].data.fd : 9
events[n].data.fd : 9
events[n].data.fd : 9
events[n].data.fd : 9
events[n].data.fd : 9
events[n].data.fd : 9
events[n].data.fd : 9
events[n].data.fd : 9
events[n].data.fd : 9
