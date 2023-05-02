/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_response.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgruson <mgruson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 15:09:46 by mgruson           #+#    #+#             */
/*   Updated: 2023/05/02 16:01:59 by mgruson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server_response.hpp"

extern volatile std::sig_atomic_t	g_code;

server_response::server_response() : _status_code(200), _cgiFd(-1), _header(""), _body(""), _content(""), _contentLength(0), _ServerResponse(""), _finalPath(""), _req(NULL), _isCgi(0)
{
	this->addType();
	if (0)
		std::cout << "server_response Default Constructor called" << std::endl;
}

server_response::server_response(int stat, std::vector<std::string> env, server_request* req) : _status_code(stat), _cgiFd(-1), _header(""), _body(""), _content(""), _contentLength(0), _ServerResponse(""), _finalPath(""), _env(env), _req(req), _isCgi(0)
{
	this->addType();
	if (0)
		std::cout << "server_response int Constructor called" << std::endl;
}

server_response::server_response(server_response const &obj)
{
	*this = obj;
}

server_response::~server_response()
{
	if (0)
		std::cout << "server_response Destructor called" << std::endl;
}

server_response	&server_response::operator=(server_response const &obj)
{
	if (this == &obj)
		return (*this);
	_status_code = obj.getStatusCode();
	_cgiFd = obj.getCgiFd();
	_header = obj.getHeader();
	_body = obj.getBody();
	_content = obj.getContent();
	_contentLength = obj.getContentLength();
	_ServerResponse = obj.getServerResponse();
	_finalPath = obj.getPath();
	_req = obj.getReq();
	_env = obj.getEnv();
	_isCgi = obj.getIsCgi();
	_contentType = obj.getContentType();
	if (0)
		std::cout << "server_response Copy assignment operator called" << std::endl;
	return *this;
}

// https://techcommunity.microsoft.com/t5/image/serverpage/image-id/456929i280730750B92FE12/
void	server_response::addType()
{
	_contentType.insert(std::make_pair<std::string, std::string>("html", "Content-Type: text/html; charset=utf-8\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("htm", "Content-Type: text/html\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("shtml", "Content-Type: text/html\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("css", "Content-Type: text/css\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("xml", "Content-Type: text/xml\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("gif", "Content-Type: image/gif\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("jpg", "Content-Type: image/jpeg\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("js", "Content-Type: image/jpeg\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("mjs", "Content-Type: application/javascript\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("atom", "Content-Type: application/atom+xml\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("rss", "Content-Type: application/rss+xml\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("mp4", "Content-Type: video/mp4\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("iso", "Content-Type: multipart/form-data; boundary=----WebKitFormBoundarybC2GrDJYSRCSriwe\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("mml", "Content-Type: text/mathml\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("txt", "Content-Type: text/plain\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("jad", "Content-Type: text/vnd.sun.j2me.app-descriptor\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("wml", "Content-Type: text/vnd.wap.wml\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("htc", "Content-Type: text/x-component\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("avif", "Content-Type: image/avif\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("png", "Content-Type: image/png\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("svg", "Content-Type: image/svg+xml\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("svgz", "Content-Type: image/svg+xml\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("tif", "Content-Type: image/tiff\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("tiff", "Content-Type: image/tiff\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("wbmp", "Content-Type: image/vnd.wap.wbmp\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("webp", "Content-Type: image/webp\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("ico", "Content-Type: image/x-icon\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("jng", "Content-Type: image/x-jng\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("bmp", "Content-Type: image/x-ms-bmp\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("font", "Content-Type: font/woff\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("font", "Content-Type: font/woff2\r\n"));
}

std::string server_response::getType(std::string type)
{
	// std::cout << "TEST : " << type << std::endl;
	for (std::map<std::string, std::string>::iterator it = _contentType.begin(); it != _contentType.end(); it++)
		if (type == it->first)
			return (it->second);
	return ("Content-Type: text/html; charset=utf-8\r\n");
}

std::string findFileName(std::string FinalPath)
{

	size_t pos = 0;
	while (FinalPath.find("/", pos + 1) != std::string::npos)
	{
		pos = FinalPath.find("/", pos + 1);
	}
	return (FinalPath.substr(pos + 1));
}

std::string	server_response::list_dir(std::string path)
{
	DIR	*dir = NULL;
	struct dirent*	send = NULL;
	struct stat	sb;
	std::stringstream	response;
	std::string	testDir;
	std::string	route;

	std::cout << "PATH : '" << path << "'" << std::endl;
	while (path.find("//") != std::string::npos)
		path = path.erase(path.find("//"), 1);
	errno = 0;
	dir = opendir(path.c_str());
	if (dir == NULL)
	{
		if (errno == EACCES || errno == EMFILE || errno == ENFILE || errno == ENOENT || errno == ENOMEM || errno == ENOTDIR)
		{
			if (errno == ENOENT || errno == ENOTDIR)
				_status_code = 404;
			else if (errno == EACCES)
				_status_code = 403;
			else if (errno == EMFILE || errno == ENFILE || errno == ENOMEM)
				_status_code = 500;
			errno = 0;
			return ("");
		}
	}
	send = readdir(dir);
	if (!send)
	{
		_status_code = 500;
		closedir(dir);
		return ("");
	}
	response << "<html><head><meta name=\"viewport\" content=\"width=device-width, minimum-scale=0.1\"><title>" << path << "</title></head><body style=\"height: 100%;\"><h1 style=\"padding-top:0.5em;font-size:3em;\">Index of " << path << "</h1></br><ul style=\"margin-top:10px;margin-bottom:10px;padding-top:10px;padding-bottom:10px;border-size:0.5em;border-top-style:solid;border-bottom-style:solid;\">";
	while (send)
	{
		testDir = path + send->d_name;
		route = send->d_name;
		if ((strcmp(route.data(), ".") || strcmp(route.data(), "..")) && stat(testDir.data(), &sb) == -1)
		{
			_status_code = 500;
			closedir(dir);
			return ("");
		}
		if (S_ISDIR(sb.st_mode) == 1)
			route += "/";
		response << "<li><a href=\"" << route << "\">" << send->d_name << "</a></li>";
		send = readdir(dir);
	}
	closedir(dir);
	response << "</ul><footer style=\"position:fixed;bottom:0;left:0;right:0;background-color:#111;color:white;text-align:center;padding:10px 0;font-size:0.8em;\">Site web créé par Nicolas, Mathieu et Cyril</footer></body></html>";
	return (response.str());
}

std::string server_response::getRealPath(std::string MethodUsed, server_configuration *server, std::string RequestURI)
{	
	for (std::map<std::string, class server_location_configuration*>::reverse_iterator it = server->getLoc().rbegin(); it != server->getLoc().rend(); it++)
	{
		/* Ici, on compare le path donné dans location avec le début de la requestURI, car le path de la location part
		du début de l'URI */
		if (it->first == RequestURI.substr(0, it->first.size()))
		{
			for (std::vector<std::string>::reverse_iterator ite = it->second->getHttpMethodAccepted().rbegin(); ite != it->second->getHttpMethodAccepted().rend(); ite++)
			{
				if (MethodUsed == *ite || isGenerallyAuthorised(MethodUsed, server, *ite))
				{
					/*	Ci dessous, si le getRoot de la location existe, alors on le donne.
						Sinon, on donne le root general. QUID SI YA PAS DE ROOT GENERAL */
					
					if (it->second->getRoot().size() > 0)
					{
						return (it->second->getRoot() + "/" + RequestURI.substr(it->first.size()));
					}
					else
					{
						return (server->getRoot() + "/" + RequestURI.substr(it->first.size()));
					}
				}
			}
		}
	}
	/* Je rajoute cette verification car au-dessus ce n'est verifie que si la Request URI trouve son path
	dans une location */
	if (isGenerallyAuthorised(MethodUsed, server, "NOT INDICATED"))
		return (server->getRoot() + "/" + RequestURI);
	_status_code = 501;
	return ("");
}

