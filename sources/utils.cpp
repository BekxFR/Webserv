/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chillion <chillion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 15:20:43 by nflan             #+#    #+#             */
/*   Updated: 2023/05/04 17:33:19 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

extern std::vector<int> open_ports;

std::string	itos(int nb)
{
	std::stringstream	convert;

	convert << nb;
	return (convert.str());
}

std::string	readingFileEP( std::string file )
{
	struct stat sb;
	std::ifstream input_file(file.c_str());

	if (stat(file.c_str(), &sb) == -1 || S_ISDIR(sb.st_mode) || access(file.c_str(), R_OK) || !input_file.is_open())
	{
		if (stat(file.c_str(), &sb) != -1 && S_ISDIR(sb.st_mode))
			std::cerr << "Can't use a directory as Error Page (" << file << ")" << std::endl;
		else
			perror("Error when trying to open the file (access) -> using default error page");
		return ("");
	}
	std::stringstream buffer;
	std::string fileContent;

	buffer << input_file.rdbuf();
	fileContent = buffer.str();
	input_file.close();
	return (fileContent.substr(0, fileContent.size()));
}

void PrintServer(std::vector<server_configuration*> servers)
{
	for (size_t i = 0; i < servers.size(); i++)
	{
		server_configuration* server = servers[i];
		std::cout << "Server " << i << ":" << std::endl;
		std::cout << *server << std::endl;
	}
}

void	CloseSockets(int *listen_sock, int epollfd, sockaddr_in *addr, std::vector<int> Ports)
{
	(void)addr;
	(void)epollfd;
	(void)Ports;
	for (size_t i = 0; i < Ports.size(); i++)
	{
		if (listen_sock[i] != -1)
			close(listen_sock[i]);
	}
//	for (int i = 0; i < tablen; i++)
//	{
//		close(listen_sock[i]);
//		if (Ports[i] != -1)
//			close(Ports[i]);
//		close(addr[i].sin_port);
//	}
//	if (epollfd != -1)
//		close(epollfd);
}

void	DeleteServers(std::vector<server_configuration*> servers)
{
	for (size_t i = 0; i < servers.size(); i++)
	{
		for (std::map<std::string, class server_location_configuration*>::iterator it = servers[i]->getLoc().begin(); it != servers[i]->getLoc().end(); it++)
			delete it->second;
		delete servers[i];
	}
}

bool	checkStatus(int status)
{
	if (status >= 200 && status <= 206)
		return (1);
	return (0);
}

bool	isMethodPossible(std::string method)
{
	if (method == "POST" || method == "GET" || method == "DELETE")
		return (1);
	return (0);
}

bool	is_dir(const char* path, server_response& sr)
{
	struct stat tab;

	if (stat(path, &tab) == -1)
		return (sr.setStatusCode(500), 1);
	return (S_ISDIR(tab.st_mode));
}

std::string	prev_link(std::string path)
{
	unsigned int	i = 0;
	for (; path.find("/") != std::string::npos; i++){} 

	return (path);
}

bool	isGenerallyAuthorised(std::string MethodUsed, server_configuration *server, std::string ite)
{
	if (ite == "NOT INDICATED")
	{
		std::vector<std::string>	tmp = server->getHttpMethodAccepted();
		for (std::vector<std::string>::iterator ite2 = tmp.begin(); ite2 != tmp.end(); ite2++)
			{
				if (MethodUsed == *ite2)
				{
					return (1);
				}
			}
	}
	return (0);
}
