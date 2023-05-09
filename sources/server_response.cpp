/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_response.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgruson <mgruson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 15:09:46 by mgruson           #+#    #+#             */
/*   Updated: 2023/05/09 19:00:28 by mgruson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server_response.hpp"

extern volatile std::sig_atomic_t	g_code;
server_response::server_response() : _status_code(200), _cgiFd(-1), _header(""), _bodyName(".tmp-post.txt"), _body(""), _content(""), _contentLength(0), _ServerResponse(""), _finalPath(""), _env(), _req(NULL), _isCgi(0)
{
	this->addType();
	if (0)
		std::cout << "server_response Default Constructor called" << std::endl;
}
server_response::server_response(int stat, server_request* req) : _status_code(stat), _cgiFd(-1), _header(""), _bodyName(".tmp-post.txt"), _body(req->getBody()), _content(""), _contentLength(0), _ServerResponse(""), _finalPath(""), _env(), _req(req), _isCgi(0)
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
	_bodyName = obj.getBodyName();
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
	_contentType.insert(std::make_pair<std::string, std::string>("aac", "Content-Type: audio/aac\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("abw", "Content-Type: application/x-abiword\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("arc", "Content-Type: application/x-freearc\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("atom", "Content-Type: application/atom+xml\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("avi", "Content-Type: video/x-msvideo\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("avif", "Content-Type: image/avif\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("azw", "Content-Type: application/vnd.amazon.ebook\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("bin", "Content-Type: application/octet-stream\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("bmp", "Content-Type: image/x-ms-bmp\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("bz", "Content-Type: application/x-bzip\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("bz2", "Content-Type: application/x-bzip2\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("cda", "Content-Type: application/x-cdf\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("csh", "Content-Type: application/x-csh\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("css", "Content-Type: text/css\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("csv", "Content-Type: text/csv\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("doc", "Content-Type: application/msword\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("docx", "Content-Type: application/vnd.openxmlformats-officedocument.wordprocessingml.document\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("eot", "Content-Type: application/vnd.ms-fontobject\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("epub", "Content-Type: application/epub+zip\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("font", "Content-Type: font/woff2\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("font", "Content-Type: font/woff\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("gz", "Content-Type: application/gzip\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("gif", "Content-Type: image/gif\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("htc", "Content-Type: text/x-component\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("htm", "Content-Type: text/html\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("html", "Content-Type: text/html; charset=utf-8\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("ico", "Content-Type: image/x-icon\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("ics", "Content-Type: text/calendar\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("iso", "Content-Type: multipart/form-data; boundary=----WebKitFormBoundarybC2GrDJYSRCSriwe\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("jad", "Content-Type: text/vnd.sun.j2me.app-descriptor\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("jar", "Content-Type: application/java-archive\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("jng", "Content-Type: image/x-jng\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("jpeg", "Content-Type: image/jpeg\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("jpg", "Content-Type: image/jpeg\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("js", "Content-Type: image/jpeg\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("json", "Content-Type: application/json\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("jsonld", "Content-Type: application/ld+json\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("mid", "Content-Type: audio/midi\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("midi", "Content-Type: audio/x-midi\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("mjs", "Content-Type: application/javascript\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("mml", "Content-Type: text/mathml\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("mp3", "Content-Type: audio/mpeg\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("mp4", "Content-Type: video/mp4\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("mpeg", "Content-Type: video/mpeg\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("mpkg", "Content-Type: application/vnd.apple.installer+xml\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("odp", "Content-Type: application/vnd.oasis.opendocument.presentation\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("ods", "Content-Type: application/vnd.oasis.opendocument.spreadsheet\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("odt", "Content-Type: application/vnd.oasis.opendocument.text\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("oga", "Content-Type: audio/ogg\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("ogv", "Content-Type: video/ogg\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("ogx", "Content-Type: application/ogg\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("opus", "Content-Type: audio/opus\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("otf", "Content-Type: font/otf\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("png", "Content-Type: image/png\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("pdf", "Content-Type: application/pdf\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("php", "Content-Type: application/x-httpd-php\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("ppt", "Content-Type: application/vnd.ms-powerpoint\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("pptx", "Content-Type: application/vnd.openxmlformats-officedocument.presentationml.presentation\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("rar", "Content-Type: application/vnd.rar\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("rss", "Content-Type: application/rss+xml\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("rtf", "Content-Type: application/rtf\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("sh", "Content-Type: application/x-sh\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("shtml", "Content-Type: text/html\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("svg", "Content-Type: image/svg+xml\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("svgz", "Content-Type: image/svg+xml\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("tar", "Content-Type: application/x-tar\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("tif", "Content-Type: image/tiff\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("tiff", "Content-Type: image/tiff\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("ts", "Content-Type: video/mp2t\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("ttf", "Content-Type: font/ttf\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("txt", "Content-Type: text/plain\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("vsd", "Content-Type: application/vnd.visio\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("wav", "Content-Type: audio/wav\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("wbmp", "Content-Type: image/vnd.wap.wbmp\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("weba", "Content-Type: audio/webm\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("webm", "Content-Type: video/webm\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("webp", "Content-Type: image/webp\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("wml", "Content-Type: text/vnd.wap.wml\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("woff", "Content-Type: font/woff\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("woff2", "Content-Type: font/woff2\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("xhtml", "Content-Type: application/xhtml+xml\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("xls", "Content-Type: application/vnd.ms-excel\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("xlsx", "Content-Type: application/vnd.openxmlformats-officedocument.spreadsheetml.sheet\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("xml", "Content-Type: text/xml\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("xul", "Content-Type: application/vnd.mozilla.xul+xml\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("zip", "Content-Type: application/zip\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("3gp", "Content-Type: video/3gpp\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("3g2", "Content-Type: video/3gpp2\r\n"));
	_contentType.insert(std::make_pair<std::string, std::string>("7z", "Content-Type: application/x-7z-compressed\r\n"));
}

std::string server_response::getType(std::string type)
{
	// std::cout << "TEST : " << type << std::endl;
	for (std::map<std::string, std::string>::iterator it = _contentType.begin(); it != _contentType.end(); it++)
		if (type == it->first)
			return (it->second);
	return ("Content-Type: application/octet-stream\r\n");
}

std::string findFileName(std::string FinalPath)
{
	size_t pos = 0;
	while (FinalPath.find("/", pos + 1) != std::string::npos)
		pos = FinalPath.find("/", pos + 1);
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

	//std::cout << "PATH : '" << path << "'" << std::endl;
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
	response << "<html><head><meta name=\"viewport\" content=\"width=device-width, minimum-scale=0.1\"><title>" << path << "</title></head><body style=\"padding:0;margin:0;\"><main style=\"margin-top:62px;padding-bottom:60px\"><h1 style=\"padding-top:0.5em;font-size:3em;\">Index of " << path << "</h1></br><ul style=\"margin-top:10px;margin-bottom:10px;padding-top:10px;padding-bottom:10px;border-size:0.5em;border-top-style:solid;border-bottom-style:solid;\">";
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
	response << "</ul></main><footer style=\"position:fixed;bottom:0;left:0;right:0;background-color:#111;color:white;text-align:center;padding:10px 0;font-size:0.8em;\">🐑 Site web créé par Nicolas, Mathieu et Cyril 🐑</footer></body></html>";
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
	bool autoindex = false;
	
	for (std::map<std::string, class server_location_configuration*>::reverse_iterator it = server->getLoc().rbegin(); it != server->getLoc().rend(); it++)
	{
		if (it->first == RequestURI.substr(0, it->first.size()))
		{
			for (std::vector<std::string>::reverse_iterator ite = it->second->getHttpMethodAccepted().rbegin(); ite != it->second->getHttpMethodAccepted().rend(); ite++)
			{
				/*	Cela permet de verifier si l'autoindex est on, pour sinon renvoyer une erreur 403 car on 
					ne peut pas lister le directory si c'est off, ds le cas où il n'y aurait pas d'index */
				if (MethodUsed == *ite)
				{
					std::cout << "listing test" << std::endl;
					if (it->second->getDirectoryListing() == "on")
						return (1);
					else if (it->second->getDirectoryListing() == "off")
						return (0);
					autoindex = true;
				}
			}
		}
	}
	if (server->getDirectoryListing() == "on" && !autoindex)
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
		infile.close();
		for (size_t i = 0; i < SessionIdGiven.size(); i++)
		{
			// std::cout << "i : " << i << std::endl;
			// std::cout << "SessionIdGiven.size() : " << SessionIdGiven.size() << std::endl;
			// std::cout << "SessionID : " << SessionID << std::endl;
			// std::cout << "SessionID[i] : " << SessionIdGiven[i] << std::endl;
			if (SessionIdGiven.size() > 0 && SessionIdGiven[i] == SessionID)
			{
				break;
			}
			else if (i == SessionIdGiven.size() - 1)
			{
				std::cout << "ID SESSION UNKNOWN" << std::endl;
				_status_code = 401;
			}
		}
	}
	return (0);
}

