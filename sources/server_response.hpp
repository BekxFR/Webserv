/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_response.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgruson <mgruson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 15:09:26 by mgruson           #+#    #+#             */
/*   Updated: 2023/05/01 17:37:43 by mgruson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_RESPONSE_HPP
#define SERVER_RESPONSE_HPP

#include "lib.hpp"

class server_configuration;

class server_response
{
	private:
	int									_status_code;
	int									_cgiFd;
	std::string							_header;
	std::string							_body;
	std::string							_content;
	size_t								_contentLength;
	std::string							_ServerResponse;
	std::string							_finalPath;
	std::string							_fileName;
	std::vector<std::string>			_env;
	server_request*						_req;
	std::map<std::string, std::string>	_contentType;

	server_response();
	
	public:
	server_response(int, std::vector<std::string>, server_request*);
	server_response(server_response const &obj);
	~server_response();
	server_response &operator=(server_response const &obj);

	// GETTERS
	int									getCgiFd() const { return (_cgiFd); }
	int									getStatusCode() const { return _status_code; }
	size_t								getContentLength() const { return _contentLength; }
	std::string							getFileName() { return (_fileName); }
	std::string							getHeader() const { return _header; }
	std::string							getBody() const { return _body; }
	std::string							getContent() const { return (_content); }
	std::string							getServerResponse() const { return (_ServerResponse); }
	std::string							getPath() const { return (_finalPath); }
	std::string							getType(std::string type);
	server_request*						getReq() const { return (_req); }
	std::vector<std::string>			getEnv() const { return (_env); }
	std::vector<std::string>&			getEnv() { return (_env); }
	std::map<std::string, std::string>	getContentType() const { return (_contentType); }

	// OTHER
	void		SendingResponse(const server_request& Server_Request, int socket, server_configuration* Root, int StatusCodeTmp);
	void		addLength();
	void		setStatusCode(int st) { _status_code = (st); }
	void		addType();
	void		createResponse(server_configuration*, std::string, const server_request& Server_Request, int IdSession);
	void		delete_dir(const char * path);
	std::string	getRedir(std::string MethodUsed, server_configuration *server, std::string RequestURI);
	std::string	list_dir(std::string path);
	std::string	addHeader(std::string statusMsg, std::pair<std::string, std::string> statusContent, const server_request& Server_Request, server_configuration *server, int IdSession);
	std::string	addBody(std::string body);
	std::string	getRealPath(std::string MethodUsed, server_configuration *server, std::string RequestURI);
	std::string	getRealPathIndex(std::string MethodUsed, server_configuration *server, std::string RequestURI);
	int			doCgi(std::string toexec, server_configuration * server); // envoyer fichier a cgiser + return fd du cgi
	bool		isRedir(std::string MethodUsed, server_configuration *server, std::string RequestURI);
	bool		autoindex_is_on(std::string MethodUsed, server_configuration *server, std::string RequestURI);
	bool		AnswerGet(const server_request& Server_Request, server_configuration *server);
	void		SendingPostResponse(const server_request& Server_Request, int conn_sock, server_configuration *server, std::string PostContent, std::string filename);
	int			getIdSessionOrSetError401(const server_request& Server_Request);
	static void		*download_file(void *arg);
	// Définition de la méthode pour obtenir le corps de la réponse
	// Définition de la méthode pour obtenir la réponse _ServerResponse
	// Définition de la méthode pour obtenir le code d'état de la réponse
};

#endif
