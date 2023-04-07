/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_configuration.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chillion <chillion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 11:03:12 by mgruson           #+#    #+#             */
/*   Updated: 2023/04/07 16:45:30 by nflan            ###   ########.fr       */
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
#include "ErrorCorresp.hpp"
#include <sstream>
#include <map>
#include "server_location_configuration.hpp"
#include <vector>
#define DEBUG 0

class ErrorCorresp;

class server_configuration
{
	private:
	std::string _ConfigFile;
	std::string _ServerName;
	std::string	_Root;
	std::map<std::string, std::string>	_cgi;
	int			_Port;
	int			_StatusCode;
	size_t		_ClientMaxBodySize;
	std::map<std::string, std::pair<std::string, std::string> >	_ErrorPage;
	std::map<std::string, std::pair<std::string, std::string> >	_DefErrorPage;
	std::map<std::string, std::string> _Location;
	std::map<std::string, class server_location_configuration*> _Loc;
	
	public:
	server_configuration();
	server_configuration(std::string ConfigFile);
	server_configuration(server_configuration const &obj);
	~server_configuration();
	server_configuration &operator=(server_configuration const &obj);

	std::string findServerName();
	int findPort();
	std::string findRoot();
	std::map<std::string, std::string> findLocation();
	void	setCgi();
	void	setErrorPage();
	void	setDefErrorPage();
	void	setStatusCode(int);
	int fillCgi(size_t pos);
	std::map<std::string, class server_location_configuration*> findLoc();

	std::ostream&	printLoc(std::ostream &out);
	
	template<class T>
	void	printMap(std::map<T,T>);

	size_t findClientMaxBodySize();
	std::string getConfigFile();
	std::string getServerName();
	int	getStatusCode();
	std::map<std::string, std::string>	getCgi();
	std::map<std::string, std::pair<std::string, std::string> >	getErrorPage();
	std::map<std::string, std::pair<std::string, std::string> >&	getDefErrorPage();
	std::map<std::string, class server_location_configuration*>& getLoc();
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
