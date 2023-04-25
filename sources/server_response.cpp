/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_response.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgruson <mgruson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 15:09:46 by mgruson           #+#    #+#             */
/*   Updated: 2023/04/25 17:56:19 by mgruson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server_response.hpp"
# define DEBUG1 1

server_response::server_response() : _status_code(200), _body(""), _content(""), _ServerResponse("")
{
	this->addType();
	if (0)
		std::cout << "server_response Default Constructor called" << std::endl;
}

server_response::server_response(int stat) : _status_code(stat), _body(""), _content(""), _ServerResponse("")
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

server_response &server_response::operator=(server_response const &obj)
{
	(void)obj;
	if (0)
		std::cout << "server_response Copy assignment operator called" << std::endl;
	return *this;
}

void	server_response::addType()
{
	_contentType.insert(std::make_pair<std::string, std::string>("html", "Content-Type: text/html\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("jpg", "Content-Type: image/jpeg\r\n"));
}

std::string server_response::getType(std::string type)
{
	// std::cout << "TEST : " << type << std::endl;
	for (std::map<std::string, std::string>::iterator it = _contentType.begin(); it != _contentType.end(); it++)
	{
		if (type == it->first)
			return (it->second);
	}
	return ("Content-Type: text/html\r\n");
}

bool	is_dir(const char* path, server_response& sr)
{
	struct stat tab;

	if (stat(path, &tab) == -1)
		return (sr.setStatusCode(500), 1);
	if (S_ISDIR(tab.st_mode))
		return (1);
	return (0);
}

std::string	prev_link(std::string path)
{
	unsigned int	i = 0;
	for (; path.find("/") != std::string::npos; i++){} 

	return (path);
}

std::string	server_response::list_dir(std::string path)
{
	DIR	*dir = NULL;
	struct dirent *send = NULL;
	std::string	content;
	std::stringstream	response;

	while (path.find("//") != std::string::npos)
		path = path.erase(path.find("//"), 1);
	errno = 0;
	dir = opendir(path.c_str());
	if (dir == NULL)
	if (errno == EACCES || errno == EMFILE || errno == ENFILE || errno == ENOENT || errno == ENOMEM || errno == ENOTDIR)
	{
		if (errno == ENOENT || errno == ENOTDIR)
		{
			_status_code = 404;
		}
		else if (errno == EACCES)
			_status_code = 403;
		else if (errno == EMFILE || errno == ENFILE || errno == ENOMEM)
			_status_code = 500;
		return ("");
	}
	send = readdir(dir);
	if (!send)
	{
		_status_code = 500;
		closedir(dir);
		return ("");
	}
	path.erase(0,1);
	response << "<html><head><meta name=\"viewport\" content=\"width=device-width, minimum-scale=0.1\"><title>" << path << "</title></head><body style=\"height: 100%;\"><h1 style=\"padding-top:0.5em;font-size:3em;\">Index of " << path << "/</h1></br><ul style=\"margin-top:10px;margin-bottom:10px;padding-top:10px;padding-bottom:10px;border-size:0.5em;border-top-style:solid;border-bottom-style:solid;\">";
	while (send)
	{
		response << "<li><a href=\"" << path << "/" << send->d_name << "\">" << send->d_name << "</a></li>";
		send = readdir(dir);
	}
	closedir(dir);
	response << "</ul><p style=\"text-align: center;\">webserv</p></body></html>";
	content = response.str();
	return (content);
}

bool	isGenerallyAuthorised(std::string MethodUsed, server_configuration *server, std::string ite)
{
	if (ite == "NOT INDICATED")
	{
		for (std::vector<std::string>::iterator ite2 = server->getHttpMethodAccepted().begin(); ite2 != server->getHttpMethodAccepted().end(); ite2++)
			{
				if (MethodUsed == *ite2)
				{
					return (1);
				}
			}
	}
	return (0);
}

int server_response::isMethodAuthorised(std::string MethodUsed, server_configuration *server, std::string RequestURI)
{	
	for (std::map<std::string, class server_location_configuration*>::reverse_iterator it = server->getLoc()->rbegin(); it != server->getLoc()->rend(); it++)
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

std::string server_response::getRealPath(std::string MethodUsed, server_configuration *server, std::string RequestURI)
{	
	for (std::map<std::string, class server_location_configuration*>::reverse_iterator it = server->getLoc()->rbegin(); it != server->getLoc()->rend(); it++)
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
	return ("");
}

std::string server_response::getRealPathIndex(std::string MethodUsed, server_configuration *server, std::string RequestURI)
{
	std::string IndexPath;
	
	for (std::map<std::string, class server_location_configuration*>::reverse_iterator it = server->getLoc()->rbegin(); it != server->getLoc()->rend(); it++)
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


std::string server_response::getPathToStore(std::string MethodUsed, server_configuration *server, std::string RequestURI)
{	
	for (std::map<std::string, class server_location_configuration*>::reverse_iterator it = server->getLoc()->rbegin(); it != server->getLoc()->rend(); it++)
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



bool server_response::autoindex_is_on(std::string MethodUsed, server_configuration *server, std::string RequestURI)
{	
	for (std::map<std::string, class server_location_configuration*>::reverse_iterator it = server->getLoc()->rbegin(); it != server->getLoc()->rend(); it++)
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
	for (std::map<std::string, class server_location_configuration*>::reverse_iterator it = server->getLoc()->rbegin(); it != server->getLoc()->rend(); it++)
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
	for (std::map<std::string, class server_location_configuration*>::reverse_iterator it = server->getLoc()->rbegin(); it != server->getLoc()->rend(); it++)
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

std::string getFileName(std::string FinalPath)
{

	size_t pos = 0;
	while (FinalPath.find("/", pos + 1) != std::string::npos)
	{
		pos = FinalPath.find("/", pos + 1);
	}
	return (FinalPath.substr(pos + 1));
}

void	server_response::SendingResponse(const server_request& Server_Request, int conn_sock, server_configuration *server)
{

	std::cout << "REQUETE\n" << Server_Request.getServerRequest() << std::endl;
	/*	Ci-dessous, je verifie que le ClientMaxBodySize n'est pas dépassé.
		Je le mets au-dessus, car si c'est le cas, retour d'erreur*/
	if (_status_code == 200 && Server_Request.getContentLength() > server->getClientMaxBodySize())
		_status_code = 413;
	/**********************************************************************/
	
	
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
		
		
	/* Ci-dessous, on vérifie que la méthode est autorisée. On le fait ici
	car sinon un code erreur peut être renvoyé. Je le mets ici pour etre
	sur que le status code n'est pas modifié par la suite */
	if (_status_code == 200)
		_status_code = isMethodAuthorised(Server_Request.getMethod(), server, Server_Request.getRequestURI()); // on sait s'ils ont le droit
	// std::cout << "STATUS isMethodAuthorised : " << _status_code << std::endl;
	/********************************************/

	
	/*Si l'on se situe, ds une location et qu'il y a une HTTP redir alors
	il faut pouvoir renvoyer la redir */
	if (isRedir(Server_Request.getMethod(), server, Server_Request.getRequestURI()) > 0)
	{
		std::stringstream response;
			response << "HTTP/1.1 301 Moved Permanently\r\nLocation: " \
			<< getRedir(Server_Request.getMethod(), server, Server_Request.getRequestURI()) << "\r\n";
			std::string response_str = response.str();
			send(conn_sock, response_str.c_str() , response_str.size(), 0);
	}
	/*********************************************************************/
	
	/*	Dans les fonctions ci-dessous, je recupere des path et ensuite je supprime les double / si necessaire*/
	std::string RealPath;
	std::string RealPathIndex;
	std::string PathToStore;
	std::string FinalPath;
	

	RealPath = getRealPath(Server_Request.getMethod(), server, Server_Request.getRequestURI());
	while (RealPath.find("//") != std::string::npos)
		RealPath = RealPath.erase(RealPath.find("//"), 1);
	RealPathIndex = getRealPathIndex(Server_Request.getMethod(), server, Server_Request.getRequestURI());
	while (RealPathIndex.find("//") != std::string::npos)
		RealPathIndex = RealPathIndex.erase(RealPathIndex.find("//"), 1);
	PathToStore = getPathToStore(Server_Request.getMethod(), server, Server_Request.getRequestURI());
	while (PathToStore.find("//") != std::string::npos)
		PathToStore = PathToStore.erase(PathToStore.find("//"), 1);
	if (0)
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
	else
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
			FinalPath = RealPathIndex;
		}
		else
		{
			// std::cout << " BOOL DIR " << dir << std::endl;
			FinalPath = RealPath;
		}
	}
	// std::cout << "FinalPath : " << FinalPath << std::endl;
	/************************************************/
	
	
	enum imethod {GET, POST, DELETE};
	std::stringstream response;
	int n = 0;
	const std::string ftab[3] = {"GET", "POST", "DELETE"};
	for (; n < 4; n++)
	{
		if (n != 3 && ftab[n] == Server_Request.getMethod()) // OK 
		{
			break ;
		}
	}
	switch (n)
	{
		case GET :
		{
			if (_status_code == 200)
			{
				if (access(FinalPath.c_str(), F_OK) && FinalPath != "./")
					_status_code = 404;
				else
				{
					std::stringstream buffer;
					if (is_dir(FinalPath.c_str(), *this) && autoindex_is_on(Server_Request.getMethod(), server, Server_Request.getRequestURI())) // && auto index no specifie ou on --> demander a Mathieu comment gerer ce parsing dans le fichier de conf car le autoindex peut etre dans une location ou non
					{
						buffer << list_dir(FinalPath);
					}
					else if (is_dir(FinalPath.c_str(), *this) && !autoindex_is_on(Server_Request.getMethod(), server, Server_Request.getRequestURI())) // && auto index no specifie ou on --> demander a Mathieu comment gerer ce parsing dans le fichier de conf car le autoindex peut etre dans une location ou non
					{
						_status_code = 403;
					}
					else if (_status_code == 200)
					{
						std::ifstream file(FinalPath.c_str());
						if (!file.is_open())
						{
							/* cela ne marche pas car il ne rentre pas mm si file est un dir*/
							_status_code = 403;
						}
						else
						{
							buffer << file.rdbuf();
						}
					}
					_content = buffer.str();
				}
			}
			// std::cerr << "AFTER RESPONSE IFSTREAM\r\n" << std::endl;
			// std::cout << "CREATE RESPONSE 200 : " << _status_code << std::endl;
			createResponse(server, _content, Server_Request, id_session);
			// std::cout << std::endl << "SERVER RESPONSE CONSTRUITE -> " << std::endl << _ServerResponse << std::endl << std::endl;
			send(conn_sock, _ServerResponse.c_str() , _ServerResponse.size(), 0);
			// std::cerr << "\nREPONSE SEND :\n";
			// std::cerr << this->_ServerResponse << std::endl;			
			break ;

		}
		case POST :
		{
			std::cout << "BODY\n" << Server_Request.getBody() << std::endl;
			// std::string infilename = "./site/42.jpg";
			// std::ifstream inputFile(infilename.c_str(), std::ios::binary);
			// std::stringstream response1;
			std::string FileName = "./" + getFileName(FinalPath);
			// std::cout << "FILENAME : " << FileName << std::endl;
			std::string outfilename = FileName.c_str(); // PATH DU FICHIER DE SORTIE
			
			std::ofstream outputFile(outfilename.c_str(), std::ios::binary); // OK 1

			// // Get the file size
			// inputFile.seekg(0, std::ios::end);
			// int fileSize = inputFile.tellg();
			// inputFile.seekg(0, std::ios::beg);
			
			// // Read the contents of the file into a buffer
			// std::vector<unsigned char> fileBuffer(fileSize);
			// inputFile.read(reinterpret_cast<char*>(fileBuffer.data()), fileSize);
			
			// inputFile.close();

			// outputFile << reinterpret_cast<char*>(fileBuffer.data());
			// outputFile.close();

			/*OK 1*/
            std::ifstream file(FinalPath.c_str(), std::ifstream::binary);
            // std::stringstream buffer;
            std::filebuf* pbuf = file.rdbuf();
            std::size_t size = pbuf->pubseekoff(0, file.end, file.in);
            pbuf->pubseekpos (0,file.in);
			
			
			std::cout << "\nC2\n" << std::endl;
            char *buffer= new char[size];
            pbuf->sgetn(buffer, size);
            file.close();
            std::string content(buffer, size);
			/*OK 1*/

            // buffer << file.rdbuf();
            // std::cout << "\nBUFFER = " << buffer.str() << "\r\n" << std::endl;
            // std::string content = buffer.str();
            response << "HTTP/1.1 200 OK\r\n";
			
			// response << _contentType.find(Server_Request.getType())->second;
			response << "Content-Type: text/plain; charset=UTF-8\r\n";
			// response << "content-Length: " << size << "\r\n";
			response << "content-Length: " << 14 << "\r\n";
            response << "\r\n";
			response << "Upload succeed" << '\0' << "\r\n";
            // response << content << '\0' << "\r\n";
			outputFile << content;
			outputFile.close();
            // }
            // response << "Hello world!\r\n";
            std::cerr << "AFTER RESPONSE IFSTREAM\r\n" << std::endl;
            std::cout << buffer << std::endl;
			delete [] buffer;
			_ServerResponse = response.str();
			send(conn_sock, _ServerResponse.c_str() , _ServerResponse.size(), 0);
/**********************************************************************************/
			// std::cout << "Successfully wrote to " << outfilename << std::endl;
			// response1 << "HTTP/1.1 200 OK\r\n";
			// response1 << "Content-Length: " << fileSize << "\r\n";
			// response1 << "\r\n";
			// response1.write(reinterpret_cast<char*>(fileBuffer.data()), fileSize);
			// std::string response_str1 = response1.str();
			// send(conn_sock, response_str1.c_str() , response_str1.size(), 0);

			break ;
		}
		case DELETE :
		{
			this->delete_dir(FinalPath.c_str());
			if (_status_code == 200)
				_content = server->getErrorPage()[STATUS200].second;
			createResponse(server, _content, Server_Request, id_session);
			send(conn_sock, _ServerResponse.c_str() , _ServerResponse.size(), 0);
			break ;
		}
		default :
		{
			std::cout << "PASSE TU ICI " << std::endl;
			response << addHeader(STATUS500, server->getErrorPage().find(STATUS500)->second, Server_Request, server, id_session);
			response << addBody(server->getErrorPage()[STATUS500].second);
			_ServerResponse = response.str();
			send(conn_sock, _ServerResponse.c_str() , _ServerResponse.size(), 0);
			break ;
		}
	}
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
	std::string	header;
	std::stringstream	response;
	
	(void)server;
	response << Server_Request.getVersion() << " " << _status_code << " " << statusMsg << "\r\n";
	if (statusContent.first != "")
	{
		if (statusContent.first.find('.', 0) != std::string::npos)
			response << this->getType(statusContent.first.substr(statusContent.first.find('.', 0) + 1));
	}
	else
		response << this->getType(Server_Request.getType()); // modif text/html (parsing) -> peut etre faire map de content type / mime en fonction de .py = /truc .html = text/html etc.
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
	header = response.str();
	std::cout << "\nHEADER\n\n" << header << std::endl;
	return (header);
}