std::string server_response::getRealPathIndex(std::string MethodUsed, server_configuration *server, std::string RequestURI)
{
	std::string IndexPath;
	
	for (std::map<std::string, class server_location_configuration*>::reverse_iterator it = server->getLoc().rbegin(); it != server->getLoc().rend(); it++)
	{

		if (it->first == RequestURI.substr(0, it->first.size()))
		{
			for (std::vector<std::string>::reverse_iterator ite = it->second->getHttpMethodAccepted().rbegin(); ite != it->second->getHttpMethodAccepted().rend(); ite++)
			{
				if (MethodUsed == *ite || isGenerallyAuthorised(MethodUsed, server, *ite))
				{
					if (it->second->getDirectoryRequest().size() > 0)
					{
						/*	Ci dessous, je rajoute l'index a IndexPath, puis, avec Access, je vois s'il existe.
							S'il existe, je renvoie Index Path, s'il n'existe pas, je renvoie sans l'index car, alors,
							il faudra afficher le dossier seulement, ou renvoyer une erreur si c'est interdit */
						IndexPath = it->second->getRoot() + "/" + RequestURI.substr(it->first.size()) + "/" + it->second->getDirectoryRequest();
						if (access(IndexPath.c_str(), F_OK) == 0)
							return (it->second->getRoot() + "/" + RequestURI.substr(it->first.size()) + "/" + it->second->getDirectoryRequest());
						else
							return (it->second->getRoot() + "/" + RequestURI.substr(it->first.size()));
					}
					else
					{
						/* Ici, je fais la meme chose, mais dans le cas où aucun index ne serait indiqué dans la location,
							alors je renvoie l'index général. FAUDRAIT-IL PREVOIR LE CAS OU IL N'Y A PAS D'INDEX GENERAL */
						IndexPath = server->getRoot() + "/" + RequestURI.substr(it->first.size()) + "/" + server->getIndex();
						if (access(IndexPath.c_str(), F_OK) == 0)
							return (server->getRoot() + "/" + RequestURI.substr(it->first.size()) + "/" + server->getIndex());
						else
							return (server->getRoot() + "/" + RequestURI.substr(it->first.size()));
					}
				}
			}
		}
	}
	IndexPath = server->getRoot() + "/" + RequestURI.substr(0) + "/" + server->getIndex();
	if (access(IndexPath.c_str(), F_OK) == 0)
		return (server->getRoot() + "/" + RequestURI.substr(0) + "/" + server->getIndex());
	else
		return (server->getRoot() + "/" + RequestURI.substr(0));
	
}

bool server_response::autoindex_is_on(std::string MethodUsed, server_configuration *server, std::string RequestURI)
{	
	for (std::map<std::string, class server_location_configuration*>::reverse_iterator it = server->getLoc().rbegin(); it != server->getLoc().rend(); it++)
	{
		if (it->first == RequestURI.substr(0, it->first.size()))
		{
			for (std::vector<std::string>::reverse_iterator ite = it->second->getHttpMethodAccepted().rbegin(); ite != it->second->getHttpMethodAccepted().rend(); ite++)
			{
				/*	Cela permet de verifier si l'autoindex est on, pour sinon renvoyer une erreur 403 car on 
					ne peut pas lister le directory si c'est off, ds le cas où il n'y aurait pas d'index */
				if (MethodUsed == *ite || isGenerallyAuthorised(MethodUsed, server, *ite))
				{
					if (it->second->getDirectoryListing() == "on")
						return (1);
				}
			}
		}
	}
	if (server->getDirectoryListing() == "on")
		return (1);
	return (0);

}

