/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chillion <chillion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 15:47:23 by nflan             #+#    #+#             */
/*   Updated: 2023/05/09 15:12:14 by chillion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi.hpp"

extern volatile std::sig_atomic_t	g_code;

Cgi::Cgi(std::string & cgi_path, std::string & file_path, std::vector<std::string> & env, int input_fd, std::string filen, const char* pythonArg): _input_fd(input_fd), _status(0) 
{
	_cmd = NULL;
	_envp = NULL;
	_pid = -1;
	_fileName = filen;
	_output_fd = -1;
	_fp = NULL;
	try {
		_cmd = new char*[4];
		_cmd[0] = &(cgi_path[0]);
		_cmd[1] = &(file_path[0]);
		_cmd[2] = (char *)pythonArg;
		_cmd[3] = NULL;
		_envp = new char* [env.size() + 1];	
		size_t	i = 0;
		for (std::vector<std::string>::iterator it = env.begin(); it != env.end(); it++, i++)
			_envp[i] = &((*it)[0]);
		_envp[env.size()] = NULL;
	}
	catch (std::exception& e)
	{
		del();
		throw std::bad_alloc();
	}
	setPid();
}

Cgi::~Cgi()
{
	del();
}

Cgi::Cgi(const Cgi & o)
{
	*this = o;
}

Cgi	&Cgi::operator=(Cgi const &o)
{
	if (this == &o)
		return (*this);
	try {
		_pid = o.getPid();
		_status = o.getStatus();
		_input_fd = o.getInputFd();
		_cmd = new char*[3];
		_cmd[0] = o.getCmd()[0];
		_cmd[1] = o.getCmd()[1];
		_cmd[2] = o.getCmd()[2];
		_cmd[3] = NULL;
		size_t	i = 0;
		for (; o.getEnvp()[i]; i++) {}
		_envp = new char* [i + 1];
		for (i = 0; o.getEnvp()[i]; i++)
			_envp[i] = o.getEnvp()[i];
		_envp[i + 1] = NULL;
	}
	catch (std::exception& e)
	{
		del();
		throw std::bad_alloc();
	}
	return (*this);
}

void	Cgi::setStatus(int s)
{
	_status = s;
}

void	Cgi::del()
{
	if (_cmd)
		delete [] _cmd;
	if (_envp)
		delete [] _envp;
	if (_fp)
		fclose(_fp);
	if (_output_fd != -1)
		close(_output_fd);
}

void	Cgi::setPid()
{
	_pid = fork();
	if (static_cast<int>(_pid) == -1)
	{
		del();
		throw ForkException();
	}
	else if (static_cast<int>(_pid) == 0)
		dupping();
	else
		if (_input_fd != -1)
			close (_input_fd);
	return ;
}

void	Cgi::setPid(pid_t p)
{
	_pid = p;
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
	//	std::cout << "filename in cgi = '" << _fileName << "'" << std::endl;
	_fp = fopen(_fileName.c_str(), "w"); // on essaie d'ouvrir le fichier de sortie
	if (_fp == NULL)
	{
		del();
		g_code = 1;
		return ;
	}
	_output_fd = fileno(_fp); // get file descriptor from file pointer
	// std::cerr << "VIEW VIEW" << _input_fd << std::endl;
	if (_input_fd != -1) // si un fichier d'entre est specifie sinon stdin
	{
		if (dup2(_input_fd, STDIN_FILENO) == -1)
		{
			del();
			g_code = 1;
			return ;
		}
		close (_input_fd);
	}
	if (dup2(_output_fd, STDOUT_FILENO) == -1) // attribution de la sortie au fichier ouvert precedemment
	{
		del();
		g_code = 1;
		return ;
	}
	// std::cerr << "VIEW VIEW 24 42" << std::endl;
	//	close (_pdes[1]);
	//	_pdes[1] = -1;
	//	close (_pdes[0]);
	//	_pdes[0] = -1;
	std::cerr << " _cmd[0] = " << _cmd[0] << " _cmd[1] = " << _cmd[1] << " _cmd[2] = " << _cmd[2] << " _cmd[3] = " << _cmd[3] << std::endl;
	if (std::strcmp(_cmd[2], ""))
	{
		if (execve(_cmd[0], _cmd, NULL) == -1)
			g_code = 1;
	}
	else if (execve(_cmd[0], _cmd, _envp) == -1) // si execve rate, on laisse passer pour appeler les destructeurs mais en changeant le code global pour sortir une 500
		g_code = 1;
}