std::string	server_response::addBody(std::string msg)
{
	std::string	body;
	std::stringstream	response;

	response << "Content-Length: " << msg.size() << "\r\n\r\n";
	response << msg << "\r\n";
	body = response.str();
	return (body);
}

void	server_response::createResponse(server_configuration * server, std::string file, const server_request& Server_Request, int IdSession)
{
	std::stringstream	response;
	enum	status { INFO, SUCCESS, REDIRECTION, CLIENT, SERVER };
	int	n = 0;
	// std::cout << "status code Create Response " << _status_code << std::endl;
	int	tmp = _status_code / 100 - 1;
	for (; n != tmp && n < 5; n++) {}
	switch (n)
	{
		case INFO:
		{
			// std::cout << "JE SUIS DANS INFO" << std::endl;
			switch (_status_code)
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
			break;
		}
		case SUCCESS:
		{
			// std::cout << "JE SUIS DANS SUCCESS" << std::endl;
			switch (_status_code)
			{
				case 200:
				{
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
			// std::cout << "JE SUIS DANS REDIRECTION" << std::endl;
			switch (_status_code)
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
			break;
		}
		case CLIENT:
		{
			// std::cout << "JE SUIS DANS CLIENT" << std::endl;
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
					std::cout << " 401 HEADER" << std::endl;
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
			// std::cout << "JE SUIS DANS SERVER" << std::endl;
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