bool server_response::isRedir(std::string MethodUsed, server_configuration *server, std::string RequestURI)
{	
	for (std::map<std::string, class server_location_configuration*>::reverse_iterator it = server->getLoc().rbegin(); it != server->getLoc().rend(); it++)
	{
		if (it->first == RequestURI.substr(0, it->first.size()))
		{
			for (std::vector<std::string>::reverse_iterator ite = it->second->getHttpMethodAccepted().rbegin(); ite != it->second->getHttpMethodAccepted().rend(); ite++)
			{
				if (MethodUsed == *ite || isGenerallyAuthorised(MethodUsed, server, *ite))
				{
					/*	Ci-dessous, on considère que s'il la redirection est vide 
						Exemple de config:
							return 301 ;
						Ici, on retourne 0, car on considère qu'aucune redirection n'est proposée */
					if (it->second->getHttpRedirection().size() > 0)
					{
						return 1;
					}
					else
					{
						return 0;
					}
				}
			}
		}
	}
	return 0;
}

std::string server_response::getRedir(std::string MethodUsed, server_configuration *server, std::string RequestURI)
{	
	for (std::map<std::string, class server_location_configuration*>::reverse_iterator it = server->getLoc().rbegin(); it != server->getLoc().rend(); it++)
	{
		if (it->first == RequestURI.substr(0, it->first.size()))
		{
			for (std::vector<std::string>::reverse_iterator ite = it->second->getHttpMethodAccepted().rbegin(); ite != it->second->getHttpMethodAccepted().rend(); ite++)
			{
				if (MethodUsed == *ite || isGenerallyAuthorised(MethodUsed, server, *ite))
				{
					/*	Ici, on renverra forcément ci-dessous, cela a ete verifiee ci-dessous */
					if (it->second->getHttpRedirection().size() > 0)
					{
						return (it->second->getHttpRedirection());
					}
				}
			}
		}
	}
	return ("CE RETURN NE SERA JAMAIS EMPRUNTE");
}

int		server_response::getIdSessionOrSetError401(const server_request& Server_Request)
{
	int id_session = 0;
	static std::vector<int> SessionIdGiven;

	if (Server_Request.getServerRequest().find("IdSession=") == std::string::npos)
	{
		// std::ofstream outfile(".session_management.txt", std::ios::app);
		std::ofstream outfile(".session_management.txt", std::ios::out | std::ios::app);
		srand(time(NULL));
		id_session = rand() % INT_MAX;
		// std::cout << "SESSION ID GENERATED : " << id_session << std::endl;
		SessionIdGiven.push_back(id_session);
		outfile << id_session << std::endl;
		outfile.close();
		return (id_session);
    }
	else if (Server_Request.getServerRequest().find("IdSession="))
	{
		int pos = Server_Request.getServerRequest().find("IdSession=") + strlen("IdSession=");
		int end_pos = Server_Request.getServerRequest().find_first_of("; \n", pos);
		int SessionID = atoi(Server_Request.getServerRequest().substr(pos, (end_pos - pos)).c_str());
		std::vector<int> SessionIdGiven;
		std::ifstream infile(".session_management.txt");
		int j = 0;
		while (infile >> j)
			SessionIdGiven.push_back(j);
		for (size_t i = 0; i <= SessionIdGiven.size(); i++)
		{
			// std::cout << "i : " << i << std::endl;
			// std::cout << "SessionIdGiven.size() : " << SessionIdGiven.size() << std::endl;
			// std::cout << "SessionID : " << SessionID << std::endl;
			
			if (SessionIdGiven.size() > 0 && SessionIdGiven[i] == SessionID)
			{
				break;
			}
			else if (i == SessionIdGiven.size())
			{
				// std::cout << "ID SESSION UNKNOWN" << std::endl;
				_status_code = 401;
			}
			infile.close();
		}
	}
	return (0);
}

bool	server_response::manageCgi(const server_request& Server_Request, server_configuration *server)
{
	std::cerr << "HELLO JE SUIS DANS LE CGI" << std::endl;
	if (access(server->getCgi().find("." + Server_Request.getType())->second.data(), X_OK))
		_status_code = 502;
	else
	{
		std::stringstream buffer;
		if (_fileName == "")
			_fileName = ".cgi-tmp.txt";
		if (!doCgi(_finalPath,server))
		{
			std::ifstream	cgiContent(_fileName.data());
			std::getline(cgiContent, _content, '\0'); // on recupere le retour du cgi
			cgiContent.close();
			std::ofstream	cgiChange(_fileName.data(), std::ios::out | std::ios::trunc);
			cgiChange << Server_Request.getVersion() << " " << _status_code << " " << STATUS200 << "\r\n";
			if (_content.find("Content-Length:") == std::string::npos)
				addLength(); // ajout content-length en fonction du retour des cgi
			if (cgiChange)
				cgiChange << _content << "\0";
			else
				_status_code = 500;
			_isCgi = 1;
			return (1);
		}
	}
	return (0);
}

