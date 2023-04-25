/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_request.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chillion <chillion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 14:29:37 by mgruson           #+#    #+#             */
/*   Updated: 2023/04/25 18:49:35 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_REQUEST_HPP
#define SERVER_REQUEST_HPP

#include "lib.hpp"

#define DEBUG 0

class server_configuration;

class server_request
{
	private:
	std::string _ServerRequest;
	std::string _Method; //old
	std::string _method;
	std::string _RequestURI; //old
	std::string _path;
	std::string _type;
	std::string _version;
	std::string _host;
	std::string _connectionType;
	std::string _mimeType;
	std::string _contentType;
	std::string _contentLength;
	std::string _body;
	std::vector<server_configuration*>	_allServers;

	server_request();

	public:
	server_request(std::string ServerRequest);
	server_request(server_request const &obj);
	~server_request();
	server_request &operator=(server_request const &obj);
	
	std::string findRequestURI();
	std::string findMethod();
	std::string	getServerRequest() const;
	std::string	getMethod() const;
	std::string	getRequestURI() const;
	std::string	getPath() const;
	std::string	getType() const;
	std::string	getVersion() const;
	std::string	getHost() const;
	std::string	getConnectionType() const;
	std::string	getMimeType() const;
	std::string	getContentType() const;
	std::string	getBody() const;
	unsigned long long getContentLength() const ;
	std::vector<server_configuration*>&	getAllServers();
	void request_parser();

	void setType(std::string);
	void	setAllServers(std::vector<server_configuration*>);
};

std::ostream& operator <<(std::ostream &out, server_request &ServRequest);

#endif
