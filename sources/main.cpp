/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chillion <chillion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 15:39:03 by mgruson           #+#    #+#             */
/*   Updated: 2023/04/13 14:26:22 by chillion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>
#include <string>
#include <sys/epoll.h>
#include <iostream>
#include <vector>
#include <map>
#include <iterator>
#include <arpa/inet.h>
#include <fcntl.h>
#include <csignal>
#include <utility>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "server_configuration.hpp"
#include "server_response.hpp"

#define READ_SIZE 1000
#define MAX_EVENTS 10

static std::vector<int> open_ports;

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

int setnonblocking(int sockfd) {
	int flags;
	flags = fcntl(sockfd, F_GETFL, 0);
	if (flags == -1) {
		return -1;
	}
	flags |= SOCK_NONBLOCK;
	if (fcntl(sockfd, F_SETFL, flags) == -1) {
		return -1;
	}
	return 0;
}

server_configuration* getGoodServer(std::vector<server_configuration*> servers, server_request *ServerRequest, int Port)
{
	std::vector<server_configuration*> SamePort;

	for (std::vector<server_configuration*>::iterator it = servers.begin(); it != servers.end(); it++)
	{
		for (size_t i = 0; i < (*it)->getPort().size(); i++)
		{
			if ((*it)->getPort()[i] == Port)
			{
				SamePort.push_back(*it);
				if ((*it)->getServerName() == ServerRequest->getHost())
					return (SamePort.at(i));
			}
		}
	}
	return (SamePort.at(0));
}

void handle_connection(std::vector<server_configuration*> servers, int conn_sock, std::map<int, int> StorePort, int CodeStatus)
{
	(void)CodeStatus;
	server_configuration *GoodServerConf;
	char buffer[1024];
	int n = read(conn_sock, buffer, 1024);
	int Port;
	
	if (n <= 0) {
		// close(conn_sock);
		return;
	}
	buffer[n] = '\0';
	std::string request;
	request.append(buffer);
	while (n > 0)
	{
		n = read(conn_sock, buffer, 1024);
		if (n > 0)
		{
			buffer[n] = '\0';
			request.append(buffer);
		}
	}
	std::cout << "Request :\n" << request << std::endl;
	server_request* ServerRequest = new server_request(request);
	ServerRequest->request_parser();
	// ici on a la requete qui est parsé, je peux donc trouver le bon et en envoyer qu'un
	for (std::map<int, int>::iterator it = StorePort.begin(); it != StorePort.end(); it++)
	{
		if (it->second == conn_sock)
			Port = it->first;
	}
	std::cout << "PORT TEST : " << Port << std::endl;
	GoodServerConf = getGoodServer(servers, ServerRequest, Port);
	server_response	ServerResponse(GoodServerConf->getStatusCode(), GoodServerConf->getEnv(), *ServerRequest);
	ServerResponse.todo(*ServerRequest, conn_sock, GoodServerConf);
	delete ServerRequest;
}


void	CloseSockets(int *listen_sock, sockaddr_in *addr, std::vector<int> Ports)
{
	int tablen = Ports.size();
	
	for (int i = 0; i < tablen; i++)
	{
		close(listen_sock[i]);
		close(Ports[i]);
		close(addr[i].sin_port);
	}
}

void DeleteServers(std::vector<server_configuration*> servers)
{
	for (size_t i = 0; i < servers.size(); i++)
	{
		for (std::map<std::string, class server_location_configuration*>::iterator it = servers[i]->getLoc()->begin(); it != servers[i]->getLoc()->end(); it++)
			delete it->second;
		delete servers[i];
	}
}

void ChangePort(std::map<int, int>& StorePort, int conn_sock, int listen_sock)
{
	for (std::map<int, int>::iterator it = StorePort.begin(); it != StorePort.end(); it++)
	{
		if (it->second == listen_sock)
			it->second = conn_sock;
	}
}

int StartServer(std::vector<server_configuration*> servers, std::vector<int> Ports)
{
	struct sockaddr_in addr[Ports.size()];
	socklen_t addrlen[Ports.size()];
	int conn_sock, nfds, epollfd;
	int listen_sock[Ports.size()];
	std::map<int, int> StorePort;
	int CodeStatus = 0;

	for (size_t i = 0; i < Ports.size(); i++)
	{
		addrlen[i] = sizeof(addr[i]);
		listen_sock[i] = socket(AF_INET, SOCK_STREAM, 0);
		if (listen_sock[i] == -1) {
			std::fprintf(stderr, "Error: cannot create socket: %s\n", strerror(errno));
			return(EXIT_FAILURE);
		}
		memset(&addr[i], 0, sizeof(addr[i]));
		addr[i].sin_family = AF_INET;
		// if ()
		addr[i].sin_addr.s_addr = htonl(INADDR_ANY);
		// else
			// inet_pton(AF_INET, "", &addr[i].sin_addr);
		addr[i].sin_port = htons(Ports[i]);
		StorePort.insert(std::pair<int, int>(Ports[i], listen_sock[i]));
		int val = 1;
		if (setsockopt(listen_sock[i], SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) < 0) {
			std::fprintf(stderr, "Error: setsockopt() failed: %s\n", strerror(errno));
			// return(CloseSockets(listen_sock, addr, Ports), EXIT_FAILURE);
		}
		if (bind(listen_sock[i], (struct sockaddr *) &addr[i], addrlen[i]) == -1)
		{
			if (errno == EADDRINUSE) // changer
			{	
				std::fprintf(stderr, "Error: bind failed: %s\n", strerror(errno));
				// return(CloseSockets(listen_sock, addr, Ports), EXIT_FAILURE);
			}
		}
		if (listen(listen_sock[i], SOMAXCONN) == -1) {
			std::fprintf(stderr, "Error: listen failed: %s\n", strerror(errno));
			// return(CloseSockets(listen_sock, addr, Ports), EXIT_FAILURE);
		}
	}

	epollfd = epoll_create1(0);
	if (epollfd == -1) {
		std::fprintf(stderr, "Error: epoll_create1: %s\n", strerror(errno));
		return(CloseSockets(listen_sock, addr, Ports), EXIT_FAILURE);
	}
	open_ports.push_back(epollfd);
	
	struct epoll_event ev, events[MAX_EVENTS];

	for (size_t i = 0; i < Ports.size(); i++)
	{
		ev.events = EPOLLIN;
		ev.data.fd = listen_sock[i];
		if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listen_sock[i], &ev) == -1) 
		{
			std::fprintf(stderr, "Error: epoll_ctl: listen_sock, %s\n", strerror(errno));
			return(CloseSockets(listen_sock, addr, Ports), EXIT_FAILURE);
		}
	}
	for (;;) {
		nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
		if (nfds == -1) {
			std::fprintf(stderr, "Error: epoll_wait: %s\n", strerror(errno));
			return(CloseSockets(listen_sock, addr, Ports), EXIT_FAILURE);
		}
		for (int n = 0; n < nfds; ++n) {
			for (size_t i = 0; i < Ports.size(); i++)
			{
				if (events[n].data.fd == listen_sock[i])
				{
					CodeStatus = 200; // a voir comment on gère le code status après envoi ds le handle connection
					// std::fprintf(stderr, "\nEVENTS I = %d ET N = %d\n", i, n);
					conn_sock = accept(listen_sock[i], (struct sockaddr *) &addr[i], &addrlen[i]);
					open_ports.push_back(conn_sock);
					std::cout << "conn_sock : " << conn_sock << std::endl;
					StorePort.insert(std::pair<int, int>(Ports[i], listen_sock[i]));
					ChangePort(StorePort, conn_sock, listen_sock[i]);
					if (conn_sock == -1) {
						CodeStatus = 500;
						std::fprintf(stderr, "Error: server accept failed: %s\n", strerror(errno));
						return(CloseSockets(listen_sock, addr, Ports), EXIT_FAILURE);
					}
					if (setnonblocking(conn_sock) == -1) {
						std::fprintf(stderr, "Error: setnonblocking: %s\n", strerror(errno));
						return(CloseSockets(listen_sock, addr, Ports), EXIT_FAILURE);
					}
					ev.events = EPOLLIN | EPOLLET;
					ev.data.fd = conn_sock;
					if (epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock, &ev) == -1) {
						std::fprintf(stderr, "Error: epoll_ctl: conn_sock, %s\n", strerror(errno));
						return(CloseSockets(listen_sock, addr, Ports), EXIT_FAILURE);
					}
				}
				handle_connection(servers, events[n].data.fd, StorePort, CodeStatus);
				std::cout << "events[n].data.fd : " << events[n].data.fd << std::endl;
			}
		}
	}
	return 0;
}

std::vector<server_configuration*> SetupNewServers(std::string filename, int ac, const char **env)
{
	std::string ConfigFileStr;
	if (ac == 2)
	{
		std::ifstream input_file(filename.c_str());
		if (!input_file.is_open()) {
			std::cerr << "Failed to open file " << filename << std::endl;
			exit (-1) ;
		}
		std::getline(input_file, ConfigFileStr, '\0');
	}
	else
		ConfigFileStr = filename;
	std::vector<server_configuration*> servers;
	size_t count, i = 0;
	while (ConfigFileStr.find("server {", i) != std::string::npos)
	{
		i = ConfigFileStr.find("server {", i);
		if (i != std::string::npos)
			count++;
		i++;
	}
	for (i = 0; i < count; i++)
	{
		size_t pos1 = ConfigFileStr.find("server {");
		size_t pos2 = ConfigFileStr.find("server {", pos1 + 1);
		server_configuration* myserver = new server_configuration(ConfigFileStr.substr(pos1, pos2), env);
		if (DEBUG)
			std::cout << "test\n" << ConfigFileStr.substr(pos1, pos2) << std::endl;
		servers.push_back(myserver);
		ConfigFileStr.erase(pos1, pos2);
	}
	return (servers);
}

void PrintServer(std::vector<server_configuration*> servers)
{
	for (size_t i = 0; i < servers.size(); i++)
	{
		server_configuration* server = servers[i];
		std::cout << "Server " << i << ":" << std::endl;
		std::cout << *server << std::endl;
	}
	
	// ANOTHER WAY OF DOING IT USING ITERATOR
	// for (std::vector<server_configuration*>::iterator it = servers.begin(); it != servers.end(); ++it)
	// {
	//     server_configuration* server = *it;
	//     // std::cout << "Server " << std::distance(servers.begin(), it) << ":" << std::endl;
	//     std::cout << *server << std::endl;
	// }
}

std::vector<int> getPorts(std::vector<server_configuration*> servers)
{
	std::vector<int> Ports;
	
	for (std::vector<server_configuration*>::iterator it = servers.begin(); it != servers.end(); it++)
	{		
		std::vector<int> ports = (*it)->getPort();
		for (std::vector<int>::iterator ite = ports.begin(); ite != ports.end(); ite++)
		{
			int i = 0;
			std::cout << "Ports " << i << " : " << *ite << std::endl;
			Ports.push_back(*ite);
			i++;
		}
	}
	return Ports;
}

int main(int argc, char const **argv, const char **envp)
{
	if (argc > 2)
	{
		std::cerr << "Wrong number of arguments" << std::endl;
		return -1;
	}
	signal(SIGINT, sigint_handler);

	std::string	config;
	if (argc == 2)
		config = argv[1];
	else
		config = "http {\nserver {\nlisten 8080;\n}\n}\n";
	std::vector<server_configuration*> servers = SetupNewServers(config, argc, envp);
//	PrintServer(servers);
	StartServer(servers, getPorts(servers));
	DeleteServers(servers);
	return 0;
}