bool	server_response::AnswerGet(const server_request& Server_Request, server_configuration *server)
{
	if (access(_finalPath.c_str(), F_OK) && _finalPath != "./")
	{
		std::cout << "d0 " << std::endl;
		std::cerr << _finalPath << std::endl;
		_status_code = 404;
	}
	else
	{
		std::stringstream buffer;
		if (is_dir(_finalPath.c_str(), *this) && autoindex_is_on(Server_Request.getMethod(), server, Server_Request.getRequestURI())) // && auto index no specifie ou on --> demander a Mathieu comment gerer ce parsing dans le fichier de conf car le autoindex peut etre dans une location ou non
		{
			std::cout << "AUTOLISTING ON" << std::endl;
			buffer << list_dir(_finalPath);
		}
		else if (is_dir(_finalPath.c_str(), *this) && !autoindex_is_on(Server_Request.getMethod(), server, Server_Request.getRequestURI())) // && auto index no specifie ou on --> demander a Mathieu comment gerer ce parsing dans le fichier de conf car le autoindex peut etre dans une location ou non
			_status_code = 403;
		else if (_status_code == 200)
		{
			if (_isCgi == 1)
				_finalPath = _fileName;
			std::ifstream file(_finalPath.c_str());
			if (!file.is_open())
				_status_code = 403;
			else
				buffer << file.rdbuf();
		}
		_content = buffer.str();
	}
	return (1);
}

struct thread_args {
	int* conn_sock;
    std::string* Response;
};

void *server_response::download_file(void *arg)
{
	struct thread_args *args = (struct thread_args *)arg;

	std::cout.write(args->Response->c_str(), 20);
	std::cout << "\n\nTEST THREAD : " << *args->conn_sock << std::endl;
	// std::string ServerResponse = *args->Response;
	// int conn_sock = *args->conn_sock;

	std::string ServerResponse = *args->Response;
	int conn_sock = *args->conn_sock;
	std::cout << "\n\nTEST THREAD SOCK: " << conn_sock << std::endl;
	std::cout << "\n\nTEST THREAD SIZE : " << ServerResponse.size() << std::endl;
	while (ServerResponse.size() > 0)
	{
		// std::cout << "c3\n" << std::endl;
		static std::string StockResponse;
		// std::cout << "TEST UPLOAD 0\n" << ServerResponse.size() << std::endl;
		static int i = 0;
		// std::cout << "c3.1\n" << std::endl;
		if (ServerResponse.size() >= 500000)
		{
			// std::cout << "c3.2\n" << std::endl;
			StockResponse = ServerResponse.substr(500000);
			// std::cout << "c3.3\n" << std::endl;
			ServerResponse = ServerResponse.erase(500000);
			// std::cout << "c3.4\n" << std::endl;
		}
		std::cout << "\nTEST SERVERREPONSE SIZE for " << conn_sock << " : " << ServerResponse.size() << std::endl;
		// if (ServerResponse.size() < 2000000)
		// {
			
		// }
		// if (i < 1 || ServerResponse.size() < 2000000)
		// {
		// 	std::cout << "\nTEST UPLOAD \n" << std::endl;
		// 	std::cout.write(ServerResponse.c_str(), ServerResponse.size());
		// }
		// std::cout << "RESPONSE SIZE : " << ServerResponse.size() << std::endl;
		// std::cout << "STOCK SIZE : " << StockResponse.size() << std::endl;
		// std::cout << "c4\n" << std::endl;
		usleep(2000);
		send(conn_sock, ServerResponse.c_str() , ServerResponse.size(), 0);
		
		// if (ServerResponse.size() < 2000000)
		// {
		// 	StockResponse.clear();
		// }	
		// std::cout << "c5\n" << std::endl;
		ServerResponse = StockResponse;
		StockResponse.clear();
		i++;
	}
	delete args->Response;
	delete args->conn_sock;
	delete args;
	return NULL;
}

