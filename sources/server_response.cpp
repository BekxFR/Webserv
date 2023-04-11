/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_response.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgruson <mgruson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 15:09:46 by mgruson           #+#    #+#             */
/*   Updated: 2023/04/11 15:58:04 by mgruson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server_response.hpp"
# define DEBUG1 1

server_response::server_response() : _status_code(200), _body(""), _ServerResponse("")
{
	this->addType();
	std::cout << "server_response Default Constructor called" << std::endl;
}

server_response::server_response(int stat) : _status_code(stat), _body(""), _ServerResponse("")
{
	this->addType();
	std::cout << "server_response int Constructor called" << std::endl;
}

server_response::server_response(server_response const &obj)
{
	*this = obj;
}

server_response::~server_response()
{
	std::cout << "server_response Destructor called" << std::endl;
}

server_response &server_response::operator=(server_response const &obj)
{
	(void)obj;
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
	std::cout << "TEST : " << type << std::endl;
	for (std::map<std::string, std::string>::iterator it = _contentType.begin(); it != _contentType.end(); it++)
	{
		if (type == it->first)
			return (it->second);
	}
	return ("Content-Type: text/html\r\n");
}

int server_response::checkConfFile(std::string MethodUsed, server_configuration *server, std::string RequestURI)
{	
	// std::cout << "METHOD : " << MethodUsed << std::endl;
	for (std::map<std::string, class server_location_configuration*>::reverse_iterator it = server->getLoc()->rbegin(); it != server->getLoc()->rend(); it++)
	{
		// std::cout << "IT-FIRST : " << it->first << " Size : " << it->first.size() << std::endl;
		// std::cout << "RequestURI.SUBSTR : " << RequestURI.substr(0, it->first.size()) << std::endl;
		if (it->first == RequestURI.substr(0, it->first.size()))
		{
			for (std::vector<std::string>::reverse_iterator ite = it->second->getHttpMethodAccepted().rbegin(); ite != it->second->getHttpMethodAccepted().rend(); ite++)
			{
				// std::cout << "ITE* : " << *ite << std::endl;
				if (MethodUsed == *ite)
				{
					// std::cout << "IL PASSE ICI" << std::endl;
					// s'il passe ici c'est que la méthode est autorisée et qu'une loc a été trouvée
					// return (200);
				}
				else
				{
					// s'il passe ici, c'est qu'une loc a ete trouvee mais la methode n'est pas trouvee
					// return (405);
				}
			}
		}
	}
	// s'il passe ici c'est qu'aucune loc n'a éte trouvée et que donc c'est possible (sauf interdiction mais non gere)
	return (200);
}

std::string server_response::getRealPath(std::string MethodUsed, server_configuration *server, std::string RequestURI)
{	
	for (std::map<std::string, class server_location_configuration*>::reverse_iterator it = server->getLoc()->rbegin(); it != server->getLoc()->rend(); it++)
	{
		if (it->first == RequestURI.substr(0, it->first.size()))
		{
			for (std::vector<std::string>::reverse_iterator ite = it->second->getHttpMethodAccepted().rbegin(); ite != it->second->getHttpMethodAccepted().rend(); ite++)
			{
				if (MethodUsed == *ite)
				{
					if (it->second->getRoot().size() > 0)
					{
						if (it->second->getRoot().at(it->second->getRoot().size() - 1 ) == '/')
							return ("." + it->second->getRoot() + RequestURI.substr(it->first.size()));
						else
							return ("." + it->second->getRoot() + "/" + RequestURI.substr(it->first.size()));
					}
					else
					{
						if (server->getRoot().at(server->getRoot().size() - 1 ) == '/') 
							return ("." + server->getRoot() + RequestURI.substr(1));
						else
							return ("." + server->getRoot() + "/" + RequestURI.substr(1));
					}
				}
			}
		}
	}
	if (server->getRoot().at(server->getRoot().size() - 1 ) == '/') 
		return ("." + server->getRoot() + RequestURI.substr(1));
	else
		return ("." + server->getRoot() + "/" + RequestURI.substr(1));

}

std::string server_response::getRealPathIndex(std::string MethodUsed, server_configuration *server, std::string RequestURI)
{	
	for (std::map<std::string, class server_location_configuration*>::reverse_iterator it = server->getLoc()->rbegin(); it != server->getLoc()->rend(); it++)
	{
		if (it->first == RequestURI.substr(0, it->first.size()))
		{
			for (std::vector<std::string>::reverse_iterator ite = it->second->getHttpMethodAccepted().rbegin(); ite != it->second->getHttpMethodAccepted().rend(); ite++)
			{
				if (MethodUsed == *ite)
				{
					if (it->second->getDirectoryRequest().size() > 0)
					{
						if (it->second->getRoot().at(it->second->getRoot().size() - 1 ) == '/')
							return ("." + it->second->getRoot() + it->second->getDirectoryRequest());
						else
							return ("." + it->second->getRoot() + "/" + it->second->getDirectoryRequest());
					}
					else
					{
						if (server->getRoot().at(server->getRoot().size() - 1 ) == '/') 
							return ("." + server->getRoot() + server->getIndex());
						else
							return ("." + server->getRoot() + "/" + server->getIndex());
					}
				}
			}
		}
	}
	if (server->getRoot().at(server->getRoot().size() - 1 ) == '/') 
		return ("." + server->getRoot() + server->getIndex());
	else
		return ("." + server->getRoot() + "/" + server->getIndex());
}

std::string server_response::getPathToStore(std::string MethodUsed, server_configuration *server, std::string RequestURI)
{	
	for (std::map<std::string, class server_location_configuration*>::reverse_iterator it = server->getLoc()->rbegin(); it != server->getLoc()->rend(); it++)
	{
		if (it->first == RequestURI.substr(0, it->first.size()))
		{
			for (std::vector<std::string>::reverse_iterator ite = it->second->getHttpMethodAccepted().rbegin(); ite != it->second->getHttpMethodAccepted().rend(); ite++)
			{
				if (MethodUsed == *ite)
				{
					if (it->second->getUploadStore().size() > 0)
					{
						if (it->second->getRoot().at(it->second->getRoot().size() - 1 ) == '/')
							return ("." + it->second->getRoot() + it->second->getUploadStore());
						else
							return ("." + it->second->getRoot() + "/" + it->second->getUploadStore());
					}
					else
						return ("." + server->getRoot());
				}
			}
		}
	}
	return ("." + server->getRoot());
}

bool server_response::isRedir(std::string MethodUsed, server_configuration *server, std::string RequestURI)
{	
	for (std::map<std::string, class server_location_configuration*>::reverse_iterator it = server->getLoc()->rbegin(); it != server->getLoc()->rend(); it++)
	{
		if (it->first == RequestURI.substr(0, it->first.size()))
		{
			for (std::vector<std::string>::reverse_iterator ite = it->second->getHttpMethodAccepted().rbegin(); ite != it->second->getHttpMethodAccepted().rend(); ite++)
			{
				if (MethodUsed == *ite)
				{
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
				if (MethodUsed == *ite)
				{
					if (it->second->getHttpRedirection().size() > 0)
					{
						return (it->second->getHttpRedirection());
					}
				}
			}
		}
	}
	return ("ERROR");
}

void	server_response::todo(const server_request& Server_Request, int conn_sock, server_configuration *server)
{
	/*	Ci-dessous, je verifie que le ClientMaxBodySize n'est pas dépassé.
		Je le mets au-dessus, car si c'est le cas, retour d'erreur*/
	if (Server_Request.getContentLength() > server->getClientMaxBodySize())
	{
		_status_code = 413;
	}
	
	/* Ci-dessous, on vérifie que la méthode est autorisée. On le fait ici
	car sinon un code erreur peut être renvoyé */
	_status_code = checkConfFile(Server_Request.getMethod(), server, Server_Request.getRequestURI()); // on sait s'ils ont le droit
	/********************************************/
	
	enum imethod {GET, POST, DELETE};
	
	std::string RealPath = getRealPath(Server_Request.getMethod(), server, Server_Request.getRequestURI());
	std::string RealPathIndex = getRealPathIndex(Server_Request.getMethod(), server, Server_Request.getRequestURI());
	std::string PathToStore = getPathToStore(Server_Request.getMethod(), server, Server_Request.getRequestURI());
	if (DEBUG1)
	{
		std::cout << "RealPath : " << RealPath << std::endl;
		std::cout << "RealPathIndex : " << RealPathIndex << std::endl;
		std::cout << "PathToStore : " << PathToStore << std::endl;
	}


	/*Ici, on check si c'est le path donné est un directory ou non.
	Une fosis que l'on sait cela, on peut renvoyer un index ou 
	un message erreur */
	struct stat path_info;
	bool is_dir;
	std::string FinalPath;
	if (stat(RealPath.c_str(), &path_info) != 0) {
		/* Si l'on va ici, cela signifie qu'il ne s'agit ni d'un directory, ni d'un file.
		Autrement dit, le PATH n'est pas valide : il faut renvoyer un message d'erreur */
		/* => VOIR AVEC NICO */
		_status_code = 404;
        if (0)
			std::cout << " BOOL FALSE" << std::endl;
    }
	else
	{
		/* Si l'on va ici, c'est qu'il s'agit d'un PATH valide, donc soit un fichier, soit un directory 
		C'est S_ISDIR qui va nous permettre de savoir si c'est un file ou un directory */
		is_dir = S_ISDIR(path_info.st_mode);
		if (0)
			std::cout << " BOOL TRUE is_dir " << is_dir << std::endl;
		if (is_dir)
			FinalPath = RealPathIndex;
		else
			FinalPath = RealPath;
	}
	/************************************************/


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
	
	/*********************************************/

	int n = 0;
	const std::string ftab[3] = {"GET", "POST", "DELETE"};
	std::string		content;
	
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
			std::ifstream file(FinalPath.c_str());
			std::stringstream buffer;
			std::stringstream response;
			if (!file.is_open())
			{
				if (_status_code == 200)
					_status_code = 404;
			}
			else
			{
				buffer << file.rdbuf();
				content = buffer.str();
			}
			std::cerr << "AFTER RESPONSE IFSTREAM\r\n" << std::endl;
			createResponse(server, content, Server_Request);
			std::cout << std::endl << "SERVER RESPONSE CONSTRUITE -> " << std::endl << this->_ServerResponse << std::endl << std::endl;
			send(conn_sock, this->_ServerResponse.c_str() , this->_ServerResponse.size(), 0);
			std::cerr << "\nREPONSE SEND :\n";
			std::cerr << this->_ServerResponse << std::endl;			
			break ;

		}
		case POST :
		{
			// std::string infilename = "./site/42.jpg";
			// std::ifstream inputFile(infilename.c_str(), std::ios::binary);
			// std::stringstream response1;
			std::string outfilename = "./test.jpg"; // OK 1
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
            std::stringstream response;
            std::filebuf* pbuf = file.rdbuf();
            std::size_t size = pbuf->pubseekoff(0, file.end, file.in);
            pbuf->pubseekpos (0,file.in);
            char *buffer= new char[size];
            pbuf->sgetn(buffer, size);
            file.close();
            std::string content(buffer, size);
			std::cout << "\nC2\n" << std::endl;
			/*OK 1*/

            // buffer << file.rdbuf();
            // std::cout << "\nBUFFER = " << buffer.str() << "\r\n" << std::endl;
            // std::string content = buffer.str();
            response << "HTTP/1.1 200 OK\r\n";
            response << "content-Type: image/jpeg\r\n";
            // response << "Content-Type: text/plain; charset=UTF-8\r\n";
            response << "content-Length: " << size << "\r\n";
            response << "\r\n";
            response << content << '\0' << "\r\n";
			outputFile << content ;
			outputFile.close();
            // }
            // response << "Hello world!\r\n";
            std::cerr << "AFTER RESPONSE IFSTREAM\r\n" << std::endl;
            std::cout << buffer << std::endl;
			delete [] buffer;
            std::string response_str = response.str();
            send(conn_sock, response_str.c_str() , response_str.size(), 0);
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
			std::cout << "\ntmp.c_str() = " << FinalPath << "\n" << std::endl;
			if (std::remove(FinalPath.c_str()) != 0) { // the remove function returns 0 on success
        		std::cerr << "Error deleting file: " << '\n';
    			}
    			std::cout << "File deleted successfully: " << '\n';
				
			std::stringstream response;
			response << "HTTP/1.1 200 OK\r\n";
            // response << "Content-Type: text/plain; charset=UTF-8\r\n";
            response << "content-Length: " << 9 << "\r\n";
            response << "\r\n";
            response << "OK delete" << "\r\n";
			std::string response_str = response.str();
            send(conn_sock, response_str.c_str() , response_str.size(), 0);

			break ;
		}
		default :
		{
			std::stringstream response;
			response << "HTTP/1.1 200 OK\r\n";
            response << "content-Length: " << 0 << "\r\n";
            response << "\r\n";
			std::string response_str = response.str();
            send(conn_sock, response_str.c_str() , response_str.size(), 0);
			break ;
		}
	}
	return ;
}

