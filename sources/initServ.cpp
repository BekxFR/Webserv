/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initServ.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgruson <mgruson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 15:32:29 by nflan             #+#    #+#             */
/*   Updated: 2023/04/27 13:46:59 by mgruson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "initServ.hpp"

extern std::vector<int> open_ports;

int	setnonblocking(int sockfd)
{
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

server_configuration*	getGoodServer(std::vector<server_configuration*> servers, server_request *ServerRequest, int Port)
{
	std::vector<server_configuration*> SamePort;
	int j = 0;
	// std::cout << "c1.0.4" << std::endl;
	for (std::vector<server_configuration*>::iterator it = servers.begin(); it != servers.end(); it++)
	{
		// std::cout << "c1.0.5 (*it)->getPort().size() " << (*it)->getPort().size() << std::endl;
		for (size_t i = 0; i < (*it)->getPort().size(); i++)
		{
			// std::cout << "c1.0.3" << std::endl;
			// std::cout << "PORT : " << Port << std::endl;
			// std::cout << "GOODSERVER : " << (*it)->getPort()[i] << std::endl;
			if ((*it)->getPort()[i] == Port)
			{
				SamePort.push_back(*it);
				// std::cout << "SERVERNAME : " << (*it)->getServerName() << std::endl;
				// std::cout << "HOST : " << ServerRequest->getHost() << std::endl;
				// std::cout << "I : " << i << std::endl;
				// std::cout << "J : " << j << std::endl;
				if ((*it)->getServerName() == ServerRequest->getHost())
				{
					return (SamePort.at(j));
				}
				j++;
			}
		}
	}
	// std::cout << "c1.0.6" << std::endl;
	return (SamePort.at(0));
}

void	upload_small_file(std::string request)
{
	std::cout << "\na2\n" << std::endl;
	std::ofstream file("smalloutput.png", std::ios::binary);
	size_t pos = request.find("------WebKitFormBoundary");
	std::cout << "\nBOUNDARY POS\n" << pos << std::endl;
	pos = request.find("\r\n\r\n") + strlen("\r\n\r\n");
	std::cout << "\nSTART OF THE STRING\n" << pos << std::endl;
	std::string start = request.substr(pos);
	std::cout << start << std::endl;
	// std::cout.write(start.c_str(), start.size());
	size_t end_pos = request.find("------WebKitFormBoundary", pos);
	std::cout << "\nEND_POS\n" << end_pos << std::endl;
	std::cout << "\nFIN\n" << (end_pos - pos) << std::endl; 
	request = request.substr(pos, (end_pos - pos));
	file.write(request.c_str(), request.size());
	file.close();
}


void handle_connection(std::vector<server_configuration*> servers, int conn_sock, std::multimap<int, int> StorePort, int CodeStatus)
{
	(void)CodeStatus;
	server_configuration *GoodServerConf;
	char buffer[100000];
	int n = read(conn_sock, buffer, 100000);
	int Port = 0;
	if (n <= 0) {
		// close(conn_sock);
		return;
	}
	// buffer[n] = '\0';
	std::cout << "\nBUFFER 1\n" << std::endl;
	std::cout.write(buffer, 2048);
	std::string request;
	request.append(buffer, 2048);
	memset(buffer, 0, 2048);
	while (n > 0)
	{
		n = read(conn_sock, buffer, 2048);
		if (n > 0)
		{
			// buffer[n] = '\0';
			std::cout << "\nBUFFER 2\n" << std::endl;
			std::cout.write(buffer, 2048);
			request.append(buffer, 2048);
			memset(buffer, 0, 2048);
		}
	}
	
	static bool go = false;
	static bool uploading = false;
	static int Webkit = 0;
	static int nb_of_webkitboudary = 0;
	static int pos_web = 0;
	std::cout << "\nREQUEST SUR LAQUELLE JE BOSSE\n\n" << std::endl;
	std::cout.write(request.c_str(), request.size());
	std::cout << "\nRESULT FIND POS\n" << request.find("POST") << std::endl;
	std::cout << "\nSIZE\n" << request.size() << std::endl;
	if (request.find("POST") != std::string::npos)
	{
		std::cout << "\na1\n" << std::endl;
		go = true;
	}
	if (request.find("------WebKitFormBoundary") != std::string::npos && go) // pas de elseif car peut etre ds la mm requete
	{
		size_t pos_web = 0;
		while (request.find("------WebKitFormBoundary", pos_web) != std::string::npos)
		{
			nb_of_webkitboudary++
			pos_web = request.find("------WebKitFormBoundary", pos_web);
		}
		if (nb_of_webkitboudary == 2)
			upload_small_file(request)
		if (nb_of_webkitboudary == 1 && pos_web < 10)
			upload_start_file(request)
			// cela veut dire que il n y a que au debut que y aura pas d autres
		if (nb_of_webkitboudary == 1 && pos_web > 10)
			upload_end_file(request)
			// cela veut dire que il n y a que a la fin que y aura pas d autres
		if (nb_of_webkitboudary == 0)
			upload_midlle_file(request)
			// cela veut dire que on est plein dedans ();
		if (Webkit == 0)
			uploading = true;
		Webkit++;
		if (Webkit == 2)
		{
			uploading = false;
			Webkit == 0;
		}
	}
	if ()
	while (go && Webkit < 2) (request.find("------WebKitFormBoundary") != std::string::npos || w
	if (go && request.find("------WebKitFormBoundary") != std::string::npos)
	{
		std::cout << "\na2\n" << std::endl;
		std::ofstream file("output.png", std::ios::binary);
		size_t pos = request.find("------WebKitFormBoundary");
		std::cout << "\nBOUNDARY POS\n" << pos << std::endl;
		pos = request.find("\r\n\r\n") + strlen("\r\n\r\n");
		std::cout << "\nSTART OF THE STRING\n" << pos << std::endl;
		std::string start = request.substr(pos);
		std::cout << start << std::endl;
		// std::cout.write(start.c_str(), start.size());
		size_t end_pos = request.find("------WebKitFormBoundary", pos);
		std::cout << "\nEND_POS\n" << end_pos << std::endl;
		std::cout << "\nFIN\n" << (end_pos - pos) << std::endl; 
		request = request.substr(pos, (end_pos - pos));
		file.write(request.c_str(), request.size());
		file.close();
	}

	// std::cout << "\n\nRequest :\n\n" << request << std::endl;
	server_request* ServerRequest = new server_request(request);
	ServerRequest->request_parser();
	// ici on a la requete qui est parsé, je peux donc trouver le bon et en envoyer qu'un
	
	// std::cout << "c5" << std::endl;
	for (std::map<int, int>::iterator it = StorePort.begin(); it != StorePort.end(); it++)
	{
		// std::cout << "TROUVER LE BON PORT" << std::endl;
		// std::cout << "it->second : " << it->second << std::endl;
		// std::cout << "conn_sock : " << conn_sock << std::endl;
		// std::cout << "it->first : " << it->first << std::endl;
		if (it->second == conn_sock)
			Port = it->first;
	}
	// std::cout << "c6" << std::endl;
	
	// std::cout << "PORT TEST : " << Port << std::endl;
	// std::cout << "e1.0" << std::endl;
	GoodServerConf = getGoodServer(servers, ServerRequest, Port);
	// std::cout << "e1.1" << std::endl;
	server_response	ServerResponse(GoodServerConf->getStatusCode(), GoodServerConf->getEnv(), ServerRequest);
	// std::cout << "e1.2" << std::endl;
	/**************************************/
	/* TENTATIVE DE UPLOAD */
	
	static std::string PostContent ;
	static bool posting;
	static unsigned long long ContentSize; 
	static int j = 0;
	static int k = 0;
	static int l = 0;
	static std::string filename;
	// static std::ifstream file(FinalPath.c_str(), std::ifstream::binary);
	
	if (ServerRequest->getMethod() == "POST")
	{
		std::cout << "c1\n" << std::endl;
		posting = true;
		ContentSize = ServerRequest->getContentLength();
		j++;
	}
	else if (PostContent.size() < ContentSize && posting)
	{
		k++;
		std::cout << "c2\n" << "ContentSize : " << ContentSize << std::endl;
		if (j == 1)
		{
			int pos = 0;
			filename = ServerRequest->getVersion();
			pos = filename.find("filename=\"") + strlen("filename=\"");
			filename = filename.substr(pos, filename.size() - pos - 1);
			std::cout << "c2.0" << std::endl;
			std::cout << "filename " << filename << std::endl;
		}
		std::cout << "PostContent.size() before : " << PostContent.size() << std::endl;
		PostContent = PostContent + ServerRequest->getServerRequest();
		std::cout << "PostContent.size() after : " << PostContent.size() << std::endl;
		// std::cout << "TEST UPLOAD\n" << PostContent << std::endl;
		if (PostContent.size() >= ContentSize)
		{
			std::cout << "c2.1" << std::endl;
			ServerResponse.SendingPostResponse(*ServerRequest, conn_sock, GoodServerConf, PostContent, filename);
			posting = false;
		}
	}
	else
		ServerResponse.SendingResponse(*ServerRequest, conn_sock, GoodServerConf);
	delete ServerRequest;
	l++;
	// std::cout << "c3\n" << "l = " << l << " k : " << k << std::endl;
}