void	server_response::SendingResponse(const server_request& Server_Request, int conn_sock, server_configuration *server,  int StatusCodeTmp)
{
	if (StatusCodeTmp != 200)
		_status_code = StatusCodeTmp;

	/*	Ci-dessous, on genere un ID de session pour chaque nouvel utilisateur
		et on verifie que si un ID est recu, c'est bien nous qui l'avons emis
		pour renvoyer sinon une erreur 401 et un id_session a zero (a savoir 
		default d authorisation)*/
	int id_session = 0;
	if (_status_code == 200)
		id_session = getIdSessionOrSetError401(Server_Request);
	// std::cout << "_status_code : " << _status_code << std::endl;
	// std::cout << "id_session : " << id_session << std::endl;
	/*********************************************************************/

	
	/*Si l'on se situe, ds une location et qu'il y a une HTTP redir alors
	il faut pouvoir renvoyer la redir */
	if (isRedir(Server_Request.getMethod(), server, Server_Request.getRequestURI()) > 0)
	{
		std::stringstream response;
			response << "HTTP/1.1 301 Moved Permanently\r\nLocation: " \
			<< getRedir(Server_Request.getMethod(), server, Server_Request.getRequestURI()) << "\r\n";
			std::string response_str = response.str();
			errno = 0;
			if (send(conn_sock, response_str.c_str() , response_str.size(), 0) == -1)
				std::cout << "\nError for " << conn_sock << " : " << errno << std::endl;
			errno = 0;
	}
	/*********************************************************************/
	
	/*	Dans les fonctions ci-dessous, je recupere des path et ensuite je supprime les double / si necessaire*/
	std::string RealPath;
	std::string RealPathIndex;
	std::string PathToStore;
	
	RealPath = getRealPath(Server_Request.getMethod(), server, Server_Request.getPath());
	while (RealPath.find("//") != std::string::npos)
		RealPath = RealPath.erase(RealPath.find("//"), 1);
	RealPathIndex = getRealPathIndex(Server_Request.getMethod(), server, Server_Request.getRequestURI());
	while (RealPathIndex.find("//") != std::string::npos)
		RealPathIndex = RealPathIndex.erase(RealPathIndex.find("//"), 1);
	// PathToStore = getPathToStore(Server_Request.getMethod(), server, Server_Request.getRequestURI());
	// while (PathToStore.find("//") != std::string::npos)
	// 	PathToStore = PathToStore.erase(PathToStore.find("//"), 1);
	if (1)
	{
		std::cout << "RealPath : " << RealPath << std::endl;
		std::cout << "RealPathIndex : " << RealPathIndex << std::endl;
		std::cout << "PathToStore : " << PathToStore << std::endl;
	}
	/*Ensuite, on check si c'est le path donné est un directory ou non.
	Une fosis que l'on sait cela, on peut renvoyer un index ou 
	un message erreur */
	struct stat path_info;
	bool dir;
	if (stat(RealPath.c_str(), &path_info) != 0) {
		/* Si l'on va ici, cela signifie qu'il ne s'agit ni d'un directory, ni d'un file.
		Autrement dit, le PATH n'est pas valide : il faut renvoyer un message d'erreur */
		_status_code = 404;
		// std::cout << " BOOL FALSE" << std::endl;
	}
	else if (_status_code == 200)
	{
		/* Si l'on va ici, c'est qu'il s'agit d'un PATH valide, donc soit un fichier, soit un directory 
		C'est S_ISDIR qui va nous permettre de savoir si c'est un file ou un directory */
		dir = S_ISDIR(path_info.st_mode);
		// std::cout << " BOOL TRUE is_dir " << dir << std::endl;
		// std::cout << " BOOL TEST " << RealPath.at(RealPath.size() - 1) << std::endl;
		if (dir && RealPath.at(RealPath.size() - 1) != '/')
		{
			// std::cout << " BOOL 404 " << dir << std::endl;
			_status_code = 404;
		}
		else if (dir)
		{
			// std::cout << " BOOL INDEX " << dir << std::endl;
			_finalPath = RealPathIndex;
		}
		else
		{
			// std::cout << " BOOL DIR " << dir << std::endl;
			_finalPath = RealPath;
		}
	}
	/* A VOIR DEMAIN MAIS FINAL PATH ETAIT MAL INITIALISE DS ANSWER GET*/
	// std::cout << "FinalPath : " << _finalPath << std::endl;
	// std::cout << "StatusCode : " << _status_code << std::endl;
	/************************************************/

	/* Gestion des CGI pour ensuite repondre à la requete*/
	if (server->getCgi().find("." + Server_Request.getType()) != server->getCgi().end() && _status_code == 200)
	{
		std::cerr << "cgi" << std::endl;
		manageCgi(Server_Request, server);
	}
	
	
	std::stringstream response;
	if (Server_Request.getMethod() == "GET")
	{
		if (_status_code == 200)
			AnswerGet(Server_Request, server);
		std::cout << "c10 " << std::endl;
		createResponse(server, _content, Server_Request, id_session);
		if (_ServerResponse.size() > 2000000)
		{
			pthread_t download_thread;
			struct thread_args *args = (struct thread_args *)malloc(sizeof(struct thread_args));
			std::cout << "c1\n" << std::endl;
			args->Response = new std::string(_ServerResponse);
			args->conn_sock = new int(conn_sock);
			std::cout << "\nTHREAD\n" << std::endl;
			if (pthread_create(&download_thread, NULL, &server_response::download_file, (void *)args) != 0) 
			{
				perror("Error creating thread");
				return ;
			}
			if (pthread_detach(download_thread) != 0)
			{
				perror("Error detaching thread");
				return ;
			}
		}
		else
			send(conn_sock, _ServerResponse.c_str() , _ServerResponse.size(), 0);
		return ;
	}
	else if (Server_Request.getMethod() == "POST" || _status_code == 201)
	{
		std::cout << "POST POST POST" << std::endl;
			// std::cout << "BODY\n" << Server_Request.getBody() << std::endl;

			// std::string FileName = "./" + findFileName(_finalPath);
			// // std::cout << "FILENAME : " << FileName << std::endl;
			// std::string outfilename = FileName.c_str(); // PATH DU FICHIER DE SORTIE
			
			// std::ofstream outputFile(outfilename.c_str(), std::ios::binary); // OK 1

			// std::ifstream file(_finalPath.c_str(), std::ifstream::binary);
			// std::stringstream buffer;
			// std::filebuf* pbuf = file.rdbuf();
			// std::size_t size = pbuf->pubseekoff(0, file.end, file.in);
			// pbuf->pubseekpos (0,file.in);
			// // std::cout << "\nC2\n" << std::endl;
			// char *buffer= new char[size];
			// pbuf->sgetn(buffer, size);
			// file.close();
			// std::string content(buffer, size);

			response << "HTTP/1.1 200 OK\r\n";
			
			// response << _contentType.find(Server_Request.getType())->second;
			response << "Content-Type: text/plain; charset=UTF-8\r\n";
			// response << "content-Length: " << size << "\r\n";
			if (_status_code == 201)
			{
				response << "content-Length: " << 16 << "\r\n";
				response << "\r\nUpload succeed\r\n";
			}
			else
			{
				response << "content-Length: " << 11 << "\r\n";
				response << "\r\nUploading\r\n";
			}
			// response << content << '\0' << "\r\n";
			// outputFile << content;
			// outputFile.close();
			// std::cerr << "AFTER RESPONSE IFSTREAM\r\n" << std::endl;
			// std::cout << buffer << std::endl;
			// delete [] buffer;
			_ServerResponse = response.str();
			send(conn_sock, _ServerResponse.c_str() , _ServerResponse.size(), 0);
			std::cout << "\ne10\n" << std::endl;
			// return ;
			// /**
			//  If one or more resources has been created on the origin server as a result of successfully 
			//  processing a POST request, the origin server SHOULD send a 201 (Created) response containing 
			//  a Location header field that provides an identifier for the primary resource created 
			//  (Section 10.2.2) and a representation that describes the status of the request while referring to 
			//  the new resource(s).
			//  https://httpwg.org/specs/rfc9110.html#POST => A LIRE
			//  * 
			// */
			return ;
	}
	else if (Server_Request.getMethod() == "DELETE")
	{
		if (_status_code == 200)
			this->delete_dir(_finalPath.c_str());
		if (_status_code == 200)
			_content = server->getErrorPage()[STATUS200].second;
		createResponse(server, _content, Server_Request, id_session);
		send(conn_sock, _ServerResponse.c_str() , _ServerResponse.size(), 0);
		return ;
	}
	else
	{
		response << addHeader(STATUS500, server->getErrorPage().find(STATUS500)->second, Server_Request, server, id_session);
		response << addBody(server->getErrorPage()[STATUS500].second);
		_ServerResponse = response.str();
		send(conn_sock, _ServerResponse.c_str() , _ServerResponse.size(), 0);
		return ;
	}
	if (_isCgi)
		std::remove(_fileName.data());
	_isCgi = 0;	
	return ;
}

