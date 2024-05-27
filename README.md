# Crafting Your Own Web Domain: Mastering HTTP Server Creation in C++

|    Project Name    |                                                                       webserv                                                                      |
| :----------------: | :-------------------------------------------------------------------------------------------------------------------------------------------------: |
|    Description     |       	A fully configurable home-made web server in C++ (following HTTP/1.1 RFC)                                       |
|    Technologies    |  <img alt="C++" src="https://custom-icon-badges.demolab.com/badge/C++-9C033A.svg?logo=cpp2&logoColor=white"> <img alt="PHP" src="https://img.shields.io/badge/PHP-777BB4.svg?logo=php&logoColor=white"> <img alt="HTML" src="https://img.shields.io/badge/HTML-E34F26.svg?logo=html5&logoColor=white"> |

## Install

```bash
  gcl https://github.com/BekxFR/Webserv.git
  cd webserv
  make
  ./webserv conf/server.conf3
```

## Usage
When its done compiling, you go to your chrome browser (firefox also works) and write on the URL bar "localhost:8083" to access to the website.

## Introduction

The project revolves around creating your own HTTP server in C++, adhering to a specific set of rules. Here's a summary of the key points to consider:

### General Guidelines

- Ensure your program never crashes, even in cases of memory depletion.
- Provide a Makefile that compiles your source files without relinking.
- Adhere to the C++98 standard and compile your code with flags -Wall -Wextra -Werror -std=c++98.
- Utilize C++ features as much as possible, avoiding external libraries or the Boost library set.
- Any use of external library is prohibited.

### Mandatory Part

- Program Name: webserv
- Your server must take a configuration file as an argument or use a default path.
- Ensure your server never blocks and that the client is correctly responded to if necessary.
- Use a single poll() (or equivalent) for all input/output operations between the client and server.
- Ensure poll() checks for both reading and writing simultaneously.
- Never perform a read or write operation without going through poll() (or equivalent).
- Checking the value of errno after a read or write operation is strictly prohibited.
- Your server must be compatible with a web browser and function with static pages.
- Implement at least the GET, POST, and DELETE methods.
- Rigorously test your server to ensure availability even under stress.

### Bonus Part

- You can add additional features such as support for cookies and session management, as well as handling multiple CGI.

In summary, you need to create an HTTP server in C++ that strictly adheres to the specified rules while ensuring stability, compatibility, and availability of the server.

The web server is a custom implementation of an HTTP server using C++.   
This entails creating a program capable of handling HTTP requests and responses according to the HTTP protocol standards.    
The server should be able to listen for incoming connections, parse HTTP requests, process them accordingly (such as serving static files, executing CGI scripts, handling redirects, etc.), and generate appropriate HTTP responses.    
Additionally, it must adhere to specific guidelines outlined in the project requirements, such as non-blocking I/O, support for multiple ports, error handling, and compatibility with various HTTP methods like GET, POST, and DELETE.    
This endeavor offers a deep dive into understanding the intricacies of web communication protocols, bolstering proficiency in C++ programming, and honing skills in system-level development.