void	ChangePort(std::map<int, int>& StorePort, int conn_sock, int listen_sock)
{
	for (std::map<int, int>::iterator it = StorePort.begin(); it != StorePort.end(); it++)
	{
		// std::cout << "CHANGE_PORT : " << std::endl;
		// std::cout << "it->second : " << it->second << std::endl;
		// std::cout << "listen_sock : " << listen_sock << std::endl;
		// std::cout << "conn_sock : " << conn_sock << std::endl;
		if (it->second == listen_sock)
			it->second = conn_sock;
	}
}

std::multimap<int, int>	ChangeOrKeepPort(std::multimap<int, int>* StorePort, int conn_sock, int Port)
{
	// std::cout << "\nINSIDE CHANGE OR KEEP\n" << std::endl;
	
	for (std::multimap<int, int>::iterator it = StorePort->begin(); it != StorePort->end(); it++)
	{
		// std::cout << "\nChangeOrKeep normal : " << std::endl;
		// std::cout << "it->second con sock : " << it->second << std::endl;
		// std::cout << "Port : " << Port << std::endl;
		// std::cout << "conn_sock : " << conn_sock << std::endl;
		/* DERNIERE MODIFICATION */
		// if (it->first == Port)
		// {
		// 	it->second = conn_sock;
		// 	return ;
		// }
		/*************************/
		if (it->second == conn_sock)
		{
			// std::cout << "RETURN CHANGE OR KEEP" << std::endl;
			return (*StorePort);
		}
	}
	// std::cout << "shoud insert" << std::endl;
	StorePort->insert(std::pair<int, int>(Port, conn_sock));

	// std::cout << "\nSTART TEST" << std::endl;
	// int i = 0;
	for (std::multimap<int, int>::iterator it = StorePort->begin(); it != StorePort->end(); it++)
	{
		// std::cout << "\n TEST ChangeOrKeep element : " << i << std::endl;
		// std::cout << "it->second con sock : " << it->second << std::endl;
		// std::cout << "Port : " << Port << std::endl;
		// std::cout << "conn_sock : " << conn_sock << std::endl;
		/* DERNIERE MODIFICATION */
		// if (it->first == Port)
		// {
		// 	it->second = conn_sock;
		// 	return ;
		// }
		/*************************/
		if (it->second == conn_sock)
		{
			// std::cout << "RETURN CHANGE OR KEEP" << std::endl;
			// return (*StorePort);
		}
	}
	// std::cout << "\nEND TEST" << std::endl;
	
	return (*StorePort);
}