std::string	server_response::addHeader(std::string statusMsg, const server_request& Server_Request)
{
	std::string	header;
	std::stringstream	response;
	
	response << "HTTP/1.1" << " " << _status_code << " " << statusMsg << "\r\n";  /* ajouter la version HTTP (parsing) */ 
	response << this->getType(Server_Request.getType()); // modif text/html (parsing) -> peut etre faire map de content type / mime en fonction de .py = /truc .html = /text/html etc.
	header = response.str();
	return (header);
}

std::string	server_response::addBody(std::string msg)
{
	std::string	body;
	std::stringstream	response;

	response << "Content-Length: " << msg.size() << "\r\n\r\n"; // modif text/html (parsing) -> peut etre faire map de content type / mime en fonction de .py = /truc .html = /text/html etc.
	response << msg << "\r\n";
	body = response.str();
	return (body);
}

void	server_response::createResponse(server_configuration * server, std::string file, const server_request& Server_Request)
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
			std::cout << "JE SUIS DANS INFO" << std::endl;
			switch (_status_code)
				case 100:
				{
					response << addHeader(STATUS100, Server_Request);
					response << addBody(server->getErrorPage()[STATUS100]);
					break;
				}
				case 101:
				{
					break;
				}
			break;
		}
		case SUCCESS:
		{
			std::cout << "JE SUIS DANS SUCCESS" << std::endl;
			switch (_status_code)
			{
				case 200:
				{
					response << addHeader(STATUS200, Server_Request);
					response << addBody(file);
					break;
				}
				case 201:
				{
					break;
				}
				case 202:
				{
					break;
				}
				case 203:
				{
					break;
				}
				case 204:
				{
					break;
				}
				case 205:
				{
					break;
				}
				case 206:
				{
					break;
				}
			}
			break;
		}
		case REDIRECTION:
		{
			std::cout << "JE SUIS DANS REDIRECTION" << std::endl;
			switch (_status_code)
				case 300:
				{
					break;
				}
				case 301:
				{
					break;
				}
				case 302:
				{
					break;
				}
				case 303:
				{
					break;
				}
				case 304:
				{
					break;
				}
				case 305:
				{
					break;
				}
				case 307:
				{
					break;
				}
			break;
		}
		case CLIENT:
		{
			std::cout << "JE SUIS DANS CLIENT" << std::endl;
			switch (_status_code)
			{
				case 400:
				{
					break;
				}
				case 401:
				{
					break;
				}
				case 402:
				{
					break;
				}
				case 403:
				{
					break;
				}
				case 404:
				{
					response << addHeader(STATUS404, Server_Request);
					response << addBody(server->getErrorPage()[STATUS404]);
					break;
				}
				case 405:
				{
					break;
				}
				case 406:
				{
					break;
				}
				case 407:
				{
					break;
				}
				case 408:
				{
					break;
				}
				case 409:
				{
					break;
				}
				case 410:
				{
					break;
				}
				case 411:
				{
					break;
				}
				case 412:
				{
					break;
				}
				case 413:
				{
					break;
				}
				case 414:
				{
					break;
				}
				case 415:
				{
					break;
				}
				case 416:
				{
					break;
				}
				case 417:
				{
				break;
				}
			}
			break;
		}
		case SERVER:
		{
			std::cout << "JE SUIS DANS SERVER" << std::endl;
			switch (_status_code)
			{
				case 500:
				{
					response << addHeader(STATUS500, Server_Request);
					response << addBody(server->getErrorPage()[STATUS500]);
					break;
				}
				case 501:
				{
						break;
				}
				case 502:
				{
					break;
				}
				case 503:
				{
					break;
				}
				case 504:
				{
					break;
				}
				case 505:
				{
					break;
				}
			}
			break;
		}
		default:
			std::cout << "default" << std::endl;
	}
	_ServerResponse = response.str();
}
