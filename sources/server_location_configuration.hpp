/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_location_configuration.hpp                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgruson <mgruson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 17:07:36 by mgruson           #+#    #+#             */
/*   Updated: 2023/04/08 15:51:30 by mgruson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_LOCATION_CONFIGURATION_HPP
#define SERVER_LOCATION_CONFIGURATION_HPP

#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>
#include <vector>
#define DEBUG 0
class server_location_configuration
{
	private:
	std::vector<std::string>	_HttpMethodAccepted;
	std::string	_HttpRedirection;
	std::string	_Root;
	std::string	_DirectoryListing;
	std::string	_DirectoryRequest;
	std::map<std::string, std::string>	_Cgi;
	std::string _UploadStore;

	public:
	server_location_configuration();
	server_location_configuration(std::string location_conf);
	server_location_configuration(server_location_configuration const &obj);
	~server_location_configuration();
	server_location_configuration &operator=(server_location_configuration const &obj);

	std::vector<std::string> findHttpMethodAccepted(std::string location_conf);
	std::string findHttpRedirection(std::string location_conf);
	std::string findRoot(std::string location_conf);
	std::string findDirectoryListing(std::string location_conf);
	std::string findDirectoryRequest(std::string location_conf);
	int	fillCgi(size_t pos, std::string location_conf);
	void setCgi(std::string location_conf);
	std::string findUploadStore(std::string location_conf);

	std::vector<std::string>& getHttpMethodAccepted();
	std::string	getHttpRedirection();
	std::string getRoot();
	std::string getDirectoryListing();
	std::string getDirectoryRequest();
	std::map<std::string, std::string>&	getCgi();
	std::string getUploadStore();
};


#endif