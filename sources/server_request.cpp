/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_request.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgruson <mgruson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 14:31:36 by mgruson           #+#    #+#             */
/*   Updated: 2023/04/21 11:50:38 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server_request.hpp"

server_request::server_request()
{
	std::cout << "server_request Default Constructor called" << std::endl;
}

server_request::server_request(server_request const &obj)
{
	*this = obj;
}

server_request::~server_request()
{
	if (0)
		std::cout << "server_request Destructor called" << std::endl;
}

server_request &server_request::operator=(server_request const &obj)
{
	if (this == &obj)
		return (*this);
	_ServerRequest = obj.getServerRequest();
	_Method = obj.getMethod();
	_RequestURI = obj.getRequestURI();
	_path = obj.getPath();
	_type = obj.getType();
	_version = obj.getVersion();
	_host = obj.getHost();
	_connectionType = obj.getConnectionType();
	_mimeType = obj.getMimeType();
	_contentType = obj.getContentType();
	_body = obj.getBody();
	std::cout << "server_request Copy assignment operator called" << std::endl;
	return *this;
}

server_request::server_request(std::string ServerRequest) :
_ServerRequest(ServerRequest) //, _Method(findMethod()), _RequestURI(findRequestURI())
{
}

std::string server_request::findMethod()
{
	size_t pos = _ServerRequest.find(" ");
	if (pos == std::string::npos)
		return ("");
	std::string tmp = _ServerRequest.substr(0, pos);
	_ServerRequest.erase(0, pos + 1);
	return (tmp);
}

std::string server_request::findRequestURI()
{
	size_t pos = _ServerRequest.find(" ");
	if (pos == std::string::npos)
		return ("");
	std::string tmp = _ServerRequest.substr(0, pos);
	_ServerRequest.erase(0, pos);
	return (tmp);
}

std::string	server_request::getServerRequest() const { return (_ServerRequest); }
std::string	server_request::getMethod() const { return (_Method); }
std::string	server_request::getRequestURI() const { return (_RequestURI); }
std::string	server_request::getPath() const { return (_path); }
std::string	server_request::getType() const { return (_type); }
std::string	server_request::getVersion() const { return (_version); }
std::string	server_request::getHost() const { return (_host); }
std::string	server_request::getConnectionType() const { return (_connectionType); }
std::string	server_request::getMimeType() const { return (_mimeType); }
std::string	server_request::getContentType() const { return (_contentType); }
std::string	server_request::getBody() const { return (_body); }

unsigned long long server_request::getContentLength() const
{
	char *endptr;
	unsigned long long result = strtoull(_contentLength.c_str(), &endptr, 10);
	if (*endptr != '\0') {
        std::cout << "Invalid character : " << *endptr << std::endl; 
    }
    return (result);
}

std::ostream& operator <<(std::ostream &out, server_request &ServRequest)
{
	out << "Method : " << ServRequest.getMethod() \
		<< "\nRequestURI : " << ServRequest.getRequestURI() \
		<< "\nServerRequest : " << ServRequest.getServerRequest() << "\n";
	return (out);
}

void server_request::request_parser()
{
    // Extraire la méthode HTTP (GET/POST/DELETE)
    std::string::size_type method_end = _ServerRequest.find(' ');
    this->_method = _ServerRequest.substr(0, method_end);
    this->_Method = this->_method;

    // Extraire le chemin de la ressource (/index.html)
    std::string::size_type path_end = _ServerRequest.find(' ', method_end + 1);
    this->_path = _ServerRequest.substr(method_end + 1, path_end - method_end - 1);
    this->_RequestURI = this->_path;

	this->_type = "html";
	if (this->_path.find('.', 0) != std::string::npos)
	{
		this->_type = this->_path.substr(this->_path.find('.', 0) + 1);
	}

    // Extraire la version HTTP (HTTP/1.1)
    std::string::size_type version_end = _ServerRequest.find("\r\n", path_end + 1);
    this->_version = _ServerRequest.substr(path_end + 1, version_end - path_end - 1);

    // Extraire l'en-tête Host (www.example.com)
    std::string::size_type host_start = _ServerRequest.find("Host: ");
    std::string::size_type host_end = _ServerRequest.find("\r\n", host_start);
	if (host_start != host_end)
    	this->_host = _ServerRequest.substr(host_start + 6, host_end - host_start - 6);

    // Extraire le type de connexion (keep-alive)
    std::string::size_type ctype_start = _ServerRequest.find("Connection: ");
    std::string::size_type ctype_end = _ServerRequest.find("\r\n", ctype_start);
	if (ctype_start != ctype_end)
    	this->_connectionType = _ServerRequest.substr(ctype_start + 12, ctype_end - ctype_start - 12);

    // Extraire le type de MIME (image/png) IF GET
    std::string::size_type mtype_start = _ServerRequest.find("Accept: ");
    std::string::size_type mtype_end = _ServerRequest.find("\r\n", mtype_start);
	if (mtype_start != mtype_end)
    	this->_mimeType = _ServerRequest.substr(mtype_start + 8, mtype_end - mtype_start - 8);

    // Extraire le type de content (multipart/form-data) IF POST
    std::string::size_type cotype_start = _ServerRequest.find("Content-Type: ");
    std::string::size_type cotype_end = _ServerRequest.find("\r\n", cotype_start);
	if (cotype_start != cotype_end)
    	this->_contentType = _ServerRequest.substr(cotype_start + 14, cotype_end - cotype_start - 14);

    // Extraire la longueur du content (multipart/form-data) IF POST
    std::string::size_type colength_start = _ServerRequest.find("Content-Length: ");
    std::string::size_type colength_end = _ServerRequest.find("\r\n", colength_start);
	if (colength_start != colength_end)
    	this->_contentLength = _ServerRequest.substr(colength_start + 16, colength_end - colength_start - 16);

	
    std::string::size_type body_start = _ServerRequest.find("\r\n\r\n", method_end + 1);
	std::string::size_type body_end = _ServerRequest.find("\r\n", body_start + 4);
	if (0)
		std::cout << "\nbody_start : " << body_start << " body_end : " << body_end  << std::endl;
	if (body_start != body_end)
    	this->_body = _ServerRequest.substr(body_start + 4);

		
    // Afficher les résultats
	if (0)
	{	
    std::cout << "\n\nMéthode : " << this->_method << std::endl;
    std::cout << "Chemin : " << this->_path << std::endl;
    std::cout << "Type : " << this->_type << std::endl;
    std::cout << "Version : " << this->_version << std::endl;
    std::cout << "Host : " << this->_host << std::endl;
    std::cout << "Connection : " << this->_connectionType <<  std::endl;
    std::cout << "MIME : " << this->_mimeType <<  std::endl;
    std::cout << "Content Type : " << this->_contentType <<  std::endl;
    std::cout << "Content Length : " << this->_contentLength << "\n" <<  std::endl;
    std::cout << "Body : " << this->_body << "\n" <<  std::endl;
	}
}

