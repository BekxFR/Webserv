/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lib.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgruson <mgruson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 18:38:10 by nflan             #+#    #+#             */
/*   Updated: 2023/04/28 17:17:55 by mgruson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIB_HPP
#define LIB_HPP

#define READ_SIZE 1000
#define MAX_EVENTS 10
#define DEBUG 0
#define DEBUG1 0

#include <algorithm>
#include <arpa/inet.h>
#include <climits>
#include <cstdio>
#include <cstdlib>
#include <csignal>
#include <cstring>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <filesystem>
#include <fstream>
#include <ftw.h>
#include <iostream>
#include <iterator>
#include <map>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <sstream>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <utility>
#include <vector>
#include <pthread.h>


#include "cgi.hpp"
#include "default_error.hpp"
#include "ErrorCorresp.hpp"
#include "server_configuration.hpp"
#include "server_location_configuration.hpp"
#include "server_request.hpp"
#include "server_response.hpp"
#include "utils.hpp"
#include "initServ.hpp"

#endif
