/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initServ.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgruson <mgruson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 15:32:29 by nflan             #+#    #+#             */
/*   Updated: 2023/05/10 17:08:50 by mgruson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib.hpp"

extern std::vector<int> open_ports;
extern std::vector<int> sockets;
extern volatile std::sig_atomic_t	g_code;

server_configuration*	getGoodServer(std::vector<server_configuration*> servers, server_request *ServerRequest, int Port)
{
	std::vector<server_configuration*> SamePort;
	int j = 0;
	for (std::vector<server_configuration*>::iterator it = servers.begin(); it != servers.end(); it++)
	{
		for (size_t i = 0; i < (*it)->getPort().size(); i++)
		{
			if ((*it)->getPort()[i] == Port)
			{
				SamePort.push_back(*it);
				if ((*it)->getServerName() == ServerRequest->getHost())
				{
					return (SamePort.at(j));
				}
				j++;
			}
		}
	}
	return (SamePort.at(0));
}

int isMethodAuthorised(std::string MethodUsed, server_configuration *server, std::string RequestURI)
{
	bool loc = false;
	for (std::map<std::string, class server_location_configuration*>::reverse_iterator it = server->getLoc().rbegin(); it != server->getLoc().rend(); it++)
	{
		if (it->first == RequestURI.substr(0, it->first.size()))
		{
			for (std::vector<std::string>::reverse_iterator ite = it->second->getHttpMethodAccepted().rbegin(); ite != it->second->getHttpMethodAccepted().rend(); ite++)
			{
				if (MethodUsed == *ite)
				{
					// s'il passe ici c'est que la méthode est autorisée et qu'une loc a été trouvée
					return (200);
				}
				loc = true;
			}
			if (loc == true)
			{
				return (405);
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
	if (server->getUploadStore().size() != 0)
	{
		return (server->getUploadStore());
	}
	return (server->getRoot());
}

bool	isNotinUnauthorizedSocket(std::vector<int> UnauthorizedSocket, int conn_sock)
{
	for (std::vector<int>::iterator it = UnauthorizedSocket.begin(); it != UnauthorizedSocket.end(); it++)
		if (*it == conn_sock)
			return 0;
	return 1;
}

std::string UpdateFileNameifAlreadyExist(std::string UploadFileName)
{
	int i = 1;
	std::string UploadFileNameTmp = UploadFileName;
	while (true)
	{
		std::ifstream infile(UploadFileName.c_str());
		if (infile.good())
		{
			int pos = 0;
			while (UploadFileNameTmp.find(".", pos) != std::string::npos)
			{
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
		}
		else
			break;
		i++;
	}
	return (UploadFileName);
}

#include <cctype>
#include <algorithm>

bool check_Header_Size(const std::string& str)
{
	std::string::size_type header_end = str.find("\r\n\r\n");
	std::string header = str.substr(0, header_end);
	if (header.size() >= 4096)
		return (1);
	return (0);
}

int nbr_Host_line(const std::string& request)
{
	int count = 0;
	std::string tmpStr;
	std::string line;
	std::string::size_type host_start = request.find("HTTP/1.1\r\n");
	std::string::size_type host_end = request.find("\r\n\r\n", host_start);

	tmpStr = request.substr(host_start + 8, host_end);
	std::transform(tmpStr.begin(), tmpStr.end(), tmpStr.begin(), ::tolower);
	
	std::istringstream ss(tmpStr);
	while (std::getline(ss, line)) {
		if (line.size() >= 5 && line.substr(0, 5) == "host:")
			count++;
		if (count == 2)
			return (2);
	}
	return (0);
}

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
	if (tmpStr.find("host:") == std::string::npos)
	{
		if (check_End_Line(tmpStr))
			return(1);
		return (2);
	}
	std::istringstream ss(tmpStr);
	std::istringstream iss(tmpStr);
	while (std::getline(ss, line)) {
		if (line.size() >= 5 && line.substr(0, 5) == "host:")
		{
			std::getline(iss, line);
			while (iss >> word)
			{
				if (count == 2)
					break ;
				if (count == 0)
				{
					if (word.size() > 5 && word.substr(0, 5) == "host:")
						return (0);
					if (word.size() == 5 && word.substr(0, 5) == "host:")
						status = 1;
					status = 1;
				}
				if (count == 1 && status == 1){
					return(0);}
				count++;
			}
			count = 0;
		}
		count = 0;
	}
	if (!check_End_Line(tmpStr))
		return (2);
	if (ss.eof()) {
		return (1);
	}
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
	if (status == 1){
		status_Ref = check_Host_Line(request);
	}
	if (status == 2){
		if (nbr_Host_line(request) == 2)
			return (2);
		status_Ref = check_End_Line(request);}
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
			if (check_Header_Size(new_Request) == 1)
				return (2);
			status_Ref = check_Request_Value(new_Request, it->second);
			if (status_Ref == 0)
			{
				it->second = 1;
			}
			if (status_Ref == 2)
				return (status_Ref);
		}
		if (it->first == conn_sock && it->second == 1)
		{
			if (index_Status == 0)
				add_Request_To_File(conn_sock, request);
			index_Status = 1;
			new_Request = get_file_contents(conn_sock);
			if (check_Header_Size(new_Request) == 1)
				return (2);
			status_Ref = check_Request_Value(new_Request, it->second);
			if (status_Ref == 0)
			{
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
			if (check_Header_Size(new_Request) == 1)
				return (2);
			status_Ref = check_Request_Value(new_Request, it->second);
			if (status_Ref == 0)
			{
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
	RequestSocketStatus.insert(std::pair<int, int>(conn_sock, 0));
}

void print_Map(std::map<int, int>& RequestSocketStatus)
{
	for (std::map<int, int>::iterator it = RequestSocketStatus.begin(); it != RequestSocketStatus.end(); ++it) {
		std::cout << "\nIN MAP" << it->first << " - "  << it->second << std::endl;
	}
}

int	handle_connection(std::vector<server_configuration*> servers, int conn_sock, std::multimap<int, int> StorePort, int CodeStatus, std::map<int, std::pair<std::string, std::string> >* MsgToSent)
{
	server_configuration *GoodServerConf = NULL;
	char buffer[2048];
	int n = 0;
	int Port = 0;
	int status = 0;
	static std::map<int, int> RequestSocketStatus;
	static std::map<int, std::pair<std::string, int> > SocketUploadFile;
	static std::map<int, std::string> UploadFilePath;
	static std::map<int, std::string> FileName;
	static std::vector<int> UnauthorizedSocket;
	errno = 0;

	n = recv(conn_sock, buffer, 2048, MSG_DONTWAIT);
	if (n == 0)
		return 0;
	if (n < 0)
	{
		MsgToSent->insert(std::make_pair(conn_sock, std::make_pair("HTTP/1.1 500 Internal Server Error\r\nContent-Type: text/html; charset=utf-8\r\nContent-Length: 383\r\n\r\n<html><head><meta name=\"viewport\" content=\"width=device-width, minimum-scale=0.1\"><title>500 Internal Server Error</title></head><body style=\"background: #0e0e0e; height: 100%;text-align:center;color:white;\"><h1>500 Internal Server Error</h1><img src=\"https://http.cat/500\" style=\"display: block;margin: auto;\" alt=\"500 Internal Server Error\"><p>webserv</p></body></html>", "")));
		return 1;
	}
	std::string request;
	request.append(buffer, n);
	memset(buffer, 0, n);
	while (n >= 2048)
	{
				if (g_code == 42)
			break ;
		n = recv(conn_sock, buffer, n, MSG_DONTWAIT);
		if (n > 0)
		{
			request.append(buffer, n);
			memset(buffer, 0, n);
		}
	}
	
	if (SocketUploadFile.find(conn_sock) == SocketUploadFile.end())
	{
		char str[10];
		sprintf(str, "%d", conn_sock);
		server_request ServerRequest;
		add_To_map(RequestSocketStatus, conn_sock);
		status = pre_Request_Parser(RequestSocketStatus, conn_sock, request);
		if (status == 2)
		{
			MsgToSent->insert(std::make_pair(conn_sock, std::make_pair("HTTP/1.1 400 Bad Request\r\nContent-Type: text/html; charset=utf-8\nContent-Length: 335\r\n\r\n<html><head><meta name=\"viewport\" content=\"width=device-width, minimum-scale=0.1\"><title>400 Bad Request</title></head><body style=\"background: #0e0e0e; height: 100%;text-align:center;color:white;\"><h1>400 Bad Request</h1><img src=\"https://http.cat/400\" style=\"display: block;margin: auto;\" alt=\"400 Bad Request\"><p>webserv</p></body></html>",""))); // remplace sent
			remove(str);
			RequestSocketStatus.clear();
			RequestSocketStatus.erase(conn_sock);
			return 0;
		}
		if (status == 1)
		{
			std::string fullRequest;
			fullRequest = get_file_contents(conn_sock);
			ServerRequest.setRequest(fullRequest);
			ServerRequest.add_Host_Value(fullRequest);
			ServerRequest.request_parser();
			remove(str);
			RequestSocketStatus.erase(conn_sock);
		}
		else if (status == 0)
			return 0;

		
		/*	Cette partie permet de parser la requete afin de pouvoir travailler
			sur chaque élément indépendemment */
		// server_request ServerRequest(request);
		// ServerRequest.request_parser();
		/* Cette partie permet de connaitre le port utilisé afin de d'avoir les
		bonnes configurations de serveur */
		for (std::map<int, int>::iterator it = StorePort.begin(); it != StorePort.end(); it++)
		{
			if (it->second == conn_sock)
				Port = it->first;
		}

		GoodServerConf = getGoodServer(servers, &ServerRequest, Port);

		/* Ci-dessous, on vérifie que la méthode est autorisée. On le fait ici
		car sinon un code erreur peut être renvoyé. Je le mets ici pour etre
		sur que le status code n'est pas modifié par la suite */
		if (CodeStatus == 200)
		{
			if (!isMethodPossible(ServerRequest.getMethod()))
				CodeStatus = 400;
			else
				CodeStatus = isMethodAuthorised(ServerRequest.getMethod(), GoodServerConf, ServerRequest.getRequestURI()); // on sait s'ils ont le droit
			if (CodeStatus != 200)
				UnauthorizedSocket.push_back(conn_sock);
		}
		/********************************************/
		
		if (((ServerRequest.getMethod() == "GET" || ServerRequest.getMethod() == "DELETE") || (ServerRequest.getMethod() == "POST" && request.find("WebKitFormBoundary") == std::string::npos)) && checkStatus(CodeStatus))
		{
			server_response	ServerResponse(GoodServerConf->getStatusCode(), &ServerRequest);
			if (GoodServerConf->getClientMaxBodySize() < ServerRequest.getContentLength())
				ServerResponse.SendingResponse(ServerRequest, conn_sock, GoodServerConf, 413, MsgToSent);
			else
				ServerResponse.SendingResponse(ServerRequest, conn_sock, GoodServerConf, 200, MsgToSent);
			return 0;
		}
		else if (ServerRequest.getMethod() == "POST" && checkStatus(CodeStatus))
		{

			if (GoodServerConf->getClientMaxBodySize() > ServerRequest.getContentLength())
			{
				SocketUploadFile.insert(std::make_pair(conn_sock, std::make_pair("", 0)));
				std::string PathToStore = getPathToStore(ServerRequest.getMethod(), GoodServerConf, ServerRequest.getRequestURI());
				while (PathToStore.find("//") != std::string::npos)
					PathToStore = PathToStore.erase(PathToStore.find("//"), 1);
				struct stat path_info;
				memset(&path_info, 0, sizeof(path_info));
				stat(PathToStore.c_str(), &path_info);
				if (S_ISDIR(path_info.st_mode) == 0)
				{
					PathToStore = GoodServerConf->getRoot();
				}
				UploadFilePath.insert(std::pair<int, std::string>(conn_sock, PathToStore));
			}
			else
			{
				server_response ServerResponse(GoodServerConf->getStatusCode(), &ServerRequest);
				ServerResponse.SendingResponse(ServerRequest, conn_sock, GoodServerConf, 413, MsgToSent);
				return 0;
			}
		}
		else if (isNotinUnauthorizedSocket(UnauthorizedSocket, conn_sock))
		{
			server_response ServerResponse(GoodServerConf->getStatusCode(), &ServerRequest);
			ServerResponse.SendingResponse(ServerRequest, conn_sock, GoodServerConf, 200, MsgToSent);
			return 0;
		}
		else
		{
			server_response ServerResponse(GoodServerConf->getStatusCode(), &ServerRequest);
			ServerResponse.setStatusCode(CodeStatus);
			ServerResponse.createResponse(GoodServerConf, "", ServerRequest, 0, 0);
			MsgToSent->insert(std::make_pair(conn_sock, std::make_pair(ServerResponse.getServerResponse(), "")));
			return 0;
		}
	}

	if (SocketUploadFile.find(conn_sock) != SocketUploadFile.end())
	{
		if (SocketUploadFile[conn_sock].first.empty())
		{
			srand(time(0));
			int random_num = rand() % 100000000 + 1;
			std::string temp_filename = ".up" + itos(random_num);
			std::ofstream temp_file(temp_filename.c_str(), std::ios::binary);
			if (!temp_file.is_open())
			{
				SocketUploadFile.erase(SocketUploadFile.find(conn_sock));
				return 1;
			}
			// Store the temporary file path in the map
			SocketUploadFile[conn_sock].first = temp_filename;
		}
		
		std::ofstream temp_file(SocketUploadFile[conn_sock].first.c_str(), std::ios::binary | std::ios::app);
		if (!temp_file.is_open())
		{
			std::remove(SocketUploadFile[conn_sock].first.c_str());
			SocketUploadFile.erase(SocketUploadFile.find(conn_sock));
			return 1;
		}

		size_t pos = 0;
		size_t found = 0;
		while (request.find("WebKitFormBoundary", pos) != std::string::npos)
		{
			pos = request.find("WebKitFormBoundary", pos) + strlen("WebKitFormBoundary");
			found++;
		}

		SocketUploadFile[conn_sock].second = SocketUploadFile[conn_sock].second + found;

		if (FileName.find(conn_sock) == FileName.end() || temp_file.tellp() > 0)
		{
			if (temp_file.tellp() == 0)
			{
				size_t SaveFilePos = request.find("\r\n\r\n", request.find("filename=\""));
				if (SaveFilePos != std::string::npos)
				{
					int FileNamePos = request.find("filename=\"") + strlen("filename=\"");
					FileName.insert(std::make_pair(SocketUploadFile.find(conn_sock)->first, request.substr(FileNamePos, request.find("\"", FileNamePos) - FileNamePos)));
					request = request.substr(SaveFilePos + 4);
					if (request.find("------WebKitFormBoundary") != std::string::npos)
					{
						size_t end_pos = request.size() - request.find("------WebKitFormBoundary");
						temp_file.write(request.c_str(), request.size() - end_pos);
						temp_file.close();
					}
					else
					{
					temp_file.write(request.c_str(), request.size());
					temp_file.close();
					}
				}
			}
			else
			{
					if (request.find("------WebKitFormBoundary") != std::string::npos)
					{
						size_t end_pos = request.size() - request.find("------WebKitFormBoundary");
						temp_file.write(request.c_str(), request.size() - end_pos);
						temp_file.close();
					}
					else
					{
					temp_file.write(request.c_str(), request.size());
					temp_file.close();
					}
			}
		}

		if (SocketUploadFile[conn_sock].second >= 3)
		{
			std::ifstream file(SocketUploadFile[conn_sock].first.c_str());
			if (!file.is_open())
			{
				std::remove(SocketUploadFile[conn_sock].first.c_str());
				SocketUploadFile.erase(SocketUploadFile.find(conn_sock));
				FileName.erase(FileName.find(conn_sock));
				return 1;
			}

			if (UploadFilePath.find(conn_sock) != UploadFilePath.end())
			{
				FileName[conn_sock] = UploadFilePath[conn_sock] + "/" + FileName[conn_sock];
			}

			FileName[conn_sock] = UpdateFileNameifAlreadyExist(FileName[conn_sock].c_str());
			int result = std::rename(SocketUploadFile[conn_sock].first.c_str(), FileName[conn_sock].c_str());
			if (result != 0)
			{
				std::remove(SocketUploadFile[conn_sock].first.c_str());
				SocketUploadFile.erase(SocketUploadFile.find(conn_sock));
				FileName.erase(FileName.find(conn_sock));
				return 1;
			}
			MsgToSent->insert(std::make_pair(conn_sock, std::make_pair("HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=utf-8\r\nContent-Length: 312\r\n\r\n<html><head><meta name=\"viewport\" content=\"width=device-width, minimum-scale=0.1\"><title>200 OK</title></head><body style=\"background: #0e0e0e; height: 100%;text-align:center;color:white;\"><h1>200 OK</h1><img src=\"https://http.cat/200\" style=\"display: block;margin: auto;\" alt=\"200 OK\"g><p>webserv</p></body></html>", "")));
			SocketUploadFile.erase(SocketUploadFile.find(conn_sock));
			FileName.erase(FileName.find(conn_sock));
			UploadFilePath.erase(UploadFilePath.find(conn_sock));

		}
	}

	return 0;
}

