/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_response.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgruson <mgruson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 15:09:26 by mgruson           #+#    #+#             */
/*   Updated: 2023/04/25 18:49:03 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_RESPONSE_HPP
#define SERVER_RESPONSE_HPP

#include "lib.hpp"

class server_configuration;

class server_response
{
	private:
	int			_status_code;
	int			_cgiFd;
	std::string	_header;
	std::string	_body;
	std::string	_content;
	size_t	_contentLength;
	std::string	_ServerResponse;
	std::string	_finalPath;
	std::string	_fileName;
	std::vector<std::string>	_env;
	server_request*	_req;
	std::map<std::string, std::string> _contentType;

	server_response();
	
	public:
	server_response(int, std::vector<std::string>, server_request*);
	server_response(server_response const &obj);
	~server_response();
	server_response &operator=(server_response const &obj);

	void	todo(const server_request& Server_Request, int socket, server_configuration* Root);
	void	createResponse(server_configuration*, std::string, const server_request& Server_Request);
	void	delete_dir(const char * path);
	std::string	list_dir(std::string path);
	std::string	addHeader(std::string statusMsg, std::pair<std::string, std::string> statusContent, const server_request& Server_Request);
	std::string	addBody(std::string body);
	int isMethodAuthorised(std::string MethodUsed, server_configuration *server, std::string RequestURI);
	std::string getRealPath(std::string MethodUsed, server_configuration *server, std::string RequestURI);
	std::string getRealPathIndex(std::string MethodUsed, server_configuration *server, std::string RequestURI);
	std::string getPathToStore(std::string MethodUsed, server_configuration *server, std::string RequestURI);
	bool isRedir(std::string MethodUsed, server_configuration *server, std::string RequestURI);
	bool autoindex_is_on(std::string MethodUsed, server_configuration *server, std::string RequestURI);
	int	doCgi(std::string toexec, server_configuration * server); // envoyer fichier a cgiser + return fd du cgi
	void	addLength();

	std::string getRedir(std::string MethodUsed, server_configuration *server, std::string RequestURI);
	// Définition de la méthode pour obtenir le corps de la réponse
	std::string	getHeader() const { return _header; }
	std::string	getBody() const { return _body; }
	std::string	getContent() const { return (_content); }
	size_t	getContentLength() const { return _contentLength; }
	std::string	getServerResponse() const { return (_ServerResponse); }
	std::string	getPath() const { return (_finalPath); }
	server_request*	getReq() const { return (_req); }
	std::vector<std::string>	getEnv() const { return (_env); }
	std::vector<std::string>&	getEnv() { return (_env); }
	std::map<std::string, std::string>	getContentType() const { return (_contentType); }
	// Définition de la méthode pour obtenir la réponse _ServerResponse
	std::string get_ServerResponse() const { return _ServerResponse; }

	bool	AnswerGet(const server_request& Server_Request, server_configuration *server);

	// Définition de la méthode pour obtenir le code d'état de la réponse
	int	getCgiFd() const { return (_cgiFd); }
	int getStatusCode() const { return _status_code; }
	void		setStatusCode(int st) { _status_code = (st); }
	std::string	getFileName()	{ return (_fileName); }

	void		addType();
	std::string	getType(std::string type);

};

#endif
