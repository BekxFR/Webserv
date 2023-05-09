/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initServ.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgruson <mgruson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 15:32:45 by nflan             #+#    #+#             */
/*   Updated: 2023/05/09 10:42:03 by mgruson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INITSERV_HPP
#define INITSERV_HPP

#include "lib.hpp"

server_configuration*								getGoodServer(std::vector<server_configuration*> servers, server_request *ServerRequest, int Port);
void												handle_connection(std::vector<server_configuration*> servers, int conn_sock, std::multimap<int, int> StorePort, int CodeStatus);
void												ChangePort(std::map<int, int>&	StorePort, int conn_sock, int listen_sock);
std::multimap<int, int>								ChangeOrKeepPort(std::multimap<int, int>* StorePort, int conn_sock, int Port);
int	StartServer(std::vector<server_configuration*>	servers, std::vector<int> Ports, std::vector<std::string> Hosts);
std::vector<server_configuration*>					SetupNewServers(std::string& filename, int ac);
std::vector<int>									getPorts(std::vector<server_configuration*> servers);
std::vector<std::string>							getHosts(std::vector<server_configuration*> servers);

#endif
