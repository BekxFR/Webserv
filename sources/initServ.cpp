/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initServ.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgruson <mgruson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 15:32:29 by nflan             #+#    #+#             */
/*   Updated: 2023/05/02 14:36:55 by mgruson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "initServ.hpp"
#include "server_response.hpp"

extern std::vector<int> open_ports;

int	setnonblocking(int sockfd)
{
	int flags;
	flags = fcntl(sockfd, F_GETFL, 0);
	if (flags == -1) {
		return -1;
	}
	flags |= O_NONBLOCK;
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

int isMethodAuthorised(std::string MethodUsed, server_configuration *server, std::string RequestURI)
{	
	for (std::map<std::string, class server_location_configuration*>::reverse_iterator it = server->getLoc().rbegin(); it != server->getLoc().rend(); it++)
	{
		if (it->first == RequestURI.substr(0, it->first.size()))
		{
			for (std::vector<std::string>::reverse_iterator ite = it->second->getHttpMethodAccepted().rbegin(); ite != it->second->getHttpMethodAccepted().rend(); ite++)
			{
				if (MethodUsed == *ite || isGenerallyAuthorised(MethodUsed, server, *ite))
				{
					// s'il passe ici c'est que la méthode est autorisée et qu'une loc a été trouvée
					return (200);
				}
			}
		}
	}
	/* Je rajoute cette verification car au-dessus ce n'est verifie que si la Request URI trouve son path
	dans une location */
	if (isGenerallyAuthorised(MethodUsed, server, "NOT INDICATED"))
		return (200);
	// s'il passe ici c'est qu'aucune loc n'a éte trouvée et que donc c'est possible, meme ds le principal
	return (405);
}

std::string getPathToStore(std::string MethodUsed, server_configuration *server, std::string RequestURI)
{	
	for (std::map<std::string, class server_location_configuration*>::reverse_iterator it = server->getLoc().rbegin(); it != server->getLoc().rend(); it++)
	{
		if (it->first == RequestURI.substr(0, it->first.size()))
		{
			for (std::vector<std::string>::reverse_iterator ite = it->second->getHttpMethodAccepted().rbegin(); ite != it->second->getHttpMethodAccepted().rend(); ite++)
			{
				if (MethodUsed == *ite || isGenerallyAuthorised(MethodUsed, server, *ite))
				{
					/*	Ci-dessous, on renvoie directement le path au store, car ce path se suffit à lui-même. 
						Si on ne le trouve pas, alors on renvoie le root car on enregistra à la racine du root. */
					if (it->second->getUploadStore().size() > 0)
						return (it->second->getUploadStore());
					else
						return (server->getRoot());
				}
			}
		}
	}
	return (server->getRoot());
}

bool isNotBinaryData(std::map<int, std::string> SocketUploadFile, int conn_sock)
{
	for (std::map<int, std::string>::iterator it = SocketUploadFile.begin(); it != SocketUploadFile.end(); it++)
	{
		if (it->first == conn_sock)
			return 0;
	}
	return 1;
}

bool	isNotinUnauthorizedSocket(std::vector<int> UnauthorizedSocket, int conn_sock)
{
	for (std::vector<int>::iterator it = UnauthorizedSocket.begin(); it != UnauthorizedSocket.end(); it++)
	{
		if (*it == conn_sock)
			return 0;
	}
	return 1;
}

