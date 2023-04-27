/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_configuration.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chillion <chillion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 11:03:12 by mgruson           #+#    #+#             */
/*   Updated: 2023/04/26 20:28:08 by chillion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_CONFIGURATION_HPP
#define SERVER_CONFIGURATION_HPP

#include "lib.hpp"

class ErrorCorresp;

class server_configuration
{
	private:
		std::string													_ConfigFile;
		std::string													_ServerName;
		std::string													_Root;
		std::string													_Index;
		std::vector<std::string>									_env;
		std::vector<std::string>									_HttpMethodAccepted;
		std::map<std::string, std::string>							_cgi;
		std::vector<int>											_Port;
		std::vector<std::string>									_Host;
		std::vector<std::string>									_CookieHeader;
		std::string													_DirectoryListing;
		int															_StatusCode;
		size_t														_ClientMaxBodySize;
		std::map<std::string,std::pair<std::string,std::string> >	_ErrorPage;
		std::map<std::string,std::pair<std::string,std::string> >	_DefErrorPage;
		std::map<std::string,std::string>							_Location;
		std::map<std::string,class server_location_configuration*>	_Loc;

		server_configuration();

	public:
		server_configuration(std::string ConfigFile, const char **);
		server_configuration(server_configuration const &obj);
		~server_configuration();
		server_configuration &operator=(server_configuration const &obj);

		// GETTERS
		std::string														getConfigFile() const { return _ConfigFile;}
		std::string														getServerName() const { return _ServerName;}
		std::string														getRoot() const { return _Root;}
		std::string														getIndex() const { return _Index;}
		std::vector<int>												getPort() const { return _Port;}
		std::vector<std::string>										getEnv() const { return _env;}
		std::vector<std::string>										getHttpMethodAccepted() const { return (_HttpMethodAccepted); }
		std::vector<std::string>&										getHttpMethodAccepted() { return (_HttpMethodAccepted); }
		std::vector<std::string>										getHost() const { return _Host;}
		size_t															getClientMaxBodySize() const { return _ClientMaxBodySize;}
		std::map<std::string, std::string>								getCgi() const { return (_cgi); }
		std::map<std::string, std::string>&								getCgi() { return (_cgi); }
		std::map<std::string, std::string>								getLocation() const { return (_Location); }
		std::map<std::string, std::pair<std::string, std::string> >		getErrorPage() const { return _ErrorPage;}
		std::map<std::string, std::pair<std::string, std::string> >		getDefErrorPage() const { return _DefErrorPage;}
		std::map<std::string, class server_location_configuration*>		getLoc() const { return (_Loc);}
		// std::map<std::string, class server_location_configuration*>* 	getLoc() { return (&_Loc);}
		std::map<std::string, class server_location_configuration*>&	getLoc() { return (_Loc);}
		std::vector<std::string> 										getCookieHeader() { return _CookieHeader;}
		int																getStatusCode()	const { return (_StatusCode); }
		std::string														getDirectoryListing() { return (_DirectoryListing);}

		// SETTER
		void														setCgi();
		void														setErrorPage();
		void														setDefErrorPage();
		void														setStatusCode(int);
		size_t														findClientMaxBodySize();
		std::string													findElement(std::string);
//		std::string													findServerName();
//		std::string													findRoot();
//		std::string													findIndex();
		std::vector<int>											findPort();
		std::vector<std::string>									findHost();
		std::vector<std::string>									findHttpMethodAccepted();
		std::map<std::string, std::string>							findLocation();
		std::map<std::string, class server_location_configuration*>	findLoc();
		std::vector<std::string>									findCookieHeader();
		std::string													findDirectoryListing();


		//OTHER
		int		fillCgi(size_t pos);
		bool	is_in_location(size_t conf_pos, std::string str);

		//PRINT
		std::ostream&	printLoc(std::ostream &out);

		//EXCEPTION
		class CgiException: public std::exception {
			public:
				virtual const char *	what() const throw() { return ("CGI parsing error\n"); }
		};
		class ErrorPageException: public std::exception {
			public:
				virtual const char *	what() const throw() { return ("Error Page parsing error\n"); }
		};
		class RootException: public std::exception {
			public:
				virtual const char *	what() const throw() { return ("Error Root parsing error\n"); }
		};
};

std::ostream& operator <<(std::ostream &out, server_configuration &ServConfig);

#endif
