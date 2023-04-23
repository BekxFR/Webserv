/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_configuration.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgruson <mgruson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 11:06:26 by mgruson           #+#    #+#             */
/*   Updated: 2023/04/23 15:28:21 by mgruson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server_configuration.hpp"

server_configuration::server_configuration()
{
	if (DEBUG)
		std::cout << "server_configuration Default Constructor called" << std::endl;
}

server_configuration::server_configuration(std::string ConfigFile) : 
_ConfigFile(ConfigFile),
_ServerName(findServerName()),
_Root(findRoot()),
_Index(findIndex()),
_HttpMethodAccepted(findHttpMethodAccepted()),
_Port(findPort()),
_Host(findHost()),
_CookieHeader(findCookieHeader()),
_DirectoryListing(findDirectoryListing()),
_StatusCode(200),
_ClientMaxBodySize(findClientMaxBodySize()),
_Location(findLocation()),
_Loc(findLoc())
{
	setCgi();
	setDefErrorPage();
	setErrorPage();
	// deferrorpage -> errorpage
	ErrorCorresp	check;
	std::map<std::string, std::pair<std::string, std::string> >::iterator	tmp;
	for (std::map<std::string, std::pair<std::string, std::string> >::iterator it = _DefErrorPage.begin(); it != _DefErrorPage.end(); it++)
	{
		tmp = _ErrorPage.find(check.getCorresp().find(it->first)->second);
		if (tmp == _ErrorPage.end())
			_ErrorPage.insert(*it);
		else
		{
			_ErrorPage.insert(std::make_pair(it->first, tmp->second));
			_ErrorPage.erase(tmp);
		}
	}
	if (DEBUG)
	{
		std::cout << "server_configuration Overload Constructor called" << std::endl;
		std::cout << "server_configuration::server_configuration(std::string ConfigFile)\n" << ConfigFile << std::endl;
		std::cout << "Server name " << this->_ServerName << std::endl;
	}
}

server_configuration::server_configuration(server_configuration const &obj)
{
	*this = obj;
}

server_configuration::~server_configuration()
{
	if (DEBUG)
		std::cout << "server_configuration Destructor called" << std::endl;
}

server_configuration &server_configuration::operator=(server_configuration const &obj)
{
	if (DEBUG)
		std::cout << "server_configuration Copy assignment operator called" << std::endl;
	(void)obj;
	return *this;
}

int	server_configuration::getStatusCode() { return (_StatusCode); }
void	server_configuration::setStatusCode(int nb) { _StatusCode = nb; }

std::string server_configuration::findServerName()
{
	size_t pos = _ConfigFile.find("server_name"); // find the position of "server_name" in the string
	if (pos != std::string::npos) { // check if "server_name" was found
		pos += strlen("server_name"); // move the position to the end of "server_name"
		std::string server_name = _ConfigFile.substr(pos + 1); // extract the substring starting from the next character
		size_t space_pos = server_name.find_first_of(" \n;"); // find the position of the first space or newline character
		if (space_pos != std::string::npos) { // check if a space character was found
			if (DEBUG)
				std::cout << "std::string server_configuration::findServerName() " << server_name.substr(0, space_pos) << std::endl;
			return(server_name.substr(0, space_pos)); // extract the substring before the space character
		}
	}
	return ("");
}

bool server_configuration::is_in_location(size_t conf_pos, std::string str)
{
	size_t pos = 0;
	size_t end_loc = 0;
	// std::cout << "TEST\n\n" << "\n\n" << conf_pos << std::endl;
	while (pos != std::string::npos)
		{
			// std::cout << "c0" << std::endl;
			pos = str.find("location /", pos);
			if (pos != std::string::npos)
			{
				// std::cout << "c1" << std::endl;
				pos += strlen("location /"); 
				pos = str.find_first_of("{", pos); 
				if (pos == std::string::npos) 
					return 0;
				end_loc = str.find_first_of("{}", pos + 1);
				int j = 0;
				while (str[end_loc] == '{')
				{
					// std::cout << "c2" << std::endl;
					j++;
					end_loc = str.find_first_of("{}", end_loc + 1);
				}
				if (end_loc == std::string::npos)
				{
					// std::cout << "NO END LOC" << std::endl;
					return 0;
				}
				if (conf_pos > pos && conf_pos < end_loc)
				{
					// std::cout << "IN LOC" << std::endl;
					return (1);
				}
				pos = end_loc;
				end_loc = 0;
			}
			else
			{
				// std::cout << "NO LOC" << std::endl;
				return 0;
			}
		}
		// std::cout << "NO = LOC" << std::endl;

		return (0);
}

std::string server_configuration::findRoot()
{
	size_t pos = _ConfigFile.find("root");
	if (pos != std::string::npos) {
		pos += strlen("root");
		std::string root = _ConfigFile.substr(pos + 1);
		size_t space_pos = root.find_first_of(" \n;");
		if (space_pos != std::string::npos) {
			return (root.substr(0, space_pos));
		}
	}
	return ("");
}

std::string server_configuration::findIndex()
{
	size_t pos = _ConfigFile.find("index");
	if (pos != std::string::npos) {
		pos += strlen("index");
		std::string index = _ConfigFile.substr(pos + 1);
		size_t space_pos = index.find_first_of(" \n;");
		if (space_pos != std::string::npos) {
			return (index.substr(0, space_pos));
		}
	}
	return ("");
}

int server_configuration::fillCgi(size_t pos)
{
	size_t tmp = 0;
	std::pair<std::string, std::string> cgi_pair;
	for (tmp = pos; _ConfigFile[pos] != ' ' && _ConfigFile[pos] != ';' && _ConfigFile[pos] != '\n'; pos++) {}
	cgi_pair.first = _ConfigFile.substr(tmp, pos - tmp);
	std::cout << "c1 " << cgi_pair.first << std::endl;
	for (; _ConfigFile[pos] == ' '; pos++) {}
	if (_ConfigFile[pos] == ';')
		throw CgiException();
	for (tmp = pos; _ConfigFile[pos] != ' ' && _ConfigFile[pos] != ';'; pos++) {}
	cgi_pair.second = _ConfigFile.substr(tmp, pos - tmp);
	std::cout << "c0 " << cgi_pair.second << std::endl;
	_cgi.insert(cgi_pair);
	return (pos + 1);
}

void server_configuration::setCgi()
{
	// std::cout << "c-1\n" << _ConfigFile << std::endl;
	size_t pos = 0;
	while (is_in_location(_ConfigFile.find("cgi", pos + 1), _ConfigFile))
	{
		// std::cout << "IS_IN_LOC" << std::endl;
		pos = _ConfigFile.find("cgi", pos + 1);
	}
	pos = _ConfigFile.find("cgi", pos + 1);
	if (pos == std::string::npos)
		return ;
	pos += strlen("cgi");
	for (; _ConfigFile[pos] == ' '; pos++) {}
	if (_ConfigFile[pos] == ';' || _ConfigFile[pos] != '.')
		throw CgiException();
	for (; _ConfigFile[pos] != ';' && _ConfigFile[pos] != '\n';)
		pos = fillCgi(pos);
}

std::string	readingFileEP( std::string file )
{
	std::ifstream input_file(file.c_str());

	if (!input_file.is_open()) {
		if (0)
			std::cout << "Can't open file " << file << " using default error page" << std::endl;
		return ("");
	}

	std::stringstream buffer;
	std::string fileContent;

	buffer << input_file.rdbuf();
	fileContent = buffer.str();
//	std::getline(input_file, fileContent, '\0');
	input_file.close();
	return (fileContent.substr(0, fileContent.size()));
}

void server_configuration::setErrorPage()
{
	std::string	first;
	std::string	second;
	std::string	file;

	size_t pos = _ConfigFile.find("error_page");

	if (pos == std::string::npos)
		return ;
	pos += strlen("error_page");
	for (; _ConfigFile[pos] == ' '; pos++) {}
	if (_ConfigFile[pos] == ';')
		throw ErrorPageException();
	for (size_t tmp = 0; _ConfigFile[pos] != ';' && _ConfigFile[pos] != '\n'; tmp = 0, pos++)
	{
		for (tmp = pos; _ConfigFile[pos] != ' ' && _ConfigFile[pos] != ';' && _ConfigFile[pos] != '\n'; pos++) {}
		first = _ConfigFile.substr(tmp, pos - tmp);
		for (; _ConfigFile[pos] == ' '; pos++) {}
		if (_ConfigFile[pos] == ';')
			throw ErrorPageException();
		for (tmp = pos; _ConfigFile[pos] != ' ' && _ConfigFile[pos] != ';'; pos++) {}
		file = _ConfigFile.substr(tmp, pos - tmp);
		second = readingFileEP(file);
		if (second.size() > 0)
		{
			_ErrorPage.insert(std::make_pair(first, std::make_pair(file, second)));
		}
	}
}

void server_configuration::setDefErrorPage()
{
	_DefErrorPage.insert(std::make_pair(STATUS100, std::make_pair("", HTML100)));
	_DefErrorPage.insert(std::make_pair(STATUS101, std::make_pair("", HTML101)));
	_DefErrorPage.insert(std::make_pair(STATUS200, std::make_pair("", HTML200)));
	_DefErrorPage.insert(std::make_pair(STATUS201, std::make_pair("", HTML201)));
	_DefErrorPage.insert(std::make_pair(STATUS202, std::make_pair("", HTML202)));
	_DefErrorPage.insert(std::make_pair(STATUS203, std::make_pair("", HTML203)));
	_DefErrorPage.insert(std::make_pair(STATUS204, std::make_pair("", HTML204)));
	_DefErrorPage.insert(std::make_pair(STATUS205, std::make_pair("", HTML205)));
	_DefErrorPage.insert(std::make_pair(STATUS206, std::make_pair("", HTML206)));
	_DefErrorPage.insert(std::make_pair(STATUS300, std::make_pair("", HTML300)));
	_DefErrorPage.insert(std::make_pair(STATUS301, std::make_pair("", HTML301)));
	_DefErrorPage.insert(std::make_pair(STATUS302, std::make_pair("", HTML302)));
	_DefErrorPage.insert(std::make_pair(STATUS303, std::make_pair("", HTML303)));
	_DefErrorPage.insert(std::make_pair(STATUS304, std::make_pair("", HTML304)));
	_DefErrorPage.insert(std::make_pair(STATUS305, std::make_pair("", HTML305)));
	_DefErrorPage.insert(std::make_pair(STATUS307, std::make_pair("", HTML307)));
	_DefErrorPage.insert(std::make_pair(STATUS400, std::make_pair("", HTML400)));
	_DefErrorPage.insert(std::make_pair(STATUS401, std::make_pair("", HTML401)));
	_DefErrorPage.insert(std::make_pair(STATUS402, std::make_pair("", HTML402)));
	_DefErrorPage.insert(std::make_pair(STATUS403, std::make_pair("", HTML403)));
	_DefErrorPage.insert(std::make_pair(STATUS404, std::make_pair("", HTML404)));
	_DefErrorPage.insert(std::make_pair(STATUS405, std::make_pair("", HTML405)));
	_DefErrorPage.insert(std::make_pair(STATUS406, std::make_pair("", HTML406)));
	_DefErrorPage.insert(std::make_pair(STATUS407, std::make_pair("", HTML407)));
	_DefErrorPage.insert(std::make_pair(STATUS408, std::make_pair("", HTML408)));
	_DefErrorPage.insert(std::make_pair(STATUS409, std::make_pair("", HTML409)));
	_DefErrorPage.insert(std::make_pair(STATUS410, std::make_pair("", HTML410)));
	_DefErrorPage.insert(std::make_pair(STATUS411, std::make_pair("", HTML411)));
	_DefErrorPage.insert(std::make_pair(STATUS412, std::make_pair("", HTML412)));
	_DefErrorPage.insert(std::make_pair(STATUS413, std::make_pair("", HTML413)));
	_DefErrorPage.insert(std::make_pair(STATUS414, std::make_pair("", HTML414)));
	_DefErrorPage.insert(std::make_pair(STATUS415, std::make_pair("", HTML415)));
	_DefErrorPage.insert(std::make_pair(STATUS416, std::make_pair("", HTML416)));
	_DefErrorPage.insert(std::make_pair(STATUS417, std::make_pair("", HTML417)));
	_DefErrorPage.insert(std::make_pair(STATUS500, std::make_pair("", HTML500)));
	_DefErrorPage.insert(std::make_pair(STATUS501, std::make_pair("", HTML501)));
	_DefErrorPage.insert(std::make_pair(STATUS502, std::make_pair("", HTML502)));
	_DefErrorPage.insert(std::make_pair(STATUS503, std::make_pair("", HTML503)));
	_DefErrorPage.insert(std::make_pair(STATUS504, std::make_pair("", HTML504)));
	_DefErrorPage.insert(std::make_pair(STATUS505, std::make_pair("", HTML505)));
//	for (std::map<std::string, std::string>::iterator it = _DefErrorPage.begin(); it != _DefErrorPage.end(); it++) // Print Def error pages
//		std::cout << "Error code = '" << it->first << "' && Error HTML = '" << it->second << "'" << std::endl; // Print Def error pages
}
std::vector<std::string> server_configuration::findHttpMethodAccepted()
{
	std::vector<std::string> MethodAccepted;
	std::string delimiter = " ;";
	std::string methods;
	size_t end_pos = 0;
		
	size_t pos = _ConfigFile.find("	allow_methods ");
	if (pos != std::string::npos) {
		pos += strlen("	allow_methods ");
		methods = _ConfigFile.substr(pos);
		end_pos = methods.find_first_of(";");
	}
	else
	{
		MethodAccepted.push_back("");
		return (MethodAccepted);
	}
	size_t i = 0;
	std::string token;
	while ((i = methods.find_first_of(delimiter)) != std::string::npos && methods.find_first_of(delimiter) <= end_pos)
	{
		token = methods.substr(0, i);
		if (0)
			std::cout << "TOKEN : " << token << std::endl;
		MethodAccepted.push_back(token);
		methods.erase(0, i + 1);
	}
	MethodAccepted.push_back(methods);
		return (MethodAccepted);
}

std::vector<std::string> server_configuration::findHost()
{
	size_t pos = 0;
	size_t end_pos = 0;
	size_t pos_colon = -1;
	std::vector<std::string> Host;
	while (pos != std::string::npos)
	{
		if (_ConfigFile.find("	listen ", pos) != std::string::npos)
		{
			pos = _ConfigFile.find("	listen ", pos) + strlen("	listen ");
			std::string host = _ConfigFile.substr(pos);
			end_pos = host.find_first_of(" ;");
			if (host.find_first_of(":", pos_colon + 1) != std::string::npos && host.find_first_of(":", pos_colon + 1 ) < end_pos)
			{	
				while (host.find_first_of(":", pos_colon + 1) != std::string::npos && host.find_first_of(":", pos_colon + 1 ) < end_pos)
				{
					pos_colon = host.find_first_of(":", pos_colon + 1);
				}
			}
			else
				pos_colon = end_pos + 1;
			if (0)
			{
				std::cout << "HOST :" << host << std::endl;
				std::cout << "POS_COLON : " << pos_colon << std::endl;
				std::cout << "END_POS : " << end_pos << std::endl;
			}
			if (end_pos != std::string::npos && (pos_colon == std::string::npos || pos_colon > end_pos))
			{
				Host.push_back("");
			}
			else if (end_pos != std::string::npos && pos_colon != std::string::npos && pos_colon < end_pos)
			{
				if (0)
					std::cout << "server_configuration::findhost() 2" << host.substr(0, pos_colon) << std::endl;
				Host.push_back(host.substr(0, pos_colon).c_str());
			}
		}
		else
			pos = _ConfigFile.find("	listen ", pos);
		pos = pos + end_pos;
		end_pos = 0;
		pos_colon = -1;
	}
	return (Host);
}

std::vector<std::string>	server_configuration::findCookieHeader()
{
	size_t pos = 0;
	size_t pos2 = 1;
	size_t mid_pos = 0;
	std::vector<std::string> Cookie;

	if (_ConfigFile.find("add_header Set-Cookie ") != std::string::npos)
	{
		pos = _ConfigFile.find("add_header Set-Cookie \"") + strlen("add_header Set-Cookie ");
		std::string CookieConf = _ConfigFile.substr(pos);
		// std::cout << "c1 " << CookieConf.at(CookieConf.find_first_of(";\"", mid_pos + 1)) << std::endl;
		while (CookieConf.at(CookieConf.find_first_of(";\"", mid_pos + 1)) != '"')
		{
			// std::cout << "c2" << std::endl;
			pos2 = CookieConf.find_first_of("_0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", mid_pos);
			mid_pos = CookieConf.find_first_of(";\"", mid_pos + 1);
			// std::cout << "c2.1 " << mid_pos << " " << CookieConf.at(mid_pos) << std::endl;
			if (CookieConf.at(mid_pos) == ';')
			{
				// std::cout << "c3 " << CookieConf.substr(pos2, (mid_pos - pos2)) << std::endl;
				// std::cout << "c4 " << pos2 << std::endl;
				Cookie.push_back(CookieConf.substr(pos2, (mid_pos - pos2)));
			
			}
			pos2 = mid_pos;
		}
	}
	// int i = 0;
	// std::cout << "\nCOOKIE HEADER\n" << std::endl;
	// for (std::vector<std::string>::iterator it = Cookie.begin(); it != Cookie.end(); it++)
	// {
	// 	std::cout << i++ << std::endl;
	// 	std::cout << *it << std::endl;
	// }
	return (Cookie);
}

std::string server_configuration::findDirectoryListing()
{
	size_t pos = 0;
	// std::cout << "START" << std::endl;
	while (is_in_location(_ConfigFile.find("autoindex", pos + 1), _ConfigFile))
	{
		// std::cout << "IS_IN_LOC" << std::endl;
		pos = _ConfigFile.find("autoindex", pos + 1);
	}
	pos = _ConfigFile.find("autoindex", pos + 1);
	// std::cout << "END" << std::endl;
	pos = _ConfigFile.find("autoindex", pos);
	if (pos != std::string::npos) {
		pos += strlen("autoindex");
		std::string root = _ConfigFile.substr(pos + 1);
		size_t space_pos = root.find_first_of(" \n;");
		std::cout << "TEST sur OFF ON " << root.substr(0, space_pos) << std::endl;
		if (space_pos != std::string::npos) {
			return (root.substr(0, space_pos));
		}
	}
	return ("off");
}

std::vector<int> server_configuration::findPort()
{
	size_t pos = 0;
	size_t end_pos = 0;
	size_t pos_colon = -1;
	std::vector<int> Port;
	while (pos != std::string::npos)
	{
		if (_ConfigFile.find("	listen ", pos) != std::string::npos)
		{
			pos = _ConfigFile.find("	listen ", pos) + strlen("	listen ");
			std::string port = _ConfigFile.substr(pos);
			if (0)
				std::cout << "\nTest :" << port << std::endl;
			end_pos = port.find_first_of(" ;");
			if (port.find_first_of(":", pos_colon + 1) != std::string::npos && port.find_first_of(":", pos_colon + 1 ) < end_pos)
			{	
				while (port.find_first_of(":", pos_colon + 1) != std::string::npos && port.find_first_of(":", pos_colon + 1 ) < end_pos)
				{
					pos_colon = port.find_first_of(":", pos_colon + 1);
				}
			}
			else
				pos_colon = end_pos + 1;
			
			if (end_pos != std::string::npos && (pos_colon == std::string::npos || pos_colon > end_pos))
			{
				if (0)
					std::cout << "server_configuration::findPort() 1:" << port.substr(0, end_pos).c_str() << std::endl;
				Port.push_back(atoi(port.substr(0, end_pos).c_str()));
			}
			else if (end_pos != std::string::npos && pos_colon != std::string::npos && pos_colon < end_pos)
			{
				if (0)
					std::cout << "server_configuration::findPort() 2:" << port.substr(pos_colon + 1, (end_pos - (pos_colon + 1))).c_str() << std::endl;
				Port.push_back(atoi(port.substr(pos_colon + 1, (end_pos - (pos_colon + 1))).c_str()));
			}
		}
		else
			pos = _ConfigFile.find("	listen ", pos);
		pos = pos + end_pos;
		end_pos = 0;
		pos_colon = -1;
	}
	return (Port);
}

size_t server_configuration::findClientMaxBodySize()
{
	size_t pos = _ConfigFile.find("client_max_body_size");
	if (pos != std::string::npos) {
		pos += strlen("client_max_body_size");
		std::string port = _ConfigFile.substr(pos + 1);
		size_t space_pos = port.find_first_of(" \n;");
		if (space_pos != std::string::npos) {
			if (DEBUG)
				std::cout << "server_configuration::findPort() " << port.substr(0, space_pos).c_str() << std::endl;
			int ClientBodySize = atoi(port.substr(0, space_pos).c_str());
			if (DEBUG)
				std::cout << "DEBUG : " << port.at(space_pos - 1) << std::endl;
			if (port.at(space_pos - 1) == 'M')
				return (ClientBodySize * 1048576);
			else if (port.at(space_pos - 1) == 'K')
				return (ClientBodySize * 1024);
			else
				return (ClientBodySize);
		}
	}
	return (1048576);
}

std::map<std::string, std::string> server_configuration::findLocation()
{
	std::map<std::string, std::string> location_map;
	std::pair<std::string, std::string> location_pair;
	size_t pos = 0;
	size_t end_loc = 0;
	
	while (pos != std::string::npos)
	{
		pos = _ConfigFile.find("location /", pos);
		if (pos != std::string::npos)
		{
			pos += strlen("location"); 
			std::string location_path = _ConfigFile.substr(pos + 1); 
			size_t space_pos = location_path.find_first_of("{"); 
			if (space_pos == std::string::npos) 
			{
				location_pair.first = "";
				location_pair.second = "";
			}
			location_pair.first = location_path.substr(0, space_pos - 1);
			end_loc = location_path.find_first_of("{}", space_pos + 1);
			int j = 0;
			while (location_path[end_loc] == '{')
			{
				j++;
				end_loc = location_path.find_first_of("{}", end_loc + 1);
			}
			for (int i = 0; i < j; i++)
			{
				end_loc = location_path.find_first_of("}", end_loc + 1);
			}
			if (end_loc == std::string::npos)
			{
				location_pair.first = "";
				location_pair.second = "";
			}
			location_pair.second = location_path.substr(space_pos + 1, (end_loc - space_pos) );
			pos = pos + end_loc;
			end_loc = 0;
		}
		else
		{
			return (location_map);
		}
		location_map.insert(location_pair);
	}
	return (location_map);
}

std::map<std::string, class server_location_configuration*> server_configuration::findLoc()
{
	std::map<std::string, class server_location_configuration*>	map_location;
	std::pair<std::string, class server_location_configuration*>	pair_location;
	
	for (std::map<std::string, std::string>::iterator it = _Location.begin(); it != _Location.end(); it++)
	{
		pair_location.first = it->first;
		server_location_configuration* tmp = new server_location_configuration(it->second);
		pair_location.second = tmp;
		map_location.insert(pair_location);
	}
	return (map_location);
	
}

template<class T>
void	server_configuration::printMap(std::map<T, T> map)
{
	for (std::map<std::string, std::string>::iterator it = map.begin(); it != map.end(); it++)
		std::cout << "- first = '" << it->first << "' && second = '" << it->second << "'" << std::endl;
}

std::ostream&	server_configuration::printLoc(std::ostream &out)
{
		
		for (std::map<std::string, class server_location_configuration*>::iterator it = _Loc.begin(); it != _Loc.end(); it++)
		{
			out << "\n\nLocation configurations  for " << it->first ;
			for (std::vector<std::string>::iterator ite = it->second->getHttpMethodAccepted().begin(); ite != it->second->getHttpMethodAccepted().end(); ite++)
				out << "\nHttpMethodAccepted : " << *ite;
			out << "\nHttpRedirection " << it->second->getHttpRedirection() \
			<< "\nRoot : " << it->second->getRoot() \
			<< "\nDirectoryListing : " << it->second->getDirectoryListing() \
			<< "\nDirectoryRequest : " << it->second->getDirectoryRequest() ;
			for (std::map<std::string, std::string>::iterator ite = it->second->getCgi().begin(); ite != it->second->getCgi().end(); ite++)
			{
				out << "\nCGI : \n" \
				<< "path : " << ite->first << "\nconf : " << ite->second ;
			}
			out << "\nUploadStore : " << it->second->getUploadStore();
		}
		return (out);
}

std::string server_configuration::getConfigFile() { return _ConfigFile;}
std::string server_configuration::getServerName() { return _ServerName;}
std::string server_configuration::getRoot() { return _Root;}
std::string server_configuration::getIndex() { return _Index;}
std::vector<int> server_configuration::getPort() { return _Port;}
std::vector<std::string>& server_configuration::getHttpMethodAccepted() {return _HttpMethodAccepted;}
std::vector<std::string> server_configuration::getHost() { return _Host;}
std::vector<std::string> server_configuration::getCookieHeader() { return _CookieHeader;}
size_t server_configuration::getClientMaxBodySize() { return _ClientMaxBodySize;}
std::map<std::string, std::string> server_configuration::getCgi() { return (_cgi); }
std::map<std::string, std::pair<std::string, std::string> >		server_configuration::getErrorPage() { return _ErrorPage;}
std::map<std::string, std::pair<std::string, std::string> >&	server_configuration::getDefErrorPage() { return _DefErrorPage;}
std::map<std::string, class server_location_configuration*>*	server_configuration::getLoc() { return (&_Loc);}
std::string server_configuration::getDirectoryListing() { return (_DirectoryListing);}

const char *	server_configuration::CgiException::what() const throw()
{
	return ("CGI parsing error\n");
}

const char *	server_configuration::ErrorPageException::what() const throw()
{
	return ("Error Page parsing error\n");
}

std::ostream& operator <<(std::ostream &out, server_configuration &ServConfig)
{
	out << "Server name : " << ServConfig.getServerName() \
		<< "\nRoot : " << ServConfig.getRoot();
		for (size_t i = 0; i < ServConfig.getPort().size(); i++)
			out << "\nPort : " << ServConfig.getPort()[i];
		for (size_t i = 0; i < ServConfig.getHost().size(); i++)
			out << "\nHost : " << ServConfig.getHost()[i];
		out << "\nCliend Body Limit : " << ServConfig.getClientMaxBodySize() \
		<< "\nCGI (first = extension, second = root):" \
		<< "\nAutoindex general : " << ServConfig.getDirectoryListing() << std::endl;
		ServConfig.printMap(ServConfig.getCgi());
		out << "\n\n***\n" \
		<< "\nLocation : "; ServConfig.printLoc(out);
		out << std::endl \
		<< "\n***\n" << std::endl;
		
	//out << "\nERROR_PAGES (first = status, second = root):" << std::endl;
	//ServConfig.printMap(ServConfig.getErrorPage());
	return (out);
}