void ChangePort(std::map<int, int> &StorePort, int conn_sock, int listen_sock)
{
	for (std::map<int, int>::iterator it = StorePort.begin(); it != StorePort.end(); it++)
	{
		if (it->second == listen_sock)
			it->second = conn_sock;
	}
}

std::multimap<int, int> ChangeOrKeepPort(std::multimap<int, int> *StorePort, int NewConnSock, int PortSock)
{

	for (std::multimap<int, int>::iterator it = StorePort->begin(); it != StorePort->end(); it++)
	{
		if (it->second == PortSock)
		{
			StorePort->insert(std::make_pair(it->first, NewConnSock));
			return (*StorePort);
		}
	}
	return (*StorePort);
}

int isNotPort(int fd, std::vector<int> listen_sock)
{
	for (size_t i = 0; i < listen_sock.size(); i++)
	{
		if (fd == listen_sock[i])
			return 0;
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
	int j = -1;
	std::vector<int> listen_sock;
	int	socktmp = -1;
	std::multimap<int, int> StorePort;
	int CodeStatus = 0;
	std::map<int, std::pair<std::string, std::string> > MsgToSent;
	std::string PartialFileSent;
	static std::map<int, int> PercentageSent;

	signal(SIGPIPE, SIG_IGN);
	for (size_t i = 0; i < Ports.size(); i++)
	{
		socktmp = -1;
		addrlen[i] = sizeof(addr[i]);
		socktmp = socket(AF_INET, SOCK_STREAM, 0);
		if (socktmp == -1) {
			std::cerr << "Error: cannot create socket: " << strerror(errno) << std::endl;
		}
		else
		{

			memset(&addr[i], 0, sizeof(addr[i]));
			addr[i].sin_family = AF_INET;
	
			/****Ci-dessous, tentative de bien lier les adresses IP**********/
			int error_pton = 42;
			if (Hosts[i].size() == 0 || Hosts[i] == "localhost")
			{
				addr[i].sin_addr.s_addr = htonl(INADDR_ANY);
				error_pton = 1;
			}
			else
				error_pton = inet_pton(AF_INET, Hosts[i].c_str(), &addr[i].sin_addr);
			
			if (error_pton != 1)
			{
				close(socktmp);
				std::cout << "\033[1;31m" << "Host " << Hosts[i] << " is not good" << "\033[0m\n" << std::endl;
			}
			else
			{
				addr[i].sin_port = htons(Ports[i]);
				int val = 1;
				if (setsockopt(socktmp, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) < 0) {
					close(socktmp);
					std::cerr << "Error: setsockopt() failed: " << strerror(errno) << std::endl;
				}
				else
				{
					if (bind(socktmp, (struct sockaddr *) &addr[i], addrlen[i]) == -1)
					{
						if (errno == EADDRINUSE) // changer
						{
							std::cout << "\033[1;31m" << "Port " << Ports[i] << " is already listening" << "\033[0m\n" << std::endl;
						}
						close(socktmp);
					}
					else
					{
						std::cout << "\033[1;32m" << "Port " << Ports[i] << " is listening" << "\033[0m\n" << std::endl;
						if (listen(socktmp, SOMAXCONN) == -1) {
							close(socktmp);
							std::cerr << "Error: listen failed: " << strerror(errno) << std::endl;
						}
						else
						{
							listen_sock.push_back(socktmp);
							StorePort.insert(std::make_pair(Ports[i], socktmp));
						}
					}
				}
			}
		}
	}
	if(!listen_sock.size()) // si listen_sock vide, erreur system call lors de la creation des sockets
		return(EXIT_FAILURE);

	epollfd = epoll_create1(0);
	if (epollfd == -1) {
		std::cerr << "Error: epoll_create1: " << strerror(errno) << std::endl;
		return(CloseListenSockets(listen_sock), EXIT_FAILURE);
	}
	errno = 0;
	open_ports.push_back(epollfd);
	
	struct epoll_event	ev, events[MAX_EVENTS];

	for (std::vector<int>::iterator it = listen_sock.begin(); it != listen_sock.end(); it++)
	{
		ev.events = EPOLLIN;
		ev.data.fd = *it;
 		if (epoll_ctl(epollfd, EPOLL_CTL_ADD, *it, &ev) == -1) 
		{
			std::cerr << "Error: epoll_ctl: listen_sock, " << strerror(errno) << std::endl;
			return(CloseListenSockets(listen_sock), EXIT_FAILURE);
		}
	}
	for (;;) {
		if (g_code == 42)
			return(CloseListenSockets(listen_sock), EXIT_FAILURE);
		nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
		if (nfds == -1 || g_code == 42)
			return(CloseListenSockets(listen_sock), EXIT_FAILURE);
		for (int n = 0; n < nfds; ++n)
		{
			for (size_t i = 0; i < listen_sock.size(); i++)
			{
				if (events[n].data.fd == listen_sock[i])
				{
					CodeStatus = 200; // a voir comment on gère le code status après envoi ds le handle connection

					if (sockets.size() > 100)
					{
						close(*(sockets.begin()));
						sockets.erase(sockets.begin());
						for (std::multimap<int, int>::iterator it = StorePort.begin(); it != StorePort.end(); it++)
						{
							if (it->second == *(sockets.begin()))
							{
								StorePort.erase(it);
								break;
							}
						}
					}
					conn_sock = accept(events[n].data.fd, (struct sockaddr *) &addr[i], &addrlen[i]);
					if (conn_sock != -1)
					{
						ev.events = EPOLLIN | EPOLLOUT;
						ev.data.fd = conn_sock;
						if (epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock, &ev) == -1)
							std::cerr << "Error: epoll_ctl: conn_sock, " << strerror(errno) << std::endl;
						else
						{
							sockets.push_back(conn_sock);
							StorePort = ChangeOrKeepPort(&StorePort, conn_sock, events[n].data.fd);
						}
					}
					else
						j++;
					if (j == 1000) // si le accept bug 1000 fois, on finit par fermer le serveur car probleme d'alloc fd
						return(CloseListenSockets(listen_sock), EXIT_FAILURE);
				}
			}
			if ((events[n].events & EPOLLIN) && isNotPort(events[n].data.fd, listen_sock))
			{
				if (handle_connection(servers, events[n].data.fd, StorePort, CodeStatus, &MsgToSent))
				{
					for (std::vector < int >::iterator it2 = sockets.begin(); it2 != sockets.end(); it2++)
					{
						if (*it2 == events[n].data.fd)
						{
							sockets.erase(it2);
							close(conn_sock);
							break;
						}
					}
					epoll_ctl(epollfd, EPOLL_CTL_DEL, events[n].data.fd, &ev);
					close (events[n].data.fd);
				}
			}
			if ((events[n].events & EPOLLOUT) && isNotPort(events[n].data.fd, listen_sock))
			{
				std::map<int, std::pair<std::string, std::string> >::iterator it = MsgToSent.find(events[n].data.fd);
				if (it != MsgToSent.end())
				{
					if (it->second.second.size() > 0)
					{
						if (PercentageSent.find(it->first) == PercentageSent.end())
						{
							if (send(it->first, it->second.first.c_str() , it->second.first.size(), MSG_NOSIGNAL | MSG_DONTWAIT) <= 0)
							{
								for (std::vector < int >::iterator it2 = sockets.begin(); it2 != sockets.end(); it2++)
								{
									if (*it2 == it->first)
									{
										sockets.erase(it2);
										break;
									}
								}
								epoll_ctl(epollfd, EPOLL_CTL_DEL, it->first, &ev);
								close (it->first);
								MsgToSent.erase(it);
							}
							else
								PercentageSent.insert(std::make_pair(it->first, 0));
						}
						else
						{
							std::ifstream file(it->second.second.c_str(), std::ios::binary);
							if (file.is_open())
							{
								char chunk[4096];
								file.seekg(PercentageSent[it->first], std::ios::beg);
								file.read(chunk, 4096);
								std::streamsize bytes_read = file.gcount();
								if (bytes_read == 0)
								{
									if (send(it->first, "\r\n\r\n", 4 , MSG_NOSIGNAL | MSG_DONTWAIT) <= 0)
									{
										for (std::vector < int >::iterator it2 = sockets.begin(); it2 != sockets.end(); it2++)
										{
											if (*it2 == it->first)
											{
												sockets.erase(it2);
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
									if (send(it->first, chunk, bytes_read, MSG_NOSIGNAL | MSG_DONTWAIT) <= 0)
									{
										for (std::vector < int >::iterator it2 = sockets.begin(); it2 != sockets.end(); it2++)
										{
											if (*it2 == it->first)
											{
												sockets.erase(it2);
												break;
											}
										}
										epoll_ctl(epollfd, EPOLL_CTL_DEL, it->first, &ev);
										close (it->first);
										MsgToSent.erase(it);
									}
									else
										PercentageSent[it->first] = PercentageSent[it->first] + bytes_read;
									file.close();
								}
							}
							else //file not open
							{
								if (send(it->first, it->second.first.c_str() , it->second.first.size(), MSG_NOSIGNAL | MSG_DONTWAIT) <= 0)
								{
									for (std::vector < int >::iterator it2 = sockets.begin(); it2 != sockets.end(); it2++)
									{
										if (*it2 == it->first)
										{
											sockets.erase(it2);
											break;
										}
									}
									epoll_ctl(epollfd, EPOLL_CTL_DEL, it->first, &ev);
									close (it->first);
								}
								MsgToSent.erase(it);
							}
						}
					}
					else
					{
						if (send(it->first, it->second.first.c_str() , it->second.first.size(), MSG_NOSIGNAL | MSG_DONTWAIT) <= 0)
						{
							for (std::vector < int >::iterator it2 = sockets.begin(); it2 != sockets.end(); it2++)
							{
								if (*it2 == it->first)
								{
									sockets.erase(it2);
									break;
								}
							}
							epoll_ctl(epollfd, EPOLL_CTL_DEL, it->first, &ev);
							close (it->first);
						}
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
			Hosts.push_back(*ite);
			i++;
		}
	}
	return Hosts;
}
