/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chillion <chillion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 15:47:23 by nflan             #+#    #+#             */
/*   Updated: 2023/04/25 14:45:47 by chillion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi.hpp"
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>

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
	std::string filename("./cgi-tmp.txt");

	FILE* fp = fopen(filename.c_str(), "w");
	if (fp == NULL) {
		std::cerr << "Error opening file: " << std::strerror(errno) << std::endl;
		return ((void) 1);
	}

	int fd = fileno(fp); // get file descriptor from file pointer

	if (dup2(fd, STDOUT_FILENO) == -1) {
		std::cerr << "Error redirecting output: " << std::strerror(errno) << std::endl;
		return ((void) 1);
	}
// if (_input_fd != -1)
// {
// 	if (dup2(_input_fd, STDIN_FILENO) == -1)
// 		exit (1);
// 	close (_input_fd);
// }
//	if (dup2(_pdes[1], STDOUT_FILENO) == -1) // uncom quand good
//		exit (1);
	close (fd);
	close (_pdes[0]);
	close (_pdes[1]);
}

int	parse_answer_file()
{
	std::string filename("./cgi-tmp.txt");
	std::ifstream ifs(filename.c_str());
	std::string line;
	std::string body;
	std::string header;
	int status = 0;
	int i = 0;

	if (ifs.is_open())
	{
		while (std::getline(ifs, line))
		{
			if (line == "\r")
				break;
			if (i == 0)
				status = std::atoi(line.c_str() + 7);
			header += line;
			i++;
		}
		while (std::getline(ifs, line))
		{
			body += line;
		}
		ifs.close();
	}
	else
		std::fprintf(stderr, "Error: CGI data file invalide: %s\n", strerror(errno));
	return (status);
}

int	check_access(std::string exec)
{
    int fd = 1;

	if (exec.c_str()[0] != '.' || exec.c_str()[0] != '/'){
		std::fprintf(stderr, "Error: %s: Can't be execute\n", exec.c_str());
        return 1;
    }
	fd = access(exec.c_str(), X_OK);
	if (fd == 0)
		return 0;

    if (fd == -1) {
		std::fprintf(stderr, "Error: %s: %s\n", exec.c_str(), strerror(errno));
        return 1;
    }
    return 2;
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
	if (WIFEXITED(status))
		status = WEXITSTATUS(status);
	std::cerr << "EXIT STATUS = " << status << std::endl;
}

int check_cgi_type(std::map<std::string, std::string>& _Cgi, std::string const &Type, Cgi & cgi)
{
	for (std::map<std::string, std::string>::iterator it = _Cgi.begin(); it != _Cgi.end(); it++)
	{
		if (it->first == Type)
		{
			if (check_access(it->second) != 0)
				return 1;
			exeCgi(cgi);
		}
	}
	return 0;
}

std::string	cgi_type(std::string const &type)
{
	enum	status {PHP, PYTHON, AUTRE};

	const std::string ftab[3] = {"php", "py", "others"};
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
			std::cout << "JE SUIS DANS CGI PHP" << std::endl;
			return ("/usr/bin/php-cgi");
			break;
		}
		case PYTHON:
		{
			std::cout << "JE SUIS DANS CGI PYTHON" << std::endl;
			return ("/usr/bin/python3");
			break;
		}
		case AUTRE:
		{
			std::cout << "JE SUIS DANS CGI AUTRE" << std::endl;
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
	std::string a = cgi_type("php");
	std::string cmd(av[1]);
	try {
		//ajout d'une commande conditionnee pour voir si on fait un cgi ou pas (si le cgi en question est precise dans le fichier de configue. Sinon, on renvoie la reponse en dur, sans traitement)
		Cgi	cgi(a, cmd, env, -1);
		exeCgi(cgi);
		cgi.closePdes();
		parse_answer_file();
	}
	catch ( std::exception& e )
	{
		return (std::cerr << e.what() << std::endl, 1);
	}
	return (0);
}
