/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_configuration.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgruson <mgruson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 11:03:12 by mgruson           #+#    #+#             */
/*   Updated: 2023/04/04 19:04:18 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_CONFIGURATION_HPP
#define SERVER_CONFIGURATION_HPP

#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "default_error.hpp"
#include <sstream>
#include <map>

#define DEBUG 0

class server_configuration
{
	private:
	std::string _ConfigFile;
	std::string _ServerName;
	std::string	_Root;
	std::map<std::string, std::string>	_cgi;
	int			_Port;
	size_t		_ClientMaxBodySize;
	std::map<std::string, std::string>	_ErrorPage;
	std::map<std::string, std::string>	_DefErrorPage;
	
	public:
	server_configuration();
	server_configuration(std::string ConfigFile);
	server_configuration(server_configuration const &obj);
	~server_configuration();
	server_configuration &operator=(server_configuration const &obj);

	std::string convertConfigFile(std::string ConfigFile);
	std::string findServerName();
	int findPort();
	std::string findRoot();
	void	setCgi();
	void	setErrorPage();
	void	setDefErrorPage();
	int fillCgi(size_t pos);

	template<class T>
	void	printMap(std::map<T,T>);

	size_t findClientMaxBodySize();
	std::string findErrorPage();
	std::string getConfigFile();
	std::string getServerName();
	std::map<std::string, std::string>	getCgi();
	std::map<std::string, std::string>	getErrorPage();
	std::map<std::string, std::string>&	getDefErrorPage();
	std::string getRoot();
	int getPort();

	class CgiException: public std::exception {
		public:
			virtual const char *	what() const throw();
	};
	class ErrorPageException: public std::exception {
		public:
			virtual const char *	what() const throw();
	};
	size_t getClientMaxBodySize();
};

std::ostream& operator <<(std::ostream &out, server_configuration &ServConfig);

#endif