static int	delete_fct(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf)
{
	static_cast<void>(sb);
	static_cast<void>(ftwbuf);

	if (tflag == FTW_DP)
	{
		if (rmdir(fpath))
			return (1);
	}
	else
		if (unlink(fpath))
			return (1);
	return (0);
}

void	server_response::delete_dir(const char* path)
{
	if (access(path, F_OK))
		_status_code = 404;
	else
		if (nftw(path, &delete_fct, 1, FTW_DEPTH))
			_status_code = 403;
}

std::string	server_response::addHeader(std::string statusMsg, std::pair<std::string, std::string> statusContent, const server_request& Server_Request, server_configuration *server, int IdSession)
{
	std::stringstream	response;
	
	response << Server_Request.getVersion() << " " << _status_code << " " << statusMsg << "\r\n";
	if (statusContent.first != "")
	{
		if (statusContent.first.find('.', 0) != std::string::npos)
			response << this->getType(statusContent.first.substr(statusContent.first.find('.', 0) + 1));
	}
	else
		response << this->getType(Server_Request.getType());
	if (Server_Request.getServerRequest().find("IdSession=") == std::string::npos && _status_code != 401)
		response << "Set-Cookie: " << "IdSession=" << IdSession << "\n"; // tentative d'implementation des cookies
	if (server->getCookieHeader().size() != 0)
	{
		std::vector<std::string> CookieHeader = server->getCookieHeader();
		for (std::vector<std::string>::iterator it = CookieHeader.begin(); it != CookieHeader.end(); it++)
		{
			// std::cout << "Set-Cookie: " << *it << "\n"; 
			response << "Set-Cookie: " << *it << "\n"; // tentative d'implementation des cookies
		}
	}
	_header = response.str();
	return (_header);
}

std::string	server_response::addBody(std::string msg)
{
	std::stringstream	response;

	_contentLength = msg.size();
	if (_isCgi == 0)
		response << "Content-Length: " << _contentLength << "\r\n\r\n";
	response << msg << "\r\n";
	_body = response.str();
	return (_body);
}

void	server_response::addLength()
{
	std::stringstream	l;
	std::string	tmp = "Content-Length: ";

	if (_content.size() > 0 && _content.find_first_of("\n\n") + 4 < _content.size())
		l << _content.size() - (_content.find_first_of("\n\n") + 4) << "\r\n"; // on retire les \n et les retours a la ligne generees par le cgi
	else
		l << 0 << "\r\n";
	tmp += l.str();
	_content.insert(0, tmp);
}

