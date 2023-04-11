/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 15:47:23 by nflan             #+#    #+#             */
/*   Updated: 2023/04/11 12:39:13 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi.hpp"
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <fstream>

Cgi::Cgi(std::string & cgi_path, std::string & file_path, std::vector<std::string> & env, int input_fd)
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
	_pid = -1;
	setPdes();
	setPid();
}

Cgi::~Cgi()
{
	delete [] _cmd;
	delete [] _envp;
}

const int*	Cgi::getPdes() const
{
	return (_pdes);
}

pid_t	Cgi::getPid() const
{
	return (_pid);
}

char**	Cgi::getCmd() const
{
	return (_cmd);
}

char**	Cgi::getEnvp() const
{
	return (_envp);
}

void	Cgi::setPid()
{
	_pid = fork();
	if (static_cast<int>(_pid) == -1)
		throw ForkException();
}

void	Cgi::setPdes()
{
	if (pipe(_pdes) == -1)
		throw PipeException();
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

void	Cgi::dupping()
{
	if (_input_fd != -1)
	{
		if (dup2(_input_fd, STDIN_FILENO) == -1)
			exit (1);
		close (_input_fd);
	}
//	if (dup2(_pdes[1], STDOUT_FILENO) == -1) // uncom quand good
//		exit (1);
	close (_pdes[1]);
	close (_pdes[0]);
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

void	exeCgi(Cgi & cgi)
{
	int	status;
	if (cgi.getPid() == 0)
	{
		cgi.dupping();
		if (execve((cgi.getCmd())[0], cgi.getCmd(), cgi.getEnvp()))
			throw ExecveException();
	}
	waitpid(cgi.getPid(), &status, 0);
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

int	main(int ac, char **av, char **envp)
{
	if (ac != 2)
		return (1);
	std::vector<std::string>	env;
	for (size_t i = 0; envp[i]; i++)
		env.push_back(envp[i]);
	std::string a("/usr/bin/php-cgi");
	std::string cmd(av[1]);
	try {
		//ajout d'une commande conditionnee pour voir si on fait un cgi ou pas (si le cgi en question est precise dans le fichier de configue. Sinon, on renvoie la reponse en dur, sans traitement)
		Cgi	cgi(a, cmd, env, -1);
		exeCgi(cgi);
	}
	catch ( std::exception& e )
	{
		return (std::cerr << e.what() << std::endl, 1);
	}
	return (0);
}
