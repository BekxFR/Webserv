/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_response.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgruson <mgruson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 15:09:26 by mgruson           #+#    #+#             */
/*   Updated: 2023/04/24 17:41:39 by mgruson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_RESPONSE_HPP
#define SERVER_RESPONSE_HPP

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include <vector>
#include <iterator>
#include <arpa/inet.h>
#include <fcntl.h>
#include <csignal>
#include <fstream>
#include <sstream>
#include <cstring>
#include <ftw.h>
#include <filesystem>
#include <errno.h>
#include <iostream>
#include <sys/stat.h>
#include <ctime>
#include <cstdlib>
#include <climits>
#include "server_configuration.hpp"
#include "server_request.hpp"

class server_request;

class server_response
{
	private:
	int			_status_code;
	std::string	_body;
	std::string	_content;
	std::string	_ServerResponse;
	std::map<std::string, std::string> _contentType;
	
	public:
	server_response();
	server_response(int);
	server_response(server_response const &obj);
	~server_response();
	server_response &operator=(server_response const &obj);

	void	todo(const server_request& Server_Request, int conn_sock, server_configuration* Root);
	void	createResponse(server_configuration*, std::string, const server_request& Server_Request, int IdSession);
	void	delete_dir(const char * path);
	std::string	list_dir(std::string path);
	std::string	addHeader(std::string statusMsg, std::pair<std::string, std::string> statusContent, const server_request& Server_Request, server_configuration *server, int IdSession);
	std::string	addBody(std::string body);
	int isMethodAuthorised(std::string MethodUsed, server_configuration *server, std::string RequestURI);
	std::string getRealPath(std::string MethodUsed, server_configuration *server, std::string RequestURI);
	std::string getRealPathIndex(std::string MethodUsed, server_configuration *server, std::string RequestURI);
	std::string getPathToStore(std::string MethodUsed, server_configuration *server, std::string RequestURI);
	bool isRedir(std::string MethodUsed, server_configuration *server, std::string RequestURI);
	bool autoindex_is_on(std::string MethodUsed, server_configuration *server, std::string RequestURI);
	int	getIdSessionOrSetError401(const server_request& Server_Request);

	std::string getRedir(std::string MethodUsed, server_configuration *server, std::string RequestURI);
	// Définition de la méthode pour obtenir le corps de la réponse
	std::string get_body() const { return _body; }

	// Définition de la méthode pour obtenir la réponse _ServerResponse
	std::string get_ServerResponse() const { return _ServerResponse; }

	// Définition de la méthode pour obtenir le code d'état de la réponse
	int get_status_code() const { return _status_code; }
	void		setStatusCode(int st) { _status_code = (st); }

	void		addType();
	std::string	getType(std::string type);

};

#endif
