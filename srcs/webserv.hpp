/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: steh <steh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/11 17:41:34 by steh              #+#    #+#             */
/*   Updated: 2023/03/19 21:20:54 by steh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEB_SERVER_HPP
# define WEB_SERVER_HPP

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"      
#define BLUE    "\033[34m"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <vector>
#include <set>

using namespace std;
#define buffer_size 300000

#include "config.hpp"
#include "socket.hpp"
#include "server.hpp"
#include "logger.hpp"


#endif
