/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_location_configuration.hpp                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgruson <mgruson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 17:07:36 by mgruson           #+#    #+#             */
/*   Updated: 2023/05/10 13:20:10 by mgruson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_LOCATION_CONFIGURATION_HPP
#define SERVER_LOCATION_CONFIGURATION_HPP

#include "lib.hpp"

#define DEBUG 0
class server_location_configuration
{
	private:
		std::string							_HttpRedirection;
		std::string							_Root;
		std::string							_DirectoryListing;
		std::string							_DirectoryRequest;
		std::string							_UploadStore;
		std::vector<std::string>			_HttpMethodAccepted;
		std::map<std::string, std::string>	_Cgi;

		server_location_configuration();

	public:
		server_location_configuration(std::string location_conf);
		server_location_configuration(server_location_configuration const &obj);
		~server_location_configuration();
		server_location_configuration &operator=(server_location_configuration const &obj);

		// GETTERS
		std::vector<std::string>&	getHttpMethodAccepted() {return _HttpMethodAccepted;}
		std::vector<std::string>	getHttpMethodAccepted() const {return _HttpMethodAccepted;}
		std::string	getHttpRedirection() const { return _HttpRedirection;}
		std::string getRoot() const { return _Root;}
		std::string getDirectoryListing() const { return _DirectoryListing;}
		std::string getDirectoryRequest() const { return _DirectoryRequest;}
		std::map<std::string, std::string>&	getCgi() { return _Cgi;}
		std::map<std::string, std::string>	getCgi() const { return _Cgi;}
		std::string getUploadStore() const { return _UploadStore;}


		// SETTERS
		int							fillCgi(size_t pos, std::string location_conf);
		void						setCgi(std::string location_conf);
		std::string					findElement(std::string location_conf, std::string elem);
		std::vector<std::string>	findHttpMethodAccepted(std::string location_conf);

};

#endif
