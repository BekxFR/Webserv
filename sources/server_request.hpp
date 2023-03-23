/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_request.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgruson <mgruson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 14:29:37 by mgruson           #+#    #+#             */
/*   Updated: 2023/03/23 15:30:32 by mgruson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_REQUEST_HPP
#define SERVER_REQUEST_HPP

#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <algorithm>

#define DEBUG 0

class server_request
{
	private:
	std::string _ServerRequest;
	std::string _Method;
	std::string _PathToFile;
	std::string _Host;
	std::string _ContentType;
	unsigned int _ContentLength;
	server_request();

	public:
	server_request(std::string ServerRequest);
	server_request(server_request const &obj);
	~server_request();
	server_request &operator=(server_request const &obj);
	
	std::string findPathToFile();
	std::string findMethod();
	std::string findContentType();
	unsigned int findContentLength();
	std::string getServerRequest() const;
	std::string getMethod() const;
	std::string getPathToFile() const;
	std::string getContentType() const;
	unsigned int getContentLength() const;
};

std::ostream& operator <<(std::ostream &out, server_request &ServRequest);

#endif