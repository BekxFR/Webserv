/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initServ.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 15:32:45 by nflan             #+#    #+#             */
/*   Updated: 2023/04/26 15:37:03 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INITSERV_HPP
#define INITSERV_HPP

#include "lib.hpp"

//grosso merdo ce qu'il y avait dans main UwU

int													setnonblocking(int sockfd);
server_configuration*								getGoodServer(std::vector<server_configuration*> servers, server_request *ServerRequest, int Port);
void												handle_connection(std::vector<server_configuration*> servers, int conn_sock, std::multimap<int, int> StorePort, int CodeStatus);
void												ChangePort(std::map<int, int>&	StorePort, int conn_sock, int listen_sock);
std::multimap<int, int>								ChangeOrKeepPort(std::multimap<int, int>* StorePort, int conn_sock, int Port);
int	StartServer(std::vector<server_configuration*>	servers, std::vector<int> Ports, std::vector<std::string> Hosts);
std::vector<server_configuration*>					SetupNewServers(std::string& filename, int ac, const char **env);
std::vector<int>									getPorts(std::vector<server_configuration*> servers);
std::vector<std::string>							getHosts(std::vector<server_configuration*> servers);

#endif