int	StartServer(std::vector<server_configuration*> servers, std::vector<int> Ports, std::vector<std::string> Hosts)
{
	struct sockaddr_in addr[Ports.size()];
	socklen_t addrlen[Ports.size()];
	int conn_sock, nfds, epollfd;
	int listen_sock[Ports.size()];
	std::multimap<int, int> StorePort;
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
		
		/****Ci-dessous, tentative de bien lier les adresses IP**********/
		if (Hosts[i].size() == 0 || Hosts[i] == "[::]")
			addr[i].sin_addr.s_addr = htonl(INADDR_ANY);
		else
			inet_pton(AF_INET, Hosts[i].c_str(), &addr[i].sin_addr);
		/****************************************************************/
		
		addr[i].sin_port = htons(Ports[i]);
		// StorePort.insert(std::pair<int, int>(Ports[i], listen_sock[i]));
		int val = 1;
		if (setsockopt(listen_sock[i], SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) < 0) {
			std::fprintf(stderr, "Error: setsockopt() failed: %s\n", strerror(errno));
			// return(CloseSockets(listen_sock, addr, Ports), EXIT_FAILURE);
		}
		if (bind(listen_sock[i], (struct sockaddr *) &addr[i], addrlen[i]) == -1)
		{
			if (errno == EADDRINUSE) // changer
			{
				if (1)
					std::cout << "\033[1;31m" << "Port " << Ports[i] << " is already listening" << "\033[0m\n" << std::endl;
				// std::fprintf(stderr, "Error: bind failed: %s\n", strerror(errno));
				// return(CloseSockets(listen_sock, addr, Ports), EXIT_FAILURE);
			}
		}
		else
		{
			std::cout << "\033[1;32m" << "Port " << Ports[i] << " is listening" << "\033[0m\n" << std::endl;
		}
		if (listen(listen_sock[i], SOMAXCONN) == -1) {
			std::fprintf(stderr, "Error: listen failed: %s\n", strerror(errno));
			// return(CloseSockets(listen_sock, addr, Ports), EXIT_FAILURE);
		}
	}

	// std::cout << "STOREMAP" << std::endl;
	// for (std::map<int, int>::iterator it = StorePort.begin(); it != StorePort.end(); it++)
	// {
	// 	std::cout << "first" << it->first << std::endl;
	// 	std::cout << "second" << it->second << std::endl;
	// }
	epollfd = epoll_create1(0);
	if (epollfd == -1) {
		std::fprintf(stderr, "Error: epoll_create1: %s\n", strerror(errno));
		return(CloseSockets(listen_sock, addr, Ports), EXIT_FAILURE);
	}
	open_ports.push_back(epollfd);
	
	struct epoll_event	ev, events[MAX_EVENTS];

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
					// std::cout << "EPOLL_WAIT : " << std::endl;
					// std::cout << "CON SOCK : " << conn_sock << std::endl;
					// std::cout << "listen_sock[i] : " << listen_sock[i] << std::endl;
					// std::cout << "Ports[i] : " << Ports[i] << std::endl;
					open_ports.push_back(conn_sock);
					StorePort = ChangeOrKeepPort(&StorePort, conn_sock, Ports[i]);
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
				// std::cout << "events[n].data.fd : " << events[n].data.fd << std::endl;
			}
		}
	}
	return 0;
}

