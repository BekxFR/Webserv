/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initServ.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgruson <mgruson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 15:32:29 by nflan             #+#    #+#             */
/*   Updated: 2023/05/05 15:26:05 by mgruson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "initServ.hpp"
#include "server_response.hpp"
#include <stdio.h>
#include <stdlib.h>

extern std::vector<int> open_ports;
extern volatile std::sig_atomic_t	g_code;

int	setnonblocking(int sockfd)
{
	(void)sockfd;
	// int flags;
	// flags = fcntl(sockfd, F_GETFL, 0);
	// if (flags == -1) {
	// 	return -1;
	// }
	// flags |= O_NONBLOCK;
	// if (fcntl(sockfd, F_SETFL, flags) == -1) {
	// 	return -1;
	// }
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
			std::cout << "\nPORT : " << Port << std::endl;
			std::cout << "GOODSERVER : " << (*it)->getPort()[i] << std::endl;
			if ((*it)->getPort()[i] == Port)
			{
				SamePort.push_back(*it);
				std::cout << "SERVERNAME : " << (*it)->getServerName() << std::endl;
				std::cout << "HOST : " << ServerRequest->getHost() << std::endl;
				std::cout << "I : " << i << std::endl;
				std::cout << "J : " << j << std::endl;
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

std::string UpdateFileNameifAlreadyExist(std::string UploadFileName)
{
	int i = 1;
	std::string UploadFileNameTmp = UploadFileName;
	std::cout << "ENTREE UpdateFileNameifAlreadyExist" << std::endl;
	while (true)
	{
		std::ifstream infile(UploadFileName.c_str());
		if (infile.good())
		{
			int pos = 0;
			while (UploadFileNameTmp.find(".", pos) != std::string::npos)
			{
				std::cout << "POS " << pos << std::endl;
				pos = UploadFileNameTmp.find(".", pos);
				pos += 1;
			}
			if (pos < 2)
				UploadFileName = UploadFileName + "(" + itos(i) + ")";
			else if (i == 1)
				UploadFileName = UploadFileName.substr(0, pos - 1) + "(" + itos(i) + ")" + UploadFileName.substr(pos - 1);
			else if (i >= 2)
			{
				UploadFileName = UploadFileNameTmp.substr(0, pos - 1) + "(" + itos(i) + ")" + UploadFileNameTmp.substr(pos - 1);
			}
			std::cout << "UPLOADFILENAME : " << UploadFileName << std::endl;
		}
		else
			break;
		i++;
	}
	std::cout << "SORTIE UpdateFileNameifAlreadyExist" << std::endl;
	return (UploadFileName);
}


int	handle_connection(std::vector<server_configuration*> servers, int conn_sock, std::multimap<int, int> StorePort, int CodeStatus, std::map<int, std::pair<std::string, std::string> >* MsgToSent)

{
	server_configuration *GoodServerConf = NULL;
	char buffer[2048];
	int n = 0; 
	int Port = 0;
	static std::map<int, std::string> SocketUploadFile;
	static std::map<int, std::string> UploadFilePath;
	static std::vector<int> UnauthorizedSocket;
	errno = 0;
	
	
	// std::cout << "\nPASSE LA " << conn_sock << std::endl;
	n = recv(conn_sock, buffer, 2048, MSG_DONTWAIT);
	if (n == 0)
		return 0;
	if (n < 0)
	{
		std::cout << "ERRNO : " << errno << std::endl;
		return 1;
	}
	// std::cout << "\nPASSE LA FIN " << std::endl;
	std::string request;
	request.append(buffer, n);
	memset(buffer, 0, n);
	while (n >= 2048)
	{
		// std::cout << "\nN VALUE : " << n << std::endl;
		n = recv(conn_sock, buffer, n, MSG_DONTWAIT);
		// std::cout << "\nN VALUE after read : " << n << std::endl;
		if (n > 0)
		{
			request.append(buffer, n);
			memset(buffer, 0, n);
		}
	}
	
	std::cout << "CON SOCK " << conn_sock << std::endl;
	// static int k = 0;
	// if (k < 5)
	// {
	// 	std::cout << "\nREQUEST ET SA SOCKET : " << conn_sock << "\n\n" << std::endl;
	// 	std::cout.write(request.c_str(), 200);
	// }

	// std::cout << "\na1\n" << std::endl;
	if (isNotBinaryData(SocketUploadFile, conn_sock))
	{
		
		// std::cout << "\na1.1\n" << std::endl;
		// std::cout << "\n\nRequest :\n\n" << request << std::endl;
		/*	Cette partie permet de parser la requete afin de pouvoir travailler
			sur chaque élément indépendemment */
		server_request ServerRequest(request);
		ServerRequest.request_parser();
		/* Cette partie permet de connaitre le port utilisé afin de d'avoir les 
		bonnes configurations de serveur */
		for (std::map<int, int>::iterator it = StorePort.begin(); it != StorePort.end(); it++)
		{
			// std::cout << "\na1.2\n" << std::endl;
			if (it->second == conn_sock)
				Port = it->first;
		}

	//	std::cerr << "PORT apres set = '" << Port << "'" << std::endl;
		GoodServerConf = getGoodServer(servers, &ServerRequest, Port);
	//	std::cerr << "Serveur Port in initserv when checking good serv:" << std::endl;
	//	for (std::vector<int>::iterator it = GoodServerConf->getPort().begin(); it != GoodServerConf->getPort().end(); it++)
	//		std::cerr << *it << std::endl;
		/********************************************************************/

		std::cout << "\nREQUEST PARSED" << std::endl;
		std::cout << ServerRequest << std::endl;
		std::cout << "\nFIN REQUEST PARSED" << std::endl;

		std::cout << "\nCONF\n" << std::endl; 
		std::cout << *GoodServerConf << std::endl;
		std::cout << "\nFIN CONF" << std::endl;
		// exit(0);
		
		/* Ci-dessous, on vérifie que la méthode est autorisée. On le fait ici
		car sinon un code erreur peut être renvoyé. Je le mets ici pour etre
		sur que le status code n'est pas modifié par la suite */
		if (CodeStatus == 200)
		{
			// std::cout << "\na1.3\n" << std::endl;
			if (!isMethodPossible(ServerRequest.getMethod()))
				CodeStatus = 400;
			else
				CodeStatus = isMethodAuthorised(ServerRequest.getMethod(), GoodServerConf, ServerRequest.getRequestURI()); // on sait s'ils ont le droit
			if (CodeStatus != 200)
				UnauthorizedSocket.push_back(conn_sock);
		}
		/********************************************/

		// std::cout << "\nMETHOD REQUETE " << ServerRequest.getMethod() << std::endl;
		// std::cout << "\nROOT " << GoodServerConf->getRoot() << std::endl;
		if (((ServerRequest.getMethod() == "GET" || ServerRequest.getMethod() == "DELETE") || (ServerRequest.getMethod() == "POST" && request.find("WebKitFormBoundary") == std::string::npos)) && checkStatus(CodeStatus))
		{
			server_response	ServerResponse(GoodServerConf->getStatusCode(), &ServerRequest);
			// std::cout << "\na1.4\n" << std::endl;
			if (GoodServerConf->getClientMaxBodySize() < ServerRequest.getContentLength())
				ServerResponse.SendingResponse(ServerRequest, conn_sock, GoodServerConf, 413, MsgToSent);
			else
				ServerResponse.SendingResponse(ServerRequest, conn_sock, GoodServerConf, 200, MsgToSent);
			return 0;

		}
		else if (ServerRequest.getMethod() == "POST" && checkStatus(CodeStatus))
		{
			// std::cout << "\na1.5\n" << std::endl;
			// std::cout << "\nSOCKET TEST 1: " << conn_sock << std::endl;
			if (GoodServerConf->getClientMaxBodySize() > ServerRequest.getContentLength())
			{	
				// std::cout << "\na1.5.1\n" << std::endl;
				SocketUploadFile.insert (std::pair< int , std::string >(conn_sock, ""));
				std::string PathToStore = getPathToStore(ServerRequest.getMethod(), GoodServerConf, ServerRequest.getRequestURI());
				while (PathToStore.find("//") != std::string::npos)
					PathToStore = PathToStore.erase(PathToStore.find("//"), 1);
				UploadFilePath.insert ( std::pair< int , std::string >(conn_sock, PathToStore));
			}
			else
			{
				// std::cout << "\na1.6\n" << std::endl;
				server_response	ServerResponse(GoodServerConf->getStatusCode(), &ServerRequest);
				ServerResponse.SendingResponse(ServerRequest, conn_sock, GoodServerConf, 413, MsgToSent);
				return 0;
			}
		}
		else if (isNotinUnauthorizedSocket(UnauthorizedSocket, conn_sock))
		{
			server_response	ServerResponse(GoodServerConf->getStatusCode(), &ServerRequest);
			ServerResponse.SendingResponse(ServerRequest, conn_sock, GoodServerConf, 200, MsgToSent);
			return 0;
		}
		else
		{
			std::cerr << "je dl mais je dois pas" << std::endl;
			server_response	ServerResponse(GoodServerConf->getStatusCode(), &ServerRequest);
			ServerResponse.setStatusCode(CodeStatus);
			ServerResponse.createResponse(GoodServerConf, "", ServerRequest, 0, 0);
			MsgToSent->insert(std::make_pair(conn_sock, std::make_pair(ServerResponse.getServerResponse(), "")));
			return 0;
		}
	}

	for (std::map<int, std::string>::iterator it = SocketUploadFile.begin(); it != SocketUploadFile.end(); it++)
	{
		if (g_code == 42)
			break ;
		// std::cout << "\na1.6\n" << std::endl;
		if (it->first == conn_sock)
		{
			// std::cout << "\nSOCKET TEST 2: " << conn_sock << std::endl;
			if (g_code == 42)
						break ;
			it->second = it->second + request;
			if (g_code == 42)
						break ;
			size_t pos = 0;
			size_t found = 0;
			if (request.find("WebKitFormBoundary") != std::string::npos)
			{
				while (it->second.find("WebKitFormBoundary", pos) != std::string::npos)
				{
					if (g_code == 42)
						break ;
					pos = it->second.find("WebKitFormBoundary", pos) + strlen("WebKitFormBoundary");
					found++;
					// std::cout << "\nSOCKET TEST 3 found for " <<  conn_sock << " : " << found << std::endl;
				}
			}
			if (found >= 3)
			{
				// std::cout << "\nSOCKET TEST 4" << std::endl;
				// std::cout << "\ncontenu du fichier\n" << std::endl;
				// std::cout.write(it->second.c_str(), it->second.size());
				int posfilename = it->second.find("filename=\"") + strlen("filename=\"");
				// std::cout << "\nPOST FILE NAME \n" << posfilename <<
				std::string UploadFileName = it->second.substr(posfilename, it->second.find("\"", posfilename) - posfilename);
				// std::cout << "\nUPLOADNAME : " << UploadFileName << " FIN" << std::endl;
				for (std::map<int, std::string>::iterator it = UploadFilePath.begin(); it != UploadFilePath.end(); it++)
				{
					if (it->first == conn_sock)
						UploadFileName = it->second + "/" + UploadFileName;
					// std::cout << " TEST UPLOADFILE PATH : " << UploadFileName << std::cout;
				}
				if (g_code == 42)
						break ;
				UploadFileName = UpdateFileNameifAlreadyExist(UploadFileName);
				std::ofstream file(UploadFileName.c_str(), std::ios::binary);
				// std::cout << "\nTEST POUR POSIINIT " << it->second.substr(posfilename, 50) << std::endl;
				pos = it->second.find("\r\n\r\n", posfilename) + strlen("\r\n\r\n");
				size_t end_pos = it->second.find("------WebKitFormBoundary", pos);
				it->second = it->second.substr(pos, (end_pos - pos));
				file.write(it->second.c_str(), it->second.size());
				file.close();
				// std::cout << "\nC0" << std::endl;
				MsgToSent->insert(std::make_pair(conn_sock, std::make_pair("HTTP/1.1 200 OK\nContent-Length: 0\n\n", "")));
				SocketUploadFile.erase(it);
				// server_response	ServerResponse(GoodServerConf->getStatusCode(), GoodServerConf->getEnv(), ServerRequest);
				// ServerResponse.SendingResponse(*ServerRequest, conn_sock, GoodServerConf, 201);
				// std::cout << "\ne9\n" << std::cout;
				break ;
			}
		}
	}
	// std::cout << "\na1.7\n" << std::endl;
	return 0;
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

