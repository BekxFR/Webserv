/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_request.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgruson <mgruson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 14:31:36 by mgruson           #+#    #+#             */
/*   Updated: 2023/05/10 13:29:18 by mgruson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server_request.hpp"

server_request::server_request()
{
	_isBody = 0;
}

server_request::server_request(server_request const &obj)
{
	*this = obj;
}

server_request::~server_request()
{
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
	return *this;
}

server_request::server_request(std::string ServerRequest) :
	_ServerRequest(ServerRequest)
{
	_isBody = 0;
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

unsigned long long server_request::getContentLength() const
{
	char *endptr;
	unsigned long long result = strtoull(_contentLength.c_str(), &endptr, 10);
	return (result);
}

std::ostream& operator <<(std::ostream &out, server_request &ServRequest)
{
	out << "Method : " << ServRequest.getMethod() \
		<< "\nRequestURI : " << ServRequest.getRequestURI() \
		<< "\nServerRequest : " << ServRequest.getServerRequest() << "\n";
	return (out);
}

std::string url_decode(const std::string& encoded_string)
{
	std::ostringstream decoded_stream;
	std::istringstream encoded_stream(encoded_string);
	encoded_stream >> std::noskipws;
	char current_char;
	while (encoded_stream >> current_char)
	{
		if (current_char == '%')
		{
			char char1, char2;
			encoded_stream >> char1 >> char2;
			if (!isxdigit(char1) || !isxdigit(char2))
			{
				// handle error, invalid encoded string
				break;
			}
			int decoded_value;
			std::string tmp;
			tmp.push_back(char1);
			tmp.push_back(char2);
			std::istringstream hex_stream(tmp);
			tmp.erase();
			hex_stream >> std::hex >> decoded_value;
			decoded_stream << static_cast<char>(decoded_value);
		}
		else if (current_char == '+')
		{
			decoded_stream << ' ';
		}
		else
		{
			decoded_stream << current_char;
		}
	}
	return decoded_stream.str();
}

void server_request::add_Host_Value(const std::string& str)
{
    std::string word;
    std::string tmpStr;
    std::string line;
    int count = 0;
    int status = 0;

	std::string::size_type host_start = str.find("HTTP/1.1\r\n");
	std::string::size_type host_end = str.find("\r\n\r\n", host_start);
	tmpStr = str.substr(host_start + 8, host_end);
	std::transform(tmpStr.begin(), tmpStr.end(), tmpStr.begin(), ::tolower);

	std::istringstream ss(tmpStr);
	std::istringstream iss(tmpStr);
	while (std::getline(ss, line)) {
		if (line.size() >= 5 && line.substr(0, 5) == "host:")
		{
			std::getline(iss, line);
			while (iss >> word)
			{
				if (count == 2)
					break ;
				if (count == 0)
				{
					if (word.size() > 5 && word.substr(0, 5) == "host:")
					{
						this->_host = word.substr(word.find(":") + 1);
						return ;
					}
					if (word.size() == 5 && word.substr(0, 5) == "host:")
						status = 1;
					status = 1;
				}
				if (count == 1 && status == 1)
				{
					this->_host = word;
					return ;
				}
				count++;
			}
			count = 0;
		}
		count = 0;
	}
	return ;
}

void server_request::request_parser()
{
	// Extraire la méthode HTTP (GET/POST/DELETE)
	this->_method = "";
	std::string::size_type method_end = _ServerRequest.find(' ');
	this->_method = _ServerRequest.substr(0, method_end);
	this->_Method = this->_method;

	// Extraire le chemin de la ressource (/index.html)
	this->_path = "";
	std::string::size_type path_end = _ServerRequest.find(' ', method_end + 1);
	this->_path = _ServerRequest.substr(method_end + 1, path_end - method_end - 1);
	this->_RequestURI = this->_path;

	// Extraire le type de la ressource (html et les arguments test.php?dir=t1&div=t2&sheep=t3)
	this->_type = "html";
	this->_query = "";
	if (this->_path.find('.', 0) != std::string::npos)
	{
		std::string tmp = "";
		tmp = this->_path.substr(this->_path.find('.', 0) + 1);
		if (tmp.find('?', 0) != std::string::npos)
		{
			std::string::size_type args_start = tmp.find('?', 0);
			this->_type = tmp.substr(0, args_start);
			this->_argsBrutes = tmp.substr(args_start + 1);
			for (size_t i = 0; i < tmp.size(); i++)
				if (tmp[i] == '&')
					tmp[i] = ' ';
			this->_path = _path.substr(0, _path.find('?', 0));
			this->_query = tmp.substr(args_start + 1);
			this->_query = url_decode(this->_query);
		}
		else if (tmp.size() > 1)
			this->_type = tmp;
	}

	// Extraire la version HTTP (HTTP/1.1)
	this->_version = "";
	std::string::size_type version_end = _ServerRequest.find("\r\n", path_end + 1);
	this->_version = _ServerRequest.substr(path_end + 1, version_end - path_end - 1);

	// Extraire le type de connexion (keep-alive)
	this->_connectionType = "";
	std::string::size_type ctype_start = _ServerRequest.find("Connection: ");
	std::string::size_type ctype_end = _ServerRequest.find("\r\n", ctype_start);
	if (ctype_start != ctype_end)
		this->_connectionType = _ServerRequest.substr(ctype_start + 12, ctype_end - ctype_start - 12);

	// Extraire le type de MIME (image/png) IF GET
	this->_mimeType = "";
	std::string::size_type mtype_start = _ServerRequest.find("Accept: ");
	std::string::size_type mtype_end = _ServerRequest.find("\r\n", mtype_start);
	if (mtype_start != mtype_end)
		this->_mimeType = _ServerRequest.substr(mtype_start + 8, mtype_end - mtype_start - 8);

	// Extraire le type de content (multipart/form-data) IF POST
	this->_contentType = "";
	std::string::size_type cotype_start = _ServerRequest.find("Content-Type: ");
	std::string::size_type cotype_end = _ServerRequest.find("\r\n", cotype_start);
	if (cotype_start != cotype_end)
		this->_contentType = _ServerRequest.substr(cotype_start + 14, cotype_end - cotype_start - 14);

	// Extraire la longueur du content (multipart/form-data) IF POST
	this->_contentLength = "";
	std::string::size_type colength_start = _ServerRequest.find("Content-Length: ");
	std::string::size_type colength_end = _ServerRequest.find("\r\n", colength_start);
	if (colength_start != colength_end)
		this->_contentLength = _ServerRequest.substr(colength_start + 16, colength_end - colength_start - 16);

	this->_body = "";
	std::string::size_type body_start = _ServerRequest.find("\r\n\r\n", method_end + 1);
	std::string::size_type body_end = _ServerRequest.find("\r\n", body_start + 4);
	if (body_start != body_end)
		this->_body = _ServerRequest.substr(body_start + 4);

	// IF POST and content type : application/x-www-form-urlencoded, recupere les arguments du PHP in body
	if (this->_contentType == "application/x-www-form-urlencoded")
	{
		_isBody = 1;
		if (this->_query.size() < 1)
		{
			std::string tmp = this->_body;
			this->_argsBrutes = tmp;
			for (size_t i = 0; i < tmp.size(); i++)
				if (tmp[i] == '&')
					tmp[i] = ' ';
			this->_query = tmp;
			this->_query = url_decode(this->_query);
		}
	}
	// Afficher les résultats
	if (0)
	{	
		std::cout << "\n\nMéthode : " << this->_method << std::endl;
		std::cout << "Chemin : " << this->_path << std::endl;
		std::cout << "Type : " << this->_type << std::endl;
		std::cout << "Arguments : " << this->_query << std::endl;
		std::cout << "Arguments Brutes : " << this->_argsBrutes << std::endl;
		std::cout << "Version : " << this->_version << std::endl;
		std::cout << "Host : " << this->_host << std::endl;
		std::cout << "Connection : " << this->_connectionType <<  std::endl;
		std::cout << "MIME : " << this->_mimeType <<  std::endl;
		std::cout << "Content Type : " << this->_contentType <<  std::endl;
		std::cout << "Content Length : " << this->_contentLength << "\n" <<  std::endl;
		std::cout << "Body : " << this->_body << "\n" <<  std::endl;
	}
}

