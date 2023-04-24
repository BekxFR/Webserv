/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_configuration.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgruson <mgruson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 11:03:12 by mgruson           #+#    #+#             */
/*   Updated: 2023/04/24 18:59:44 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_CONFIGURATION_HPP
#define SERVER_CONFIGURATION_HPP

#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <unistd.h>
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
	std::string _Index;
	std::vector<std::string>	_env;
	std::vector<std::string>	_HttpMethodAccepted;
	std::map<std::string, std::string>	_cgi;
	std::vector<int> _Port;
	std::vector<std::string>	_Host;
	int			_StatusCode;
	size_t		_ClientMaxBodySize;
	std::map<std::string, std::pair<std::string, std::string> >	_ErrorPage;
	std::map<std::string, std::pair<std::string, std::string> >	_DefErrorPage;
	std::map<std::string, std::string> _Location;
	std::map<std::string, class server_location_configuration*> _Loc;
	
	server_configuration();
	public:
	server_configuration(std::string ConfigFile, const char **);
	server_configuration(server_configuration const &obj);
	~server_configuration();
	server_configuration &operator=(server_configuration const &obj);

	std::string findServerName();
	std::vector<int> findPort();
	std::vector<std::string> findHost();
	std::string findRoot();

	std::string findIndex();
	std::map<std::string, std::string> findLocation();
	std::vector<std::string> findHttpMethodAccepted();

	void	setCgi();
	void	setErrorPage();
	void	setDefErrorPage();
	void	setStatusCode(int);
	int fillCgi(size_t pos);
	std::map<std::string, class server_location_configuration*> findLoc();
	size_t findClientMaxBodySize();

	std::ostream&	printLoc(std::ostream &out);
	
	template<class T>
	void	printMap(std::map<T,T>);

	std::string	getConfigFile() const;
	std::string	getServerName() const;
	int	getStatusCode() const;
	std::vector<std::string>	getEnv() const;
	std::map<std::string, std::string>	getCgi() const;
	std::map<std::string, std::string>&	getCgi();
	std::map<std::string, std::string>	getLocation() const;
	std::map<std::string, std::pair<std::string, std::string> >	getErrorPage() const;
	std::map<std::string, std::pair<std::string, std::string> >	getDefErrorPage() const;
	std::map<std::string, class server_location_configuration*>	getLoc() const;
	std::map<std::string, class server_location_configuration*>&	getLoc();
	std::string	getRoot() const;
	std::string	getIndex() const;
	std::vector<std::string>	getHttpMethodAccepted() const;
	std::vector<std::string>&	getHttpMethodAccepted();
	std::vector<int>	getPort() const;
	size_t	getClientMaxBodySize() const;
	std::vector<std::string>	getHost() const;

	
	class CgiException: public std::exception {
		public:
			virtual const char *	what() const throw();
	};
	class ErrorPageException: public std::exception {
		public:
			virtual const char *	what() const throw();
	};
};

std::ostream& operator <<(std::ostream &out, server_configuration &ServConfig);

#endif