void	server_response::createResponse(server_configuration * server, std::string file, const server_request& Server_Request, int IdSession)
{
	std::stringstream	response;
	enum	status { INFO, SUCCESS, REDIRECTION, CLIENT, SERVER };
	int	n = 0;
	int	tmp = _status_code / 100 - 1;

	for (; n != tmp && n < 5; n++) {}
	switch (n)
	{
		case INFO:
		{
			switch (_status_code)
			{
				case 100:
				{
					response << addHeader(STATUS100, server->getErrorPage().find(STATUS100)->second, Server_Request, server, IdSession);
					response << addBody(server->getErrorPage()[STATUS100].second);
					break;
				}
				case 101:
				{
					response << addHeader(STATUS101, server->getErrorPage().find(STATUS101)->second, Server_Request, server, IdSession);
					response << addBody(server->getErrorPage()[STATUS101].second);
					break;
				}
			}
			break;
		}
		case SUCCESS:
		{
			switch (_status_code)
			{
				case 200:
				{
					if (_isCgi == 0)
						response << addHeader(STATUS200, server->getErrorPage().find(STATUS200)->second, Server_Request, server, IdSession);
					response << addBody(file);
					break;
				}
				case 201:
				{
					response << addHeader(STATUS201, server->getErrorPage().find(STATUS201)->second, Server_Request, server, IdSession);
					response << addBody(server->getErrorPage()[STATUS201].second);
					break;
				}
				case 202:
				{
					response << addHeader(STATUS202, server->getErrorPage().find(STATUS202)->second, Server_Request, server, IdSession);
					response << addBody(server->getErrorPage()[STATUS202].second);
					break;
				}
				case 203:
				{
					response << addHeader(STATUS203, server->getErrorPage().find(STATUS203)->second, Server_Request, server, IdSession);
					response << addBody(server->getErrorPage()[STATUS203].second);
					break;
				}
				case 204:
				{
					response << addHeader(STATUS204, server->getErrorPage().find(STATUS204)->second, Server_Request, server, IdSession);
					response << addBody(server->getErrorPage()[STATUS204].second);
					break;
				}
				case 205:
				{
					response << addHeader(STATUS205, server->getErrorPage().find(STATUS205)->second, Server_Request, server, IdSession);
					response << addBody(server->getErrorPage()[STATUS205].second);
					break;
				}
				case 206:
				{
					response << addHeader(STATUS206, server->getErrorPage().find(STATUS206)->second, Server_Request, server, IdSession);
					response << addBody(server->getErrorPage()[STATUS206].second);
					break;
				}
			}
			break;
		}
		case REDIRECTION:
		{
			switch (_status_code)
			{
				case 300:
				{
					response << addHeader(STATUS300, server->getErrorPage().find(STATUS300)->second, Server_Request, server, IdSession);
					response << addBody(server->getErrorPage()[STATUS300].second);
					break;
				}
				case 301:
				{
					response << addHeader(STATUS301, server->getErrorPage().find(STATUS301)->second, Server_Request, server, IdSession);
					response << addBody(server->getErrorPage()[STATUS301].second);
					break;
				}
				case 302:
				{
					response << addHeader(STATUS302, server->getErrorPage().find(STATUS302)->second, Server_Request, server, IdSession);
					response << addBody(server->getErrorPage()[STATUS302].second);
					break;
				}
				case 303:
				{
					response << addHeader(STATUS303, server->getErrorPage().find(STATUS303)->second, Server_Request, server, IdSession);
					response << addBody(server->getErrorPage()[STATUS303].second);
					break;
				}
				case 304:
				{
					response << addHeader(STATUS304, server->getErrorPage().find(STATUS304)->second, Server_Request, server, IdSession);
					response << addBody(server->getErrorPage()[STATUS304].second);
					break;
				}
				case 305:
				{
					response << addHeader(STATUS305, server->getErrorPage().find(STATUS305)->second, Server_Request, server, IdSession);
					response << addBody(server->getErrorPage()[STATUS305].second);
					break;
				}
				case 307:
				{
					response << addHeader(STATUS307, server->getErrorPage().find(STATUS307)->second, Server_Request, server, IdSession);
					response << addBody(server->getErrorPage()[STATUS307].second);
					break;
				}
			}
			break;
		}
		case CLIENT:
		{
			switch (_status_code)
			{
				case 400:
				{
					response << addHeader(STATUS400, server->getErrorPage().find(STATUS400)->second, Server_Request, server, IdSession);
					response << addBody(server->getErrorPage()[STATUS400].second);
					break;
				}
				case 401:
				{
					response << addHeader(STATUS401, server->getErrorPage().find(STATUS401)->second, Server_Request, server, IdSession);
					response << addBody(server->getErrorPage()[STATUS401].second);
					break;
				}
				case 402:
				{
					response << addHeader(STATUS402, server->getErrorPage().find(STATUS402)->second, Server_Request, server, IdSession);
					response << addBody(server->getErrorPage()[STATUS402].second);
					break;
				}
				case 403:
				{
					response << addHeader(STATUS403, server->getErrorPage().find(STATUS403)->second, Server_Request, server, IdSession);
					response << addBody(server->getErrorPage()[STATUS403].second);
					break;
				}
				case 404:
				{
					response << addHeader(STATUS404, server->getErrorPage().find(STATUS404)->second, Server_Request, server, IdSession);
					response << addBody(server->getErrorPage()[STATUS404].second);
					break;
				}
				case 405:
				{
					response << addHeader(STATUS405, server->getErrorPage().find(STATUS405)->second, Server_Request, server, IdSession);
					response << addBody(server->getErrorPage()[STATUS405].second);
					break;
				}
				case 406:
				{
					response << addHeader(STATUS406, server->getErrorPage().find(STATUS406)->second, Server_Request, server, IdSession);
					response << addBody(server->getErrorPage()[STATUS406].second);
					break;
				}
				case 407:
				{
					response << addHeader(STATUS407, server->getErrorPage().find(STATUS407)->second, Server_Request, server, IdSession);
					response << addBody(server->getErrorPage()[STATUS407].second);
					break;
				}
				case 408:
				{
					response << addHeader(STATUS408, server->getErrorPage().find(STATUS408)->second, Server_Request, server, IdSession);
					response << addBody(server->getErrorPage()[STATUS408].second);
					break;
				}
				case 409:
				{
					response << addHeader(STATUS409, server->getErrorPage().find(STATUS409)->second, Server_Request, server, IdSession);
					response << addBody(server->getErrorPage()[STATUS409].second);
					break;
				}
				case 410:
				{
					response << addHeader(STATUS410, server->getErrorPage().find(STATUS410)->second, Server_Request, server, IdSession);
					response << addBody(server->getErrorPage()[STATUS410].second);
					break;
				}
				case 411:
				{
					response << addHeader(STATUS411, server->getErrorPage().find(STATUS411)->second, Server_Request, server, IdSession);
					response << addBody(server->getErrorPage()[STATUS411].second);
					break;
				}
				case 412:
				{
					response << addHeader(STATUS412, server->getErrorPage().find(STATUS412)->second, Server_Request, server, IdSession);
					response << addBody(server->getErrorPage()[STATUS412].second);
					break;
				}
				case 413:
				{
					response << addHeader(STATUS413, server->getErrorPage().find(STATUS413)->second, Server_Request, server, IdSession);
					response << addBody(server->getErrorPage()[STATUS413].second);
					break;
				}
				case 414:
				{
					response << addHeader(STATUS414, server->getErrorPage().find(STATUS414)->second, Server_Request, server, IdSession);
					response << addBody(server->getErrorPage()[STATUS414].second);
					break;
				}
				case 415:
				{
					response << addHeader(STATUS415, server->getErrorPage().find(STATUS415)->second, Server_Request, server, IdSession);
					response << addBody(server->getErrorPage()[STATUS415].second);
					break;
				}
				case 416:
				{
					response << addHeader(STATUS416, server->getErrorPage().find(STATUS416)->second, Server_Request, server, IdSession);
					response << addBody(server->getErrorPage()[STATUS416].second);
					break;
				}
				case 417:
				{
					response << addHeader(STATUS417, server->getErrorPage().find(STATUS417)->second, Server_Request, server, IdSession);
					response << addBody(server->getErrorPage()[STATUS417].second);
					break;
				}
			}
			break;
		}
		case SERVER:
		{
			switch (_status_code)
			{
				case 500:
				{
					response << addHeader(STATUS500, server->getErrorPage().find(STATUS500)->second, Server_Request, server, IdSession);
					response << addBody(server->getErrorPage()[STATUS500].second);
					break;
				}
				case 501:
				{
					response << addHeader(STATUS501, server->getErrorPage().find(STATUS501)->second, Server_Request, server, IdSession);
					response << addBody(server->getErrorPage()[STATUS501].second);
					break;
				}
				case 502:
				{
					response << addHeader(STATUS502, server->getErrorPage().find(STATUS502)->second, Server_Request, server, IdSession);
					response << addBody(server->getErrorPage()[STATUS502].second);
					break;
				}
				case 503:
				{
					response << addHeader(STATUS503, server->getErrorPage().find(STATUS503)->second, Server_Request, server, IdSession);
					response << addBody(server->getErrorPage()[STATUS503].second);
					break;
				}
				case 504:
				{
					response << addHeader(STATUS504, server->getErrorPage().find(STATUS504)->second, Server_Request, server, IdSession);
					response << addBody(server->getErrorPage()[STATUS504].second);
					break;
				}
				case 505:
				{
					response << addHeader(STATUS505, server->getErrorPage().find(STATUS505)->second, Server_Request, server, IdSession);
					response << addBody(server->getErrorPage()[STATUS505].second);
					break;
				}
			}
			break;
		}
		default:
			response << addHeader(STATUS500, server->getErrorPage().find(STATUS500)->second, Server_Request, server, IdSession);
			response << addBody(server->getErrorPage()[STATUS500].second);
			break;
	}
	_ServerResponse = response.str();
}

