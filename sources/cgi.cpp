/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chillion <chillion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 15:47:23 by nflan             #+#    #+#             */
/*   Updated: 2023/04/25 12:28:02 by chillion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi.hpp"
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>

Cgi::Cgi(std::string & cgi_path, std::string & file_path, std::vector<std::string> & env, int input_fd, std::string filen, server_request* request): _status(0), _request(request)
{
	_cmd = new char*[3];
	_cmd[0] = &(cgi_path[0]);
	_cmd[1] = &(file_path[0]);
	_cmd[2] = NULL;
	_envp = new char* [env.size() + 1];
	size_t	i = 0;
	for (std::vector<std::string>::iterator it = env.begin(); it != env.end(); it++, i++)
		_envp[i] = &((*it)[0]);
	_envp[env.size()] = NULL;
	_input_fd = input_fd;
	_output_fd = -1;
	_pid = -1;
	_fileName = filen;
	_fp = NULL;
}

Cgi::~Cgi()
{
	delete [] _cmd;
	delete [] _envp;
}

Cgi::Cgi(const Cgi & o)
{
	*this = o;
}

Cgi	&Cgi::operator=(Cgi const &o)
{
	if (this == &o)
		return (*this);
	_pdes[0] = o.getPdes()[0];
	_pdes[1] = o.getPdes()[1];
	_pid = o.getPid();
	_input_fd = o.getInputFd();
	_cmd = new char*[3];
	_cmd[0] = o.getCmd()[0];
	_cmd[1] = o.getCmd()[1];
	_cmd[2] = NULL;
	size_t	i = 0;
	for (; o.getEnvp()[i]; i++) {}
	_envp = new char* [i + 1];
	for (i = 0; o.getEnvp()[i]; i++)
		_envp[i] = o.getEnvp()[i];
	_envp[i + 1] = NULL;
	_status = o.getStatus();
	return (*this);
}

const int*	Cgi::getPdes() const { return (_pdes); }
pid_t	Cgi::getPid() const { return (_pid); }
int	Cgi::getInputFd() const { return (_input_fd); }
char**	Cgi::getCmd() const { return (_cmd); }
char**	Cgi::getEnvp() const { return (_envp); }
int	Cgi::getStatus() const { return (_status); }
server_request*	Cgi::getRequest() const { return (_request); }

void	Cgi::setPid()
{
	_pid = fork();
	if (static_cast<int>(_pid) == -1)
		throw ForkException();
	else if (static_cast<int>(_pid) == 0)
		setPdes();
}

void	Cgi::setPdes()
{
	if (pipe(_pdes) == -1)
		exit (2);
	dupping();
}

void	Cgi::closePdes()
{
	if (_pdes[0] > 2)
		close(_pdes[0]);
	if (_pdes[1] > 2)
		close(_pdes[1]);
}

void	Cgi::print() const
{
	size_t	i = 0;
	for (; _cmd[i]; i++)
		printf("cmd = '%s'\n", _cmd[i]);
	for (i = 0; _envp[i]; i++)
		printf("envp = '%s'\n", _envp[i]);
}

void	Cgi::setStatus(int s)
{
	_status = s;
}

const char *	DupException::what() const throw()
{
	return ("Dup Error!");
}

const char *	OpenException::what() const throw()
{
	return ("Open Error!");
}

void	Cgi::dupping()
{
	std::cout << "filename in cgi = '" << _fileName << "'" << std::endl;
	std::string filename(_fileName.c_str());

	_fp = fopen(filename.c_str(), "w");
	if (_fp == NULL)
		throw OpenException();

	_output_fd = fileno(_fp); // get file descriptor from file pointer

	if (dup2(_output_fd, STDOUT_FILENO) == -1)
		throw DupException();

	close (_pdes[1]);
	close (_pdes[0]);
	exeCgi();
}

const char *	PipeException::what() const throw()
{
	return ("Pipe Error!");
}

const char *	ForkException::what() const throw()
{
	return ("Fork Error!");
}

const char *	ExecveException::what() const throw()
{
	return ("Execve Error!");
}

void DeleteServers(std::vector<server_configuration*> servers);
void PrintServer(std::vector<server_configuration*> servers);

void	Cgi::exeCgi()
{
	std::cerr << "oscour " << _request->getAllServers().size() << std::endl;
	if (execve(_cmd[0], _cmd, _envp) == -1)
	{
		DeleteServers(_request->getAllServers());
		closePdes();
		if (_output_fd != -1)
			close(_output_fd);
		delete(_request);
		fclose(_fp);
		exit (1);
	}
}

std::string	cgi_type(std::string const &type)
{
	enum	status { PHP, PYTHON, AUTRE};

	const std::string ftab[3] = {"php", ".py", "others"};
	// const std::string ftab[3] = {"/usr/bin/php-cgi", "/usr/bin/python3", "/usr/bin/autre"};
	int n = 0;
	for (; n < 4; n++)
	{
		if (n != 3 && ftab[n] == type) // OK 
		{
			break ;
		}
	}
	switch (n)
	{
		case PHP:
		{
			// std::cout << "JE SUIS DANS CGI PHP" << std::endl;
			return ("/usr/bin/php-cgi");
			break;
		}
		case PYTHON:
		{
			// std::cout << "JE SUIS DANS CGI PYTHON" << std::endl;
			return ("/usr/bin/python3");
			break;
		}
		case AUTRE:
		{
			// std::cout << "JE SUIS DANS CGI AUTRE" << std::endl;
			return ("/usr/bin/autre");
			break;
		}
		default :
		{
			return ("NONE");
			break ;
		}
	}
}

/*int	main(int ac, char **av, char **envp)
{
	if (ac != 2)
		return (1);
	std::vector<std::string>	env;
	for (size_t i = 0; envp[i]; i++)
		env.push_back(envp[i]);
	std::string a = cgi_type("php");
	std::string cmd(av[1]);
	try {
		//ajout d'une commande conditionnee pour voir si on fait un cgi ou pas (si le cgi en question est precise dans le fichier de configue. Sinon, on renvoie la reponse en dur, sans traitement)
		Cgi	cgi(a, cmd, env, -1);
		exeCgi(cgi);
		cgi.closePdes();
	}
	catch ( std::exception& e )
	{
		return (std::cerr << e.what() << std::endl, 1);
	}
	return (0);
}*/
