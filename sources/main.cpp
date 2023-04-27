/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chillion <chillion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 15:39:03 by mgruson           #+#    #+#             */
/*   Updated: 2023/04/26 19:34:44 by chillion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib.hpp"

std::vector<int> open_ports;
volatile std::sig_atomic_t	g_code = 0;

void sigint_handler(int signum)
{
    std::cerr << "\nSignal SIGINT (" << signum << ") received." << std::endl;
	for (size_t i = 0; i < open_ports.size(); i++)
	{
		close(open_ports[i]);
	}
	// close(8083); // Port 1
	// close(8086); // Port 2
	// close(3); // listen_socket 1
	// close(4); // listen_socket 2
	// close(5); // epoll_create1
	// close(6); // accept AF_INET socket conn_sock ev.data.fd Conf1
	// close(7); // accept AF_INET socket conn_sock ev.data.fd Conf1
	// close(8); // accept AF_INET socket conn_sock ev.data.fd Conf2
	// close(9); // accept AF_INET socket conn_sock ev.data.fd Conf2
	// close(0); // bind listen
	// close(1); // nfds
}

int main(int argc, char const **argv, const char **envp)
{
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
		std::vector<server_configuration*> servers = SetupNewServers(config, argc, envp);
		if (servers.size() == 0)
			return (1);
//		PrintServer(servers);
		StartServer(servers, getPorts(servers), getHosts(servers));
		DeleteServers(servers);
	}
	catch(const std::exception& e)
	{
		std::cerr << "Webserv error : " << e.what() << '\n';
		return (1);
	}
	return 0;
}
