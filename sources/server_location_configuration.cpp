/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_location_configuration.cpp                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgruson <mgruson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 17:08:06 by mgruson           #+#    #+#             */
/*   Updated: 2023/04/05 14:46:39 by mgruson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server_location_configuration.hpp"

server_location_configuration::server_location_configuration()
{
	std::cout << "server_location_configuration Default Constructor called" << std::endl;
}

server_location_configuration::server_location_configuration(std::string location_conf) :
_HttpMethodAccepted(findHttpMethodAccepted(location_conf)),
_HttpRedirection("NULL"),
_Root(findRoot(location_conf)),
_DirectoryListing(findDirectoryListing(location_conf)),
_DirectoryRequest(findDirectoryRequest(location_conf)),
_UploadStore(findUploadStore(location_conf))
{
	setCgi(location_conf);
	std::cout << "server_location_configuration Overload Constructor called" << std::endl;
}

server_location_configuration::server_location_configuration(server_location_configuration const &obj)
{
	(void)obj;
	*this = obj;
}

server_location_configuration::~server_location_configuration()
{
	std::cout << "server_location_configuration Destructor called" << std::endl;
}

server_location_configuration &server_location_configuration::operator=(server_location_configuration const &obj)
{
	(void)obj;
	std::cout << "server_location_configuration Copy assignment operator called" << std::endl;
	return *this;
}

std::vector<std::string> server_location_configuration::findHttpMethodAccepted(std::string location_conf)
{
	std::vector<std::string> MethodAccepted;
	std::string delimiter = " ";
	std::string methods;
	
	size_t pos = location_conf.find("limit_except");
	if (pos != std::string::npos) {
		pos += strlen("limit_except");
		std::string root = location_conf.substr(pos + 1);
		size_t space_pos = root.find_first_of("{");
		if (space_pos != std::string::npos) {
			methods = root.substr(0, space_pos);
		}
	}
	else
	{
		MethodAccepted.push_back("NULL");
		return (MethodAccepted);
	}
	
	size_t i = 0;
	std::string token;
	while ((i = methods.find(delimiter)) != std::string::npos)
	{
		token = methods.substr(0, i);
		MethodAccepted.push_back(token);
		methods.erase(0, i + delimiter.length());
	}
	MethodAccepted.push_back(methods);
	
	return (MethodAccepted);
}


std::string server_location_configuration::findRoot(std::string location_conf)
{
	size_t pos = location_conf.find("root");
	if (pos != std::string::npos) {
		pos += strlen("root");
		std::string root = location_conf.substr(pos + 1);
		size_t space_pos = root.find_first_of(" \n;");
		if (space_pos != std::string::npos) {
			return (root.substr(0, space_pos));
		}
	}
	return ("Not found");
}

std::string server_location_configuration::findDirectoryListing(std::string location_conf)
{
	size_t pos = location_conf.find("autoindex");
	if (pos != std::string::npos) {
		pos += strlen("autoindex");
		std::string root = location_conf.substr(pos + 1);
		size_t space_pos = root.find_first_of(" \n;");
		if (space_pos != std::string::npos) {
			return (root.substr(0, space_pos));
		}
	}
	return ("Not found");
}

std::string server_location_configuration::findDirectoryRequest(std::string location_conf)
{
	
	size_t pos = location_conf.find("	index");
	if (pos != std::string::npos) {
		pos += strlen("	index");
		std::string root = location_conf.substr(pos + 1);
		size_t space_pos = root.find_first_of(" \n;");
		if (space_pos != std::string::npos) {
			return (root.substr(0, space_pos));
		}
	}
	return ("Not found");
}

int server_location_configuration::fillCgi(size_t pos, std::string location_conf)
{
	size_t tmp = 0;
	std::pair<std::string, std::string> cgi_pair;
	for (tmp = pos; location_conf[pos] != ' '; pos++) {}
	cgi_pair.first = location_conf.substr(tmp, pos - tmp);
	for (; location_conf[pos] == ' '; pos++) {}
	if (location_conf[pos] == ';')
		return 0;
	for (tmp = pos; location_conf[pos] != ' ' && location_conf[pos] != ';'; pos++) {}
	cgi_pair.second = location_conf.substr(tmp, pos - tmp);
	_Cgi.insert(cgi_pair);
	return (pos + 1);
}

void server_location_configuration::setCgi(std::string location_conf)
{
	size_t pos = location_conf.find("cgi");
	if (pos == std::string::npos)
		return ;
	pos += strlen("cgi");
	for (; location_conf[pos] == ' '; pos++) {}
	if (location_conf[pos] == ';' || location_conf[pos] != '.')
		return ;
	for (; location_conf[pos] != ';' && location_conf[pos] != '\n';)
		pos = fillCgi(pos, location_conf);
	
}

std::string server_location_configuration::findUploadStore(std::string location_conf)
{
	size_t pos = location_conf.find("upload_store");
	if (pos != std::string::npos) {
		pos += strlen("upload_store");
		std::string root = location_conf.substr(pos + 1);
		size_t space_pos = root.find_first_of(" \n;");
		if (space_pos != std::string::npos) {
			return (root.substr(0, space_pos));
		}
	}
	return ("Not found");
}