bool	server_response::manageCgi(const server_request& Server_Request, server_configuration *server)
{
	// std::cerr << "HELLO JE SUIS DANS LE CGI" << std::endl;
	if (access(server->getCgi().find("." + Server_Request.getType())->second.data(), X_OK))
		_status_code = 502;
	else
	{
		std::stringstream buffer;
		if (_fileName == "")
			_fileName = ".cgi-tmp";
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

int	server_response::AnswerGet(const server_request& Server_Request, server_configuration *server)
{
	if (access(_finalPath.c_str(), F_OK) && _finalPath != "./")
	{
		// std::cout << "d0 " << std::endl;
		//std::cerr << _finalPath << std::endl;
		_status_code = 404;
	}
	else
	{
		std::stringstream buffer;
		if (is_dir(_finalPath.c_str(), *this) && autoindex_is_on(Server_Request.getMethod(), server, Server_Request.getRequestURI())) // && auto index no specifie ou on --> demander a Mathieu comment gerer ce parsing dans le fichier de conf car le autoindex peut etre dans une location ou non
		{
			// std::cout << "AUTOLISTING ON" << std::endl;
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
			file.seekg(0, std::ios::end);
			int FileSize = file.tellg();
			file.seekg(0, std::ios::beg);
			if (FileSize < 500000)
				buffer << file.rdbuf();
			else 
				return FileSize;
		}
		_content = buffer.str();
	}
	return (0);
}

void	server_response::SendingResponse(const server_request& Server_Request, int conn_sock, server_configuration *server,  int StatusCodeTmp, std::map<int, std::pair<std::string, std::string> >* MsgToSent)
{
	if (StatusCodeTmp != 200)
		_status_code = StatusCodeTmp;

	
	// std::cout << "\nTEST CONFIG" << std::endl;
	// std::cout << *server << std::endl;
	// std::cout << "FIN" << std::endl; 
	/*	Ci-dessous, on genere un ID de session pour chaque nouvel utilisateur
		et on verifie que si un ID est recu, c'est bien nous qui l'avons emis
		pour renvoyer sinon une erreur 401 et un id_session a zero (a savoir 
		default d authorisation)*/
	int id_session = 0;
	if (_status_code == 200)
	{
		id_session = getIdSessionOrSetError401(Server_Request);
		// std::cout << "_status_code : " << _status_code << std::endl;
		// std::cout << "id_session : " << id_session << std::endl;
	}
	/*********************************************************************/

	
	/*Si l'on se situe, ds une location et qu'il y a une HTTP redir alors
	il faut pouvoir renvoyer la redir */
	if (isRedir(Server_Request.getMethod(), server, Server_Request.getRequestURI()) > 0 && checkStatus(_status_code))
	{
		std::stringstream response;
			response << "HTTP/1.1 301 Moved Permanently\r\nLocation: " \
			<< getRedir(Server_Request.getMethod(), server, Server_Request.getRequestURI()) << "\r\n";
			std::string response_str = response.str();
			errno = 0;
			MsgToSent->insert(std::make_pair(conn_sock, std::make_pair(response_str, "")));
			// MsgToSent->push_back(std::pair<int, std::string>(conn_sock, response_str)); // remplace sent
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
	if (1)
	{
		std::cout << "RealPath : " << RealPath << std::endl;
		std::cout << "RealPathIndex : " << RealPathIndex << std::endl;
	}
	/*Ensuite, on check si c'est le path donné est un directory ou non.
	Une fosis que l'on sait cela, on peut renvoyer un index ou 
	un message erreur */
	struct stat path_info;
	bool dir;
	if (stat(RealPath.c_str(), &path_info) != 0 && checkStatus(_status_code)) {
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
	if (server->getCgi().find("." + Server_Request.getType()) != server->getCgi().end() && checkStatus(_status_code))
	{
		//std::cerr << "cgi" << std::endl;
		manageCgi(Server_Request, server);
	}
	
	std::map<int, std::pair<std::string, std::string> > MsgToSentTmp; // Je cree un TMP pour envoyer tout d'un coup apres au vrai objet.
	MsgToSentTmp.insert(std::make_pair(conn_sock, std::make_pair("", ""))); // Ici, j'ajoute la conn_sock car on sait deja;
	int MsgSize = 0;
	// std::cout << "\n TEST : " << Server_Request.getMethod() << std::endl;
	std::stringstream response;
	if ((Server_Request.getMethod() == "GET" || Server_Request.getMethod() == "POST") && checkStatus(_status_code))
	{
		// std::cout << "\nGETMETHOD SERVER_RESPONSE\n" << std::endl;
		if (_status_code == 200)
		{
			MsgSize = AnswerGet(Server_Request, server);
			if (MsgSize)	
				MsgToSentTmp[conn_sock].second = _finalPath;
		}
		createResponse(server, _content, Server_Request, id_session, MsgSize);
		MsgToSentTmp[conn_sock].first = _ServerResponse;
		std::map<int, std::pair<std::string, std::string> >::iterator it = MsgToSentTmp.find(conn_sock);
		if (it != MsgToSentTmp.end())
			MsgToSent->insert(*it);
		return ;
	}
	else if (Server_Request.getMethod() == "DELETE" && checkStatus(_status_code))
	{
		if (_status_code == 200)
			this->delete_dir(_finalPath.c_str());
		if (_status_code == 200)
			_content = server->getErrorPage()[STATUS200].second;
		createResponse(server, _content, Server_Request, id_session, 0);
		MsgToSent->insert(std::make_pair(conn_sock, std::make_pair(_ServerResponse, "")));
		return ;
	}
	else
	{
		createResponse(server, "", Server_Request, id_session, 0);
	//	std::cerr << "ServerResponse = '" << _ServerResponse << "'" << std::endl;
		MsgToSent->insert(std::make_pair(conn_sock, std::make_pair(_ServerResponse, "")));
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
	{
		if (checkStatus(_status_code))
			response << this->getType(Server_Request.getType());
		else
			response << this->getType("html");
	}
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

void	server_response::createResponse(server_configuration * server, std::string file, const server_request& Server_Request, int IdSession, int MsgSize)
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
					if (MsgSize == 0)
					{
						response << addBody(file);
						break;
					}
					else
					{
						response << "Content-Length: " << MsgSize << "\r\n\r\n";
						_ServerResponse = response.str();
						return ;
					}
					
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
	_env.push_back("SERVER_PROTOCOL=" + _req->getVersion());
	_env.push_back("SERVER_PORT=" + itos(server->getPort()[0]));
	std::string	cwd = getcwd(buff, 256);
	_env.push_back("DOCUMENT_ROOT=" + cwd);
	_env.push_back("REQUEST_METHOD=" + _req->getMethod());
	_env.push_back("SCRIPT_FILENAME=" + cwd + toexec.substr(1));
	cgiPath = server->getCgi().find("." + _req->getType())->second;
	_env.push_back("SCRIPT_NAME=" + toexec.substr(1));
	_env.push_back("QUERY_STRING=" + _req->getQuery());
	_env.push_back("PATH_INFO=/");
	_env.push_back("REQUEST_URI=" + _req->getRequestURI());
	_env.push_back("REDIRECT_STATUS=1");
	if (_body.find(std::string("content-length")) != std::string::npos)
		_env.push_back(std::string("CONTENT_LENGTH=") + itos(_contentLength));
	// std::cerr << "CONTENT_TYPE = '" << this->getType(_req->getType()).substr(14, 500) << "'" << std::endl;
	if (this->getType(_req->getType()) != "")
		_env.push_back(std::string("CONTENT_TYPE=") + this->getType(_req->getType()).substr(14, 500));
	//_env.push_back("CONTENT_TYPE=application/x-www-form-urlencoded");
	// std::cerr << "_body = '" << _body << "'" << std::endl;
	if (_req->getIsBody())
	{
		// std::cerr << "PPPPPP" << std::endl;
		std::ofstream file(getBodyName().c_str());
		if (file) {
			file << _req->getBody();
			file.close();
		}
		//ecrire ce qu'il y a dans _body, dans _bodyName ?
		//_cgiFd = open de _bodyName;
		//--> check dans cgi si on ferme bien le fd
		//supprimer _bodyName;
		_cgiFd = open(getBodyName().data(), O_RDONLY);
		if (_cgiFd < 0)
		{
			//std::cerr << "FAIL TO OPEN CGIFD" << std::endl;
			_status_code = 500;
			return (1);
		}
		std::remove(getBodyName().data());
		// std::cerr << "cgi fd = '" << _cgiFd << "'" << "_req->getBody().size() = " << _req->getBody().size() << std::endl;
	}
	try
	{
		int status = 0;
		Cgi cgi(cgiPath, toexec, _env, _cgiFd, _fileName, _req->getQuery().c_str());
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