void handle_connection(std::vector<server_configuration*> servers, int conn_sock, std::multimap<int, int> StorePort, int CodeStatus)
{
	server_configuration *GoodServerConf;
	char buffer[2048];
	int n = read(conn_sock, buffer, 2048);
	int Port = 0;
	static std::map<int, std::string> SocketUploadFile;
	static std::map<int, std::string> UploadFilePath;
	static std::vector<int> UnauthorizedSocket;
	
	if (n <= 0) 
	{
		return;
	}
	std::string request;
	request.append(buffer, n);
	memset(buffer, 0, n);
	while (n > 0)
	{
		n = read(conn_sock, buffer, n);
		if (n > 0)
		{
			request.append(buffer, n);
			memset(buffer, 0, n);
		}
	}
	
	static int k = 0;
	if (k < 5)
	{
		std::cout << "\nREQUEST ET SA SOCKET : " << conn_sock << "\n\n" << std::endl;
		std::cout.write(request.c_str(), 100);
	}

	// std::cout << "\na1\n" << std::endl;
	if (isNotBinaryData(SocketUploadFile, conn_sock))
	{
		std::cout << "\na1.1\n" << std::endl;
		// std::cout << "\n\nRequest :\n\n" << request << std::endl;
		/*	Cette partie permet de parser la requete afin de pouvoir travailler
			sur chaque élément indépendemment */
		server_request* ServerRequest = new server_request(request);
		ServerRequest->request_parser();
		/* Cette partie permet de connaitre le port utilisé afin de d'avoir les 
		bonnes configurations de serveur */
		for (std::map<int, int>::iterator it = StorePort.begin(); it != StorePort.end(); it++)
		{
			// std::cout << "\na1.2\n" << std::endl;
			if (it->second == conn_sock)
				Port = it->first;
		}
		GoodServerConf = getGoodServer(servers, ServerRequest, Port);
		/********************************************************************/
	
	
		/* Ci-dessous, on vérifie que la méthode est autorisée. On le fait ici
		car sinon un code erreur peut être renvoyé. Je le mets ici pour etre
		sur que le status code n'est pas modifié par la suite */
		if (CodeStatus == 200)
		{
			std::cout << "\na1.3\n" << std::endl;
			CodeStatus = isMethodAuthorised(ServerRequest->getMethod(), GoodServerConf, ServerRequest->getRequestURI()); // on sait s'ils ont le droit
			if (CodeStatus != 200)
				UnauthorizedSocket.push_back(conn_sock);
		}
		/********************************************/

		std::cout << "\nMETHOD REQUETE " << ServerRequest->getMethod() << std::endl;
		std::cout << "\nROOT " << GoodServerConf->getRoot() << std::endl;
		if ((ServerRequest->getMethod() == "GET" || ServerRequest->getMethod() == "DELETE") && CodeStatus == 200)
		{
			std::cout << "\na1.4\n" << std::endl;
			server_response	ServerResponse(GoodServerConf->getStatusCode(), GoodServerConf->getEnv(), ServerRequest);
			ServerResponse.SendingResponse(*ServerRequest, conn_sock, GoodServerConf, 200);
			delete ServerRequest;
			return ;
		}
		else if (ServerRequest->getMethod() == "POST" && CodeStatus == 200)
		{
			std::cout << "\na1.5\n" << std::endl;
			std::cout << "\nSOCKET TEST 1: " << conn_sock << std::endl;
			if (GoodServerConf->getClientMaxBodySize() > ServerRequest->getContentLength())
			{	
				std::cout << "\na1.5.1\n" << std::endl;
				SocketUploadFile.insert (std::pair< int , std::string >(conn_sock, ""));
				std::string PathToStore = getPathToStore(ServerRequest->getMethod(), GoodServerConf, ServerRequest->getRequestURI());
				while (PathToStore.find("//") != std::string::npos)
					PathToStore = PathToStore.erase(PathToStore.find("//"), 1);
				UploadFilePath.insert ( std::pair< int , std::string >(conn_sock, PathToStore));
			}
			else
			{
				std::cout << "\na1.6\n" << std::endl;
				server_response	ServerResponse(GoodServerConf->getStatusCode(), GoodServerConf->getEnv(), ServerRequest);
				ServerResponse.SendingResponse(*ServerRequest, conn_sock, GoodServerConf, 413);
				delete ServerRequest;
				return ;
			}
		}
		else if (isNotinUnauthorizedSocket(UnauthorizedSocket, conn_sock))
		{
			server_response	ServerResponse(GoodServerConf->getStatusCode(), GoodServerConf->getEnv(), ServerRequest);
			ServerResponse.SendingResponse(*ServerRequest, conn_sock, GoodServerConf, 200);
			delete ServerRequest;
			return ;
		}
	}

	for (std::map<int, std::string>::iterator it = SocketUploadFile.begin(); it != SocketUploadFile.end(); it++)
	{
		// std::cout << "\na1.6\n" << std::endl;
		if (it->first == conn_sock)
		{
			std::cout << "\nSOCKET TEST 2: " << conn_sock << std::endl;
			it->second = it->second + request;
			size_t pos = 0;
			size_t found = 0;
			if (request.find("WebKitFormBoundary") != std::string::npos)
			{
				while (it->second.find("WebKitFormBoundary", pos) != std::string::npos)
				{
					pos = it->second.find("WebKitFormBoundary", pos) + strlen("WebKitFormBoundary");
					found++;
					std::cout << "\nSOCKET TEST 3 found for " <<  conn_sock << " : " << found << std::endl;
				}
			}
			if (found >= 3)
			{
				std::cout << "\nSOCKET TEST 4" << std::endl;
				// std::cout << "\ncontenu du fichier\n" << std::endl;
				// std::cout.write(it->second.c_str(), it->second.size());
				int posfilename = it->second.find("filename=\"") + strlen("filename=\"");
				// std::cout << "\nPOST FILE NAME \n" << posfilename <<
				std::string UploadFileName = it->second.substr(posfilename, it->second.find("\"", posfilename) - posfilename);
				std::cout << "\nUPLOADNAME : " << UploadFileName << " FIN" << std::endl;
				for (std::map<int, std::string>::iterator it = UploadFilePath.begin(); it != UploadFilePath.end(); it++)
				{
					if (it->first == conn_sock)
						UploadFileName = it->second + "/" + UploadFileName;
					std::cout << " TEST UPLOADFILE PATH : " << UploadFileName << std::cout;
				}
				std::ofstream file(UploadFileName.c_str(), std::ios::binary);
				std::cout << "\nTEST POUR POSIINIT " << it->second.substr(posfilename, 50) << std::endl;
				pos = it->second.find("\r\n\r\n", posfilename) + strlen("\r\n\r\n");
				size_t end_pos = it->second.find("------WebKitFormBoundary", pos);
				it->second = it->second.substr(pos, (end_pos - pos));
				file.write(it->second.c_str(), it->second.size());
				file.close();
				SocketUploadFile.erase(it);
				// server_response	ServerResponse(GoodServerConf->getStatusCode(), GoodServerConf->getEnv(), ServerRequest);
				// ServerResponse.SendingResponse(*ServerRequest, conn_sock, GoodServerConf, 201);
				std::cout << "\ne9\n" << std::cout;
				return ;
			}
		}
	}
	// std::cout << "\na1.7\n" << std::endl;
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