int isNotPort(int fd, int* listen_sock, size_t len)
{
	size_t i = 0;
	while(i < len)
	{
		// std::cout << "LISTEN IS NOT PORT " << listen_sock[i] << std::endl;
		if (fd == listen_sock[i])
			return 0;
		i++;
	}
	return 1;
}

int	StartServer(std::vector<server_configuration*> servers, std::vector<int> Ports, std::vector<std::string> Hosts)
{
	struct sockaddr_in addr[Ports.size()];
	socklen_t addrlen[Ports.size()];
	int	conn_sock = -1;
	int	nfds = -1;
	int	epollfd = -1;
	int listen_sock[Ports.size()];
	std::multimap<int, int> StorePort;
	int CodeStatus = 0;
	std::map<int, std::pair<std::string, std::string> > MsgToSent;
	std::string PartialFileSent;
	static std::map<int, int> PercentageSent;

	signal(SIGPIPE, SIG_IGN);

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
		}
		if (bind(listen_sock[i], (struct sockaddr *) &addr[i], addrlen[i]) == -1)
		{
			if (errno == EADDRINUSE) // changer
			{
				if (1)
					std::cout << "\033[1;31m" << "Port " << Ports[i] << " is already listening" << "\033[0m\n" << std::endl;
				// std::fprintf(stderr, "Error: bind failed: %s\n", strerror(errno));
				// return(CloseSockets(listen_sock, Ports), EXIT_FAILURE);
			}
		}
		else
		{
			std::cout << "\033[1;32m" << "Port " << Ports[i] << " is listening" << "\033[0m\n" << std::endl;
		}
		if (listen(listen_sock[i], SOMAXCONN) == -1) {
			std::fprintf(stderr, "Error: listen failed: %s\n", strerror(errno));
			// return(CloseSockets(listen_sock, Ports), EXIT_FAILURE);
		}
		// open_ports.push_back(listen_sock[i]);
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
		return(CloseSockets(listen_sock, Ports), EXIT_FAILURE);
	}
	open_ports.push_back(epollfd);
	
	struct epoll_event	ev, events[MAX_EVENTS];

	for (size_t i = 0; i < Ports.size(); i++)
	{
		ev.events = EPOLLIN;
		ev.data.fd = listen_sock[i];
		// std::cout << "\nLISTEN " << i << " : " << listen_sock[i] << std::endl;
 		if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listen_sock[i], &ev) == -1) 
		{
			std::fprintf(stderr, "Error: epoll_ctl: listen_sock, %s\n", strerror(errno));
			return(CloseSockets(listen_sock, Ports), EXIT_FAILURE);
		}
	}
	for (;;) {
		if (g_code == 42)
			return(CloseSockets(listen_sock, Ports), EXIT_FAILURE);
		nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
		if (nfds == -1) {
			std::fprintf(stderr, "Error: epoll_wait: %s\n", strerror(errno));
			return(CloseSockets(listen_sock, Ports), EXIT_FAILURE);
		}
		// std::cout << "\nWAIT TIME" << std::endl;
		for (int n = 0; n < nfds; ++n)
		{
			for (size_t i = 0; i < Ports.size(); i++)
			{
				if (events[n].data.fd == listen_sock[i])
				{
					// std::cout << "\nACCEPT SOCKET : " << events[n].data.fd << " + " << listen_sock[i] <<  std::endl;
					CodeStatus = 200; // a voir comment on gère le code status après envoi ds le handle connection
					// std::fprintf(stderr, "\nEVENTS I = %d ET N = %d\n", i, n);
					conn_sock = accept(events[n].data.fd, (struct sockaddr *) &addr[i], &addrlen[i]);
					// std::cout << "EPOLL_WAIT : " << std::endl;
					// std::cout << "CON SOCK : " << conn_sock << std::endl;
					// std::cout << "listen_sock[i] : " << listen_sock[i] << std::endl;
					// std::cout << "Ports[i] : " << Ports[i] << std::endl;
					open_ports.push_back(conn_sock);
					StorePort = ChangeOrKeepPort(&StorePort, conn_sock, Ports[i]);
					if (conn_sock == -1) {
						CodeStatus = 500;
						std::fprintf(stderr, "Error: server accept failed: %s\n", strerror(errno));
						// return(CloseSockets(listen_sock, Ports), EXIT_FAILURE);
					}
					if (setnonblocking(conn_sock) == -1) {
						std::fprintf(stderr, "Error: setnonblocking: %s\n", strerror(errno));
						// return(CloseSockets(listen_sock, Ports), EXIT_FAILURE);
					}
					ev.events = EPOLLIN | EPOLLOUT;
					ev.data.fd = conn_sock;
					if (epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock, &ev) == -1) {
						std::fprintf(stderr, "Error: epoll_ctl: conn_sock, %s\n", strerror(errno));
						// return(CloseSockets(listen_sock, Ports), EXIT_FAILURE);
					}
				}
			}
			if ((events[n].events & EPOLLIN) && isNotPort(events[n].data.fd, listen_sock, Ports.size()))
			{
				// std::cout << "\nEPOLLIN : " << events[n].data.fd << " + " << events[n].data.fd << std::endl;
				// std::cout << "\nSENT : " << events[n].data.fd << std::endl;
				// ev.events = EPOLLOUT;
				// epoll_ctl(epollfd, EPOLL_CTL_MOD, events[n].data.fd, &ev);
				int error = handle_connection(servers, events[n].data.fd, StorePort, CodeStatus, &MsgToSent);
				if (error)
				{
					for (std::vector < int >::iterator it2 = open_ports.begin(); it2 != open_ports.end(); it2++)
					{
						// std::cout << "\nit2 : " << *it2 << " events fd : " << events[n].data.fd << std::endl;
						if (*it2 == events[n].data.fd)
						{
							open_ports.erase(it2);
							break;
						}
					}
					epoll_ctl(epollfd, EPOLL_CTL_DEL, events[n].data.fd, &ev);
					close (events[n].data.fd);
				}
				// events[n].events |= EPOLLOUT;
				// if (epoll_ctl(epollfd, EPOLL_CTL_MOD, events[n].data.fd, &ev) == -1) {
				// 	perror("epoll_ctl1");
				// }
			}
			if ((events[n].events & EPOLLOUT) && isNotPort(events[n].data.fd, listen_sock, Ports.size()))
			{
				// epoll_ctl(epollfd, EPOLL_CTL_MOD, events[n].data.fd, &ev);
				// std::cout << "\nEPOLLOUT : " << events[n].data.fd << std::endl;
				std::map<int, std::pair<std::string, std::string> >::iterator it = MsgToSent.find(events[n].data.fd);
				if (it != MsgToSent.end())
				{
					if (it->second.second.size() > 0)
					{
						// std::cout << "\nTEST SEND GROS FICHIER" << std::endl;
						std::cout << it->second.second.c_str() << std::endl;
						if (PercentageSent.find(it->first) == PercentageSent.end())
						{
							// std::cout << "\nSENT THE HEADER OF A BIG MESSAGE" << std::endl;
							// std::cout.write(it->second.first.c_str() , it->second.first.size());
							// std::cout << "\nFIN" << std::endl;
							if (send(it->first, it->second.first.c_str() , it->second.first.size(), 0) == -1)
							{
								std::cout << "\nsend 1 pb" << std::endl;
								for (std::vector < int >::iterator it2 = open_ports.begin(); it2 != open_ports.end(); it2++)
								{
									if (*it2 == it->first)
									{
										open_ports.erase(it2);
										break;
									}
								}
								epoll_ctl(epollfd, EPOLL_CTL_DEL, it->first, &ev);
								close (it->first);
							}
							PercentageSent.insert(std::make_pair(it->first, 0));
						}
						else
						{
							// std::cout << "SENT THE CONTENT OF A BIG MESSAGE" << std::endl;
							std::ifstream file(it->second.second.c_str(), std::ios::binary);
							if (file.is_open())
							{
								char chunk[500000];
								file.seekg(PercentageSent[it->first], std::ios::beg);
								file.read(chunk, 500000);
								// std::cout << "PERCENTAGE READ : " << PercentageSent[it->first] << std::endl;
								std::streamsize bytes_read = file.gcount();
								// std::cout << "bytes_read : " << bytes_read << std::endl;
								if (bytes_read == 0)
								{
									if (send(it->first, "\r\n\r\n", 4, 0) == -1)
									{
										std::cout << "\nsend 2 pb" << std::endl;
										for (std::vector < int >::iterator it2 = open_ports.begin(); it2 != open_ports.end(); it2++)
										{
											if (*it2 == it->first)
											{
												open_ports.erase(it2);
												break;
											}
										}
										epoll_ctl(epollfd, EPOLL_CTL_DEL, it->first, &ev);
										close (it->first);
									}
									PercentageSent.erase(PercentageSent.find(it->first));
									MsgToSent.erase(it);
									file.close();
        			    			break; // end of file
        						}
								else
								{
									if (send(it->first, chunk, bytes_read, 0) == -1)
									{
										std::cout << "\nsend 3 pb" << std::endl;
										for (std::vector < int >::iterator it2 = open_ports.begin(); it2 != open_ports.end(); it2++)
										{
											if (*it2 == it->first)
											{
												open_ports.erase(it2);
												break;
											}
										}
										epoll_ctl(epollfd, EPOLL_CTL_DEL, it->first, &ev);
										close (it->first);
									}
									PercentageSent[it->first] = PercentageSent[it->first] + bytes_read;
									file.close();
								}
							}
						}
					}
					else
					{
						// std::cout << "\nTEST SEND PETIT FICHIER" << std::endl;
						// std::cout.write(it->second.first.c_str(), it->second.first.size());
						// std::cout << "\n FIN TEST SEND PETIT FICHIER" << std::endl;

						send(it->first, it->second.first.c_str() , it->second.first.size(), 0);
						MsgToSent.erase(it);
					}
				}
			}
			
		}
	}
	return 0;
}

std::vector<server_configuration*>	SetupNewServers(std::string& filename, int ac)
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
		server_configuration* myserver = new server_configuration(ConfigFileStr.substr(pos1, pos2));
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