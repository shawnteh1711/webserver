/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lib.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leng-chu <-chu@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 12:53:24 by leng-chu          #+#    #+#             */
/*   Updated: 2023/03/25 14:27:55 by leng-chu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// this header contains all libraries and using std. easy to track.
// our files only need to include this file once enough.

#ifndef LIB_HPP
# define LIB_HPP

#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h> // for read & write
#include <sstream> // for ostringstream
#include <stdlib.h>
#include <string>
#include <cstring>
#include <poll.h> // for poll()
#include <csignal>
#include <vector>
#include <fstream>
#include <utility>
#include <netdb.h> // for addrinfo struct
#include <map>
#include <pthread.h> // requires to handle multi servers

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"

using namespace std;

#endif
