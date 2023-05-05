/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgruson <mgruson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 15:28:11 by nflan             #+#    #+#             */
/*   Updated: 2023/05/05 15:16:10 by mgruson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

#include "lib.hpp"

//Print
template<class T>
void	printMap(std::map<T, T> map)
{
	for (std::map<std::string, std::string>::iterator it = map.begin(); it != map.end(); it++)
		std::cout << "- first = '" << it->first << "' && second = '" << it->second << "'" << std::endl;
}
void		PrintServer(std::vector<server_configuration*> servers);

// Tools
bool		checkStatus(int status);
bool		isMethodPossible(std::string method);
bool		is_dir(const char* path, server_response& sr);
std::string	prev_link(std::string path);
bool		isGenerallyAuthorised(std::string MethodUsed, server_configuration *server, std::string ite);

// Int to String
std::string	itos(int nb);

// Add custom Error Page
std::string	readingFileEP( std::string file );

//Cleaning
void		CloseSockets(int *listen_sock, std::vector<int> Ports);
void		DeleteServers(std::vector<server_configuration*> servers);

#endif
