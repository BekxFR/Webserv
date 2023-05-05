/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgruson <mgruson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 15:39:03 by mgruson           #+#    #+#             */
/*   Updated: 2023/05/05 16:35:42 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib.hpp"

std::vector<int> open_ports;
std::vector<int> sockets;
volatile std::sig_atomic_t	g_code = 0;

void sigint_handler(int signum)
{
    std::cerr << "\nSignal SIGINT (" << signum << ") received." << std::endl;
	closeSockets();
	g_code = 42;
}

int main(int argc, char const **argv)
{
	std::vector<server_configuration*> servers;
	try 
	{
		if (argc > 2)
		{
			std::cerr << "Wrong number of arguments" << std::endl;
			return -1;
		}
		std::signal(SIGINT, sigint_handler);
		std::signal(SIGQUIT, sigint_handler);
		std::string	config;
		if (argc == 2)
			config = std::string(argv[1]);
		else
			config = std::string("server {\nlisten 8080;\nroot ./;\n}\n");
		servers = SetupNewServers(config, argc);
		if (servers.size() == 0)
			return (1);
		// PrintServer(servers);
		StartServer(servers, getPorts(servers), getHosts(servers));
		DeleteServers(servers);
		closeSockets();
		return (1);
	}
	catch(const std::exception& e)
	{
		DeleteServers(servers);
		closeSockets();
		std::cerr << "Webserv error : " << e.what() << '\n';
		return (1);
	}
	//std::cout << "\nFIN DU PROGRAMME" << std::endl;
	return 0;
}