std::vector<server_configuration*>	SetupNewServers(std::string& filename, int ac, const char **env)
{
	std::string ConfigFileStr;
	std::vector<server_configuration*> servers;
	if (ac == 2)
	{
		struct stat sb;
		std::ifstream input_file(filename.c_str());

		if (stat(filename.c_str(), &sb) == -1 || S_ISDIR(sb.st_mode) || access(filename.c_str(), R_OK) || !input_file.is_open())
		{
			if (stat(filename.c_str(), &sb) != -1 && S_ISDIR(sb.st_mode))
				std::cerr << "Can't use a directory as config file (" << filename << ")" << std::endl;
			else
				perror("Error when trying to open the file (access)");
			return (servers);
		}
		std::getline(input_file, ConfigFileStr, '\0');
	}
	else
		ConfigFileStr = filename;
	size_t	count = 0;
	for (size_t i = 0; ConfigFileStr.find("server {", i) != std::string::npos; i++)
	{
		i = ConfigFileStr.find("server {", i);
		if (i != std::string::npos)
			count++;
	}
	for (size_t i = 0; i < count; i++)
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

std::vector<int>	getPorts(std::vector<server_configuration*> servers)
{
	std::vector<int> Ports;
	int i = 0;
	
	for (std::vector<server_configuration*>::iterator it = servers.begin(); it != servers.end(); it++)
	{		
		std::vector<int> ports = (*it)->getPort();
		for (std::vector<int>::iterator ite = ports.begin(); ite != ports.end(); ite++)
		{
			if (0)
				std::cout << "Ports " << i << " : " << *ite << std::endl;
			Ports.push_back(*ite);
			i++;
		}
	}
	return Ports;
}

std::vector<std::string>	getHosts(std::vector<server_configuration*> servers)
{
	std::vector<std::string> Hosts;
	int i = 0;
	
	for (std::vector<server_configuration*>::iterator it = servers.begin(); it != servers.end(); it++)
	{		
		std::vector<std::string> hosts = (*it)->getHost();
		for (std::vector<std::string>::iterator ite = hosts.begin(); ite != hosts.end(); ite++)
		{
			if (0)
				std::cout << "Host " << i << " : " << *ite << std::endl;
			Hosts.push_back(*ite);
			i++;
		}
	}
	return Hosts;
}

