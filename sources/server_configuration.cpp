/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_configuration.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chillion <chillion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 11:06:26 by mgruson           #+#    #+#             */
/*   Updated: 2023/04/13 14:18:41 by chillion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server_configuration.hpp"

server_configuration::server_configuration()
{
	if (DEBUG)
		std::cout << "server_configuration Default Constructor called" << std::endl;
}

server_configuration::server_configuration(std::string ConfigFile, const char **env) : 
_ConfigFile(ConfigFile),
_ServerName(findServerName()),
_Root(findRoot()),
_Index(findIndex()),
_Port(findPort()),
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
	for (size_t i = 0; env[i]; i++)
		_env.push_back(env[i]);
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
	return ("localhost");
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
	for (; _ConfigFile[pos] == ' '; pos++) {}
	if (_ConfigFile[pos] == ';')
		throw CgiException();
	for (tmp = pos; _ConfigFile[pos] != ' ' && _ConfigFile[pos] != ';'; pos++) {}
	cgi_pair.second = _ConfigFile.substr(tmp, pos - tmp);
	_cgi.insert(cgi_pair);
	return (pos + 1);
}

void server_configuration::setCgi()
{
	size_t pos = _ConfigFile.find("cgi");
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

std::vector<int> server_configuration::findPort()
{
	size_t pos = 0;
	size_t space_pos = 0;
	std::vector<int> Port;
	while (pos != std::string::npos)
	{
		if (_ConfigFile.find("listen 0.0.0.0:", pos) != std::string::npos)
		{
			pos = _ConfigFile.find("listen 0.0.0.0:", pos) + strlen("listen 0.0.0.0:");
			std::string port = _ConfigFile.substr(pos);
			space_pos = port.find_first_of(" \n;");
			if (space_pos != std::string::npos) {
				if (DEBUG)
					std::cout << "server_configuration::findPort() " << port.substr(0, space_pos).c_str() << std::endl;
				Port.push_back(atoi(port.substr(0, space_pos).c_str()));
			}
		}
		else if (_ConfigFile.find("listen localhost:", pos) != std::string::npos)
		{
			pos = _ConfigFile.find("listen localhost:", pos) + strlen("listen localhost:");
			std::string port = _ConfigFile.substr(pos);
			space_pos = port.find_first_of(" \n;");
			if (space_pos != std::string::npos) {
				if (DEBUG)
					std::cout << "server_configuration::findPort() " << port.substr(0, space_pos).c_str() << std::endl;
				Port.push_back(atoi(port.substr(0, space_pos).c_str()));
			}
		}
		else if (_ConfigFile.find("	listen ", pos) != std::string::npos)
		{
			pos = _ConfigFile.find("	listen ", pos) + strlen("	listen ");
			std::string port = _ConfigFile.substr(pos);
			space_pos = port.find_first_of(" \n;");
			if (space_pos != std::string::npos) {
				if (DEBUG)
					std::cout << "server_configuration::findPort() " << port.substr(0, space_pos).c_str() << std::endl;
				Port.push_back(atoi(port.substr(0, space_pos).c_str()));
			}
		}
		else
			pos = _ConfigFile.find("	listen ", pos);
		pos = pos + space_pos;
		space_pos = 0;
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
			location_pair.second = location_path.substr(space_pos + 1, end_loc);
			pos = pos + end_loc;
			end_loc = 0;
		}
		else
		{
			location_pair.first = "";
			location_pair.second = "";
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
std::vector<int>	server_configuration::getPort() { return _Port;}
std::vector<std::string> server_configuration::getEnv() { return _env;}
size_t server_configuration::getClientMaxBodySize() { return _ClientMaxBodySize;}
std::map<std::string, std::string> server_configuration::getCgi() { return (_cgi); }
std::map<std::string, std::pair<std::string, std::string> >		server_configuration::getErrorPage() { return _ErrorPage;}
std::map<std::string, std::pair<std::string, std::string> >&	server_configuration::getDefErrorPage() { return _DefErrorPage;}
std::map<std::string, class server_location_configuration*>*	server_configuration::getLoc() { return (&_Loc);}

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
		<< "\nRoot : " << ServConfig.getRoot() \
		// for (int i = 0; i < ServConfig.getPort().size(); i++);
			// << "\nPort : " << ServConfig.getPort()[i] 
		<< "\nCliend Body Limit : " << ServConfig.getClientMaxBodySize() \
		<< "\nCGI (first = extension, second = root):" << std::endl;
		ServConfig.printMap(ServConfig.getCgi());
		out << "\n\n***\n" \
		<< "\nLocation : "; ServConfig.printLoc(out);
		out << std::endl \
		<< "\n***\n" << std::endl;
		
	//out << "\nERROR_PAGES (first = status, second = root):" << std::endl;
	//ServConfig.printMap(ServConfig.getErrorPage());
	return (out);
}
