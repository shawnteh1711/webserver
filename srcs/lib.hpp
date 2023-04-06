/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lib.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: steh <steh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 12:53:24 by leng-chu          #+#    #+#             */
/*   Updated: 2023/04/06 12:37:12 by leng-chu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// this header contains all libraries and using std. easy to track.
// our files only need to include this file once enough.

#ifndef LIB_HPP
# define LIB_HPP

#include <iostream>
//#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h> // for read & write
#include <sstream> // for ostringstream
#include <cstring>
#include <poll.h> // for poll()
#include <csignal>
#include <vector>
#include <fstream>
#include <utility>
#include <sys/stat.h>
//#include <netdb.h> // for addrinfo struct
#include <map>
#include <fcntl.h>
#include <dirent.h> //  for dirent struct autoindex
#include <filesystem> // check if file is existing

#define RESET   "\033[0m"
#define RED		"\033[31m"
#define GREEN	"\033[32m"
#define YELLOW	"\033[33m"
#define BLUE	"\033[34m"
#define MAGENTA	"\033[35m"
#define CYAN	"\033[36m"
#define WHITE	"\033[37m"

using namespace std;

#define PWD getenv("PWD")

#endif