// ajouter dans l'env avant exec (source https://www.youtube.com/watch?v=37choLzDTgY) :
// CONTENT_TYPE=
// CONTENT_LENGTH=
// HTTP_COOKIE=(askip dans le header)
// HTTP_USER_AGENT=(web browser surement dans le header requete)
// PATH_INFO=(path cgi script (on l'a))
// QUERY_STRING=(the url-encoded information that is sent with GET method request)
// REMOTE_ADDR=(the ip address of the remote host making the request. pour authentification)
// REMOTE_HOST=(the fully qualified name of the host making the request, not mandatory)
// REQUEST_MET_HOD=(the method used to make the request (GET / POST / DELETE))
// SCRIPT_FILENAME=(the full path to the cgi script)
// SCRIPT_NAME=(the name of the cgi script)
// SERVER_NAME=(hostname or ip address)
// SERVER_SOFWARE=(name and version of the software the server is running)

//https://docstore.mik.ua/orelly/linux/cgi/ch03_02.htm
int server_response::doCgi(std::string toexec, server_configuration * server) // envoyer path du cgi
{
	char	buff[256];
	std::string	cgiPath;

	_env.push_back("SERVER_SOFTWARE=Webserv/1.0");
	std::string		servNameEnv = "SERVER_NAME=";
	if (_req->getHost().find("host") != std::string::npos)
		servNameEnv += _req->getHost();
	else
	{
		if (server->getServerName()[0])
			servNameEnv += server->getServerName()[0];
		else
			servNameEnv += "localhost";
	}
	_env.push_back(servNameEnv);
	_env.push_back("AUTH_TYPE=");
	_env.push_back("SERVER_PROTOCOL=" + _req->getVersion());
	_env.push_back("SERVER_PORT=" + itos(server->getPort()[0]));
	std::string	cwd = getcwd(buff, 256);
	_env.push_back("DOCUMENT_ROOT=" + cwd);
	_env.push_back("REQUEST_METHOD=" + _req->getMethod());
	_env.push_back("SCRIPT_FILENAME=" + cwd + toexec.substr(1));
	cgiPath = server->getCgi().find("." + _req->getType())->second;
	_env.push_back("SCRIPT_NAME=" + toexec.substr(1));
	_env.push_back("QUERY_STRING=" + _req->getQuery()); // TODO
	// _env.push_back("QUERY_STRING=dir=OUAIS");// + _req->getQuery());// a pas l'info dans la requete ->The query information from requested URL (i.e., the data following "?").
	_env.push_back("PATH_INFO=" + cgiPath);
	_env.push_back("REQUEST_URI=/");// + _req->getRequestURI());
	_env.push_back("REDIRECT_STATUS=1");
	if (_body.find(std::string("content-length")) != std::string::npos)
		_env.push_back(std::string("CONTENT_LENGTH=") + itos(_contentLength));
	if (this->getType(_req->getType()) != "")
		_env.push_back(std::string("CONTENT_TYPE=") + this->getType(_req->getType()).substr(14, 500));
	if (_body.size() > 0)
	{
		this->_cgiFd = open(this->_req->getBody().data(), O_RDONLY);
		if (this->_cgiFd < 0)
		{
			_status_code = 500;
			return (-1);
		}
	}
	try
	{
		int status = 0;
		Cgi cgi(cgiPath, toexec, _env, _cgiFd, _fileName);
		waitpid(cgi.getPid(), &status, 0);
		if (WIFEXITED(status))
			if (WEXITSTATUS(status) != 0)
				_status_code = 500;
		if (g_code == 1)
		{
			_status_code = 500;
			g_code = 0;
		}
		this->_cgiFd = -1;
	}
	catch (std::exception const &e)
	{
		std::cerr << e.what() << std::endl;
		_status_code = 500;
	}
	if (_status_code != 200)
		return (1);
	return (0);
}
