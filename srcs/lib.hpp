/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lib.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leng-chu <-chu@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 12:53:24 by leng-chu          #+#    #+#             */
/*   Updated: 2023/03/24 19:49:06 by leng-chu         ###   ########.fr       */
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

using namespace std;

#endif
