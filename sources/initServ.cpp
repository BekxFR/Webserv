/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initServ.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chillion <chillion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 15:32:29 by nflan             #+#    #+#             */
/*   Updated: 2023/05/05 19:29:38 by chillion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "initServ.hpp"
#include "server_response.hpp"

extern std::vector<int> open_ports;
extern volatile std::sig_atomic_t	g_code;

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

#include <cctype>
#include <algorithm>

bool check_End_Line(const std::string& str)
{
	if (str.find("\r\n\r\n") == std::string::npos)
		return (1);
	return (0);
}

int check_Host_Line(const std::string& str)
{
    std::string word;
    std::string tmpStr;
    std::string line;
    int count = 0;
    int status = 0;

	std::string::size_type host_start = str.find("HTTP/1.1\r\n");
	std::string::size_type host_end = str.find("\r\n\r\n", host_start);
	tmpStr = str.substr(host_start + 8, host_end);
	std::transform(tmpStr.begin(), tmpStr.end(), tmpStr.begin(), ::tolower);
	if (str.find("host:") == std::string::npos)
	{
		if (check_End_Line(tmpStr))
			return(1);
		return (2);
	}
	std::istringstream ss(tmpStr);
	std::istringstream iss(tmpStr);
	while (std::getline(ss, line)) {
		// if (line.size() < 3)
		// 	return (2);
		if (line.size() >= 5 && line.substr(0, 5) == "host:")
			break ;
		count++;
	}
	if (ss.eof()) {
		return (1);
	}
	for(int i = 0; i < count; i++)
		std::getline(iss, line);
	count = 0;
	while (iss >> word)
	{
		if (count == 2)
			break ;
		if (count == 0 && (word == "host:" || word.size() > 5))
		{
			status = 1;
		}
		if (count == 1 && status == 1)
			return(0);
		count++;
	}
	if (count == 1 && status == 1)
		return (2);
	return (1);
}

int check_First_Line(const std::string& str)
{
    std::istringstream ss(str);
	std::string line;
    std::string word;
	int count = 0;

	if (std::getline(ss,line))
	{
		std::istringstream iss(line);
		while (iss >> word)
		{
			if (count > 2)
				return (2);
			if ((count == 0) && (word != "GET" && word != "POST" && word != "DELETE"))
				return (2);
			if (count == 1 && word[0] != '/')
				return (2);
			if (count == 2 && word != "HTTP/1.1")
				return (2);
			count++;
		}
	}
	if (count == 3)
		return (0);
	if (count < 3 && str.size() > 2)
	{
		return (2);
	}
	return (1);
}

int check_Request_Value(const std::string& request, const int status)
{
	int status_Ref = 0;

	if (status == 0){
		status_Ref = check_First_Line(request);}
		std::cerr << "\nWARNING 1\n" << status_Ref << std::endl;
	if (status == 1){
		status_Ref = check_Host_Line(request);
		std::cerr << "\nWARNING 2\n" << status_Ref << std::endl;
	}
	if (status == 2){
		status_Ref = check_End_Line(request);}
		std::cerr << "\nWARNING 3\n" << status_Ref << std::endl;
	// if (status == 1 && request.find("Host: ") == std::string::npos)
	// 	return (1);
	return (status_Ref);
}

void	add_Request_To_File(int conn_sock, std::string request)
{
	char str[10];
	sprintf(str, "%d", conn_sock);
	std::ofstream file(str, std::ios::app);
	if (file) {
		file << request;
		file.close();
	}
}

std::string get_file_contents(int conn_sock)
{
	char str[10];
	sprintf(str, "%d", conn_sock);
    std::ifstream file(str);
    std::string content;

    if (file.is_open()) {
        std::string line;
        while (getline(file, line)) {
            content += line + "\n";
        }
        file.close();
    }
    return content;
}

int pre_Request_Parser(std::map<int, int>& RequestSocketStatus, int conn_sock, std::string request)
{
	int status_Ref = 0;
	int index_Status = 0;
	std::string new_Request;
	
	for (std::map<int, int>::iterator it = RequestSocketStatus.begin(); it != RequestSocketStatus.end(); ++it) {
		if (it->first == conn_sock && it->second == 0)
		{
			if (request.size() < 3)
				return (0);
			index_Status = 1;
			add_Request_To_File(conn_sock, request);
			new_Request = get_file_contents(conn_sock);
			status_Ref = check_Request_Value(new_Request, it->second);
			if (status_Ref == 0)
			{
				std::cerr << "\nPHASE 1 IS OK\n" << std::endl;
				it->second = 1;
			}
			if (status_Ref == 2)
				return (status_Ref);
		}
		if (it->first == conn_sock && it->second == 1)
		{
			if (index_Status == 0)
				add_Request_To_File(conn_sock, request);
			new_Request = get_file_contents(conn_sock);
			status_Ref = check_Request_Value(new_Request, it->second);
			if (status_Ref == 0)
			{
				std::cerr << "\nPHASE 2 IS OK\n" << std::endl;
				it->second = 2;
			}
			if (status_Ref == 2)
				return (status_Ref);
		}
		if (it->first == conn_sock && it->second == 2)
		{
			if (index_Status == 0)
				add_Request_To_File(conn_sock, request);
			new_Request = get_file_contents(conn_sock);
			status_Ref = check_Request_Value(new_Request, it->second);
			if (status_Ref == 0)
			{
				std::cerr << "\nPHASE 3 IS OK\n" << std::endl;
				it->second = 3;
				return (1);
			}
			if (status_Ref == 2)
				return (status_Ref);
		}
	}
	return (0);
}

int already_In_Map(std::map<int, int>& RequestSocketStatus, int conn_sock)
{
	for (std::map<int, int>::iterator it = RequestSocketStatus.begin(); it != RequestSocketStatus.end(); ++it) {
		if (it->first == conn_sock)
			return (1);
	}
	return (0);
}

void add_To_map(std::map<int, int>& RequestSocketStatus, int conn_sock)
{
	// for (std::map<int, int>::iterator it = RequestSocketStatus.begin(); it != RequestSocketStatus.end(); ++it) {
	// 	if (it->first == conn_sock)
	// 		return ;
	// }
	RequestSocketStatus.insert(std::pair<int, int>(conn_sock, 0));
}

void print_Map(std::map<int, int>& RequestSocketStatus)
{
	for (std::map<int, int>::iterator it = RequestSocketStatus.begin(); it != RequestSocketStatus.end(); ++it) {
		std::cout << "\nIN MAP" << it->first << " - "  << it->second << std::endl;
	}
}

void handle_connection(std::vector<server_configuration*> servers, int conn_sock, std::multimap<int, int> StorePort, int CodeStatus, std::vector<std::pair<int, std::string> >* MsgToSent)
{
	server_configuration *GoodServerConf;
	char buffer[2048];
	int n = 0; 
	int Port = 0;
	int status = 0;
	static std::map<int, std::string> SocketUploadFile;
	static std::map<int, int> RequestSocketStatus;
	static std::map<int, std::string> UploadFilePath;
	static std::vector<int> UnauthorizedSocket;
	errno = 0;
	
	// std::cout << "PASSE LA " << conn_sock << std::endl;
	n = read(conn_sock, buffer, 2048);
	if (n <= 0) 
	{
		// std::cout << "\nread1 -1 : " << errno << std::endl;
		// for(std::vector<int>::iterator it = open_ports.begin(); it < open_ports.end(); it++)
		// {
		// 	if (*it == conn_sock)
		// 	{
		// 		close (*it);
		// 		open_ports.erase(it);
		// 		break;
		// 	}
		// }
		return;
	}
	std::string request;
	request.append(buffer, n);
	memset(buffer, 0, n);
	while (n >= 2048)
	{
		n = read(conn_sock, buffer, n);
		if (n > 0)
		{
			request.append(buffer, n);
			memset(buffer, 0, n);
		}
		// else if (n < 0) 
		// {
		// 	// std::cout << "\nread2 -1" << std::endl;
		// 	for(std::vector<int>::iterator it = open_ports.begin(); it < open_ports.end(); it++)
		// 	{
		// 		if (*it == conn_sock)
		// 		{
		// 			close (*it);
		// 			open_ports.erase(it);
		// 			break;
		// 		}
		// 	}
		// 	return;
		// }
	}
	
	// static int k = 0;
	// if (k < 5)
	// {
	// 	std::cout << "\nREQUEST ET SA SOCKET : " << conn_sock << "\n\n" << std::endl;
	// 	std::cout.write(request.c_str(), 100);
	// }

	// std::cout << "\na1\n" << std::endl;
	if (isNotBinaryData(SocketUploadFile, conn_sock))
	{
		// std::cout << "\na1.1\n" << std::endl;
		// std::cout << "\n\nRequest :\n\n" << request << std::endl;

// q: une fonction pour ajouter le conn_sock dans une map avec un status a 0
// si 0 verifie la string recu
//     si GET POST DELETE / HTTP/1.1 ==> 1
//     si GET && "\r\n\r\n" ==> 2
//     si POST && "\r\n\r\n" ==> ?

// creer une map avec 2 int et verifier avant l'insertion si le premier element n'existe pas
// si il existe on ne l'ajoute pas
// si il n'existe pas on l'ajoute
// si il existe on verifie le deuxieme element
// si le deuxieme element est a 0 on verifie la string recu
		std::cout << "\na1.1\n" << std::endl;
		std::cerr << "\n\nRequest :\n\n" << request << std::endl;
		char str[10];
		sprintf(str, "%d", conn_sock);
		// std::ofstream file(str, std::ios::app);
		// if (file) {
		// 	file << request;
		// 	file.close();
		// }

		// std::ifstream infile(str);
		// if file.open() {
		// 	std::string buffer\
		// 	buffer << file;
		// 	buffer.find("/r/n/r/n")
		// 	// file << request;
		// 	file.close();
		// }
		server_request ServerRequest;
		add_To_map(RequestSocketStatus, conn_sock);
		print_Map(RequestSocketStatus);
		status = pre_Request_Parser(RequestSocketStatus, conn_sock, request);
		if (status == 2)
		{
			MsgToSent->push_back(std::pair<int, std::string>(conn_sock, "HTTP/1.1 400 Bad Request\nContent-Type: text/html\nContent-Length: 353\r\n\r\n<html><head><meta name=\"viewport\" content=\"width=device-width, minimum-scale=0.1\"><title>400 Bad Request</title></head><body style=\"background: #0e0e0e; height: 100%;text-align:center;color:white;\"><h1>400 Bad Request</h1><img src=\"https://http.cat/400\" style=\"display: block;margin: auto;\" alt=\"400 Bad Request\"><p>webserv</p></body></html>")); // remplace sent
			remove(str);
			RequestSocketStatus.clear();
			return ;
		}
		if (status == 1)
		{
			std::string fullRequest;
			fullRequest = get_file_contents(conn_sock);
			ServerRequest = server_request(fullRequest);
			ServerRequest.request_parser();
			remove(str);
		}

		/*	Cette partie permet de parser la requete afin de pouvoir travailler
			sur chaque élément indépendemment */
		/* Cette partie permet de connaitre le port utilisé afin de d'avoir les 
		bonnes configurations de serveur */
		for (std::map<int, int>::iterator it = StorePort.begin(); it != StorePort.end(); it++)
		{
			// std::cout << "\na1.2\n" << std::endl;
			if (it->second == conn_sock)
				Port = it->first;
		}
		GoodServerConf = getGoodServer(servers, &ServerRequest, Port);
		/********************************************************************/
	
	
		/* Ci-dessous, on vérifie que la méthode est autorisée. On le fait ici
		car sinon un code erreur peut être renvoyé. Je le mets ici pour etre
		sur que le status code n'est pas modifié par la suite */
		if (CodeStatus == 200)
		{
			// std::cout << "\na1.3\n" << std::endl;
			CodeStatus = isMethodAuthorised(ServerRequest.getMethod(), GoodServerConf, ServerRequest.getRequestURI()); // on sait s'ils ont le droit
			if (CodeStatus != 200)
				UnauthorizedSocket.push_back(conn_sock);
		}
		/********************************************/

		std::cout << "\nMETHOD REQUETE " << ServerRequest.getMethod() << std::endl;
		// std::cout << "\nROOT " << GoodServerConf->getRoot() << std::endl;
		if (((ServerRequest.getMethod() == "GET" || ServerRequest.getMethod() == "DELETE") || (ServerRequest.getMethod() == "POST" && request.find("WebKitFormBoundary") == std::string::npos)) && CodeStatus == 200)
		{
			std::cout << "\na1.4\n" << std::endl;
			server_response	ServerResponse(GoodServerConf->getStatusCode(), &ServerRequest);
			ServerResponse.SendingResponse(ServerRequest, conn_sock, GoodServerConf, 200, MsgToSent);
			return ;
		}
		else if (ServerRequest.getMethod() == "POST")
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
				// delete ServerRequest;
				// return ;
			}
			else
			{
				// std::cout << "\na1.6\n" << std::endl;
				server_response	ServerResponse(GoodServerConf->getStatusCode(), &ServerRequest);
				ServerResponse.SendingResponse(ServerRequest, conn_sock, GoodServerConf, 413, MsgToSent);
				return ;
			}
		}
		else if (isNotinUnauthorizedSocket(UnauthorizedSocket, conn_sock))
		{
			server_response	ServerResponse(GoodServerConf->getStatusCode(), &ServerRequest);
			ServerResponse.SendingResponse(ServerRequest, conn_sock, GoodServerConf, 200, MsgToSent);
			return ;
		}
	}

	for (std::map<int, std::string>::iterator it = SocketUploadFile.begin(); it != SocketUploadFile.end(); it++)
	{
		if (g_code == 42)
			break ;
		// std::cout << "\na1.6\n" << std::endl;
		if (it->first == conn_sock)
		{
			std::cout << "\nSOCKET TEST 2: " << conn_sock << std::endl;
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
				std::cout << "\nC0" << std::endl;
				MsgToSent->push_back(std::pair<int, std::string>(conn_sock, "HTTP/1.1 200 OK\nContent-Length: 0\n\n")); // remplace sent
				SocketUploadFile.erase(it);
				// server_response	ServerResponse(GoodServerConf->getStatusCode(), GoodServerConf->getEnv(), ServerRequest);
				// ServerResponse.SendingResponse(*ServerRequest, conn_sock, GoodServerConf, 201);
				// std::cout << "\ne9\n" << std::cout;
				break ;
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
	std::vector<std::pair<int, std::string> > MsgToSent;
	std::string PartialFileSent;
	

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
		return(CloseSockets(listen_sock, addr, Ports), EXIT_FAILURE);
	}
	open_ports.push_back(epollfd);
	
	struct epoll_event	ev, events[MAX_EVENTS];

	for (size_t i = 0; i < Ports.size(); i++)
	{
		ev.events = EPOLLIN | EPOLLOUT;
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
		for (int n = 0; n < nfds; ++n)
		{
			for (size_t i = 0; i < Ports.size(); i++)
			{
				if (events[n].data.fd == listen_sock[i])
				{
					// std::cout << "\nACCEPT SOCKET : " << events[n].events << std::endl;
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
						// return(CloseSockets(listen_sock, addr, Ports), EXIT_FAILURE);
					}
					if (setnonblocking(conn_sock) == -1) {
						std::fprintf(stderr, "Error: setnonblocking: %s\n", strerror(errno));
						// return(CloseSockets(listen_sock, addr, Ports), EXIT_FAILURE);
					}
					ev.events = EPOLLIN | EPOLLOUT;
					ev.data.fd = conn_sock;
					if (epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock, &ev) == -1) {
						std::fprintf(stderr, "Error: epoll_ctl: conn_sock, %s\n", strerror(errno));
						// return(CloseSockets(listen_sock, addr, Ports), EXIT_FAILURE);
					}
				}
				if (events[n].events & EPOLLIN)
				{
					// std::cout << "\nEPOLLIN : " << events[n].events << std::endl;
					// std::cout << "\nSENT : " << events[n].data.fd << std::endl;
					handle_connection(servers, events[n].data.fd, StorePort, CodeStatus, &MsgToSent);
				}
				if (events[n].events & EPOLLOUT)
				{
					// std::cout << "\nEPOLLOUT : " << events[n].events << std::endl;
					for (std::vector<std::pair<int, std::string> >::iterator it = MsgToSent.begin(); it != MsgToSent.end(); it++)
					{
						if (events[n].data.fd == it->first)
						{
							// std::cout << "\nAS-TU ENVOYE? " << it->second.c_str() << std::endl;
							if (it->second.size() < 500000)
							{
								std::cout << "\n< 500000 " << std::endl;
								std::cout << it->first << std::endl; 
								std::cout << it->second << std::endl; 
								if (send(it->first, it->second.c_str() , it->second.size(), 0) == -1)
									std::cerr << "\nSend pb 1: " << errno << std::endl;
								MsgToSent.erase(it);
								// it->first = 0;
								// it->second = "";
								break;
							}
							else
							{
								// std::cout << "\n> 500000 " << std::endl;
								// std::cout << it->first << std::endl; 
								// std::cout << "\na0.1\n" << std::endl;
								errno = 0;
								if (send(it->first, it->second.substr(0, 500000).c_str(), 500000, 0) == -1)
								{
									std::cout << "\nSend pb 2: " << errno << std::endl;
									MsgToSent.erase(it);
									break;
								}
								else
								{
									usleep(2000);
									// std::cout << "\na0.4\n" << std::endl;
									it->second.erase(0,500000);
								}
								break;
							}
						}
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