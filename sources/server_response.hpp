/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_response.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgruson <mgruson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 15:09:26 by mgruson           #+#    #+#             */
/*   Updated: 2023/04/08 16:09:22 by mgruson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_RESPONSE_HPP
#define SERVER_RESPONSE_HPP

#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include <sys/epoll.h>
#include <vector>
#include <iterator>
#include <arpa/inet.h>
#include <fcntl.h>
#include <csignal>
#include <fstream>
#include <sstream>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <sys/stat.h>

#include "server_configuration.hpp"
#include "server_request.hpp"

class server_request;

class server_response
{
	private:
	int			_status_code;
	std::string	_body;
	std::string	_ServerResponse;
	std::map<std::string, std::string> _contentType;
	
	public:
	server_response();
	server_response(int);
	server_response(server_response const &obj);
	~server_response();
	server_response &operator=(server_response const &obj);

	void	todo(const server_request& Server_Request, int conn_sock, server_configuration* Root);
	void	createResponse(server_configuration*, std::string, const server_request& Server_Request);
	std::string	addHeader(std::string statusMsg, const server_request& Server_Request);
	std::string	addBody(std::string body);
	int checkConfFile(std::string MethodUsed, server_configuration *server, std::string RequestURI);
	std::string getRealPath(std::string MethodUsed, server_configuration *server, std::string RequestURI);
	std::string getRealPathIndex(std::string MethodUsed, server_configuration *server, std::string RequestURI);
	std::string getPathToStore(std::string MethodUsed, server_configuration *server, std::string RequestURI);
	bool isRedir(std::string MethodUsed, server_configuration *server, std::string RequestURI);
	std::string getRedir(std::string MethodUsed, server_configuration *server, std::string RequestURI);
	// Définition de la méthode pour obtenir le corps de la réponse
	std::string get_body() const { return _body; }

	// Définition de la méthode pour obtenir la réponse _ServerResponse
	std::string get_ServerResponse() const { return _ServerResponse; }

	// Définition de la méthode pour obtenir le code d'état de la réponse
	int get_status_code() const { return _status_code; }

	void		addType();
	std::string	getType(std::string type);
};

#endif