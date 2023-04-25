/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 15:47:06 by nflan             #+#    #+#             */
/*   Updated: 2023/04/25 12:51:35 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
#define CGI_HPP

#include "server_request.hpp"
#include "server_response.hpp"
#include <string>
#include <cstring>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <iostream>

class server_response;

class Cgi
{
	public:
		Cgi(std::string& cgi_path, std::string& file_path, std::vector<std::string> & env, int input_fd, std::string);
		Cgi(const Cgi & o);
		~Cgi();

		Cgi & operator=(const Cgi & o);
		void	print() const;
		const int*	getPdes() const;
		char**	getCmd() const;
		char**	getEnvp() const;
		pid_t	getPid() const;
		int	getInputFd() const;
		int		getStatus() const;
		server_request*	getRequest() const;
		server_request*	getResponse() const;
		void	setStatus(int);
		void	setPid();
		void	setPdes();
		void	dupping();
		void	closePdes();
		void	exeCgi();

		void	del();

		class PipeException: public std::exception {
			public:
				virtual const char *	what() const throw();
		};
		class ExecveException: public std::exception {
			public:
				virtual const char *	what() const throw();
		};
		class ForkException: public std::exception {
			public:
				virtual const char *	what() const throw();
		};
		class DupException: public std::exception {
			public:
				virtual const char *	what() const throw();
		};
		class OpenException: public std::exception {
			public:
				virtual const char *	what() const throw();
		};

	private:
		char**	_cmd;
		char**	_envp;
		std::string	_fileName;
		pid_t	_pid;
		int		_input_fd;
		int		_output_fd;
		FILE*	_fp;
		int		_pdes[2];
		int		_status;
		server_request*	_request;
		server_response*	_response;

		Cgi();
};


#endif
