/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_request.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgruson <mgruson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 14:29:37 by mgruson           #+#    #+#             */
/*   Updated: 2023/05/03 15:02:21 by mgruson          ###   ########.fr       */
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
		std::string							_ServerRequest;
		std::string							_Method; //old
		std::string							_method;
		std::string							_RequestURI; //old
		std::string							_path;
		std::string							_type;
		std::string							_query;
		std::string							_argsBrutes;
		std::string							_version;
		std::string							_host;
		std::string							_connectionType;
		std::string							_mimeType;
		std::string							_contentType;
		std::string							_contentLength;
		std::string							_body;
		bool								_isBody;
		server_request();

	public:
		server_request(std::string ServerRequest);
		server_request(server_request const &obj);
		~server_request();
		server_request &operator=(server_request const &obj);

		//GETTERS
		std::string	getServerRequest() const { return (_ServerRequest); }
		std::string	getMethod() const { return (_Method); }
		std::string	getRequestURI() const { return (_RequestURI); }
		std::string	getPath() const { return (_path); }
		std::string	getType() const { return (_type); }
		std::string	getVersion() const { return (_version); }
		std::string	getHost() const { return (_host); }
		std::string	getConnectionType() const { return (_connectionType); }
		std::string	getMimeType() const { return (_mimeType); }
		std::string	getContentType() const { return (_contentType); }
		std::string	getBody() const { return (_body); }
		bool		getIsBody() const { return (_isBody); }
		std::string	getQuery() const { return (_argsBrutes); }

		//OTHER
		std::string			findRequestURI();
		std::string			findMethod();
		unsigned long long	getContentLength() const ;
		void				request_parser();
		void				setType(std::string);
};

std::ostream& operator <<(std::ostream &out, server_request &ServRequest);

#endif
