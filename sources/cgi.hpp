/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nflan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 15:47:06 by nflan             #+#    #+#             */
/*   Updated: 2023/04/25 19:11:23 by nflan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
#define CGI_HPP

#include "lib.hpp"

class server_response;
class server_request;

class Cgi
{
	private:
		int				_input_fd;
		int				_output_fd;
		int				_status;
		pid_t			_pid;
		FILE*			_fp;
		char**			_cmd;
		char**			_envp;
		std::string		_fileName;
		server_request*	_request;

		Cgi();

	public:
		Cgi(std::string& cgi_path, std::string& file_path, std::vector<std::string> & env, int input_fd, std::string filen, const char* pythonArg);
		Cgi(const Cgi & o);
		~Cgi();
		Cgi & operator=(const Cgi & o);

	// GETTERS
		pid_t	getPid() const { return (_pid); }
		int	getInputFd() const { return (_input_fd); }
		char**	getCmd() const { return (_cmd); }
		char**	getEnvp() const { return (_envp); }
		int	getStatus() const { return (_status); }
		char*	getPythonArg() const { return (_cmd[2]); }
		server_request*	getRequest() const { return (_request); }

	// OTHER
		void	print() const;
		void	setStatus(int); // setter
		void	setPid();
		void	setPid(pid_t); // setter
		void	dupping();
		void	closePdes();
		void	del();

	// EXCEPTIONS
		class PipeException: public std::exception {
			public:
				virtual const char *	what() const throw() { return ("Pipe Error!"); }
		};
		class ForkException: public std::exception {
			public:
				virtual const char *	what() const throw() { return ("Fork Error!"); }
		};
		class DupException: public std::exception {
			public:
				virtual const char *	what() const throw() { return ("Dup Error!"); }
		};
		class OpenException: public std::exception {
			public:
				virtual const char *	what() const throw() { return ("Open Error!"); }
		};
};


#endif
