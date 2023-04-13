/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lib.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: steh <steh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 12:53:24 by leng-chu          #+#    #+#             */
/*   Updated: 2023/04/13 20:37:35 by leng-chu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
#include <filesystem>
#include <sstream> //isstringstream

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

template<typename T, typename U>
void printMap(map<T, U> & pmap)
{
    cout << YELLOW << "map size: " << pmap.size() << RESET << endl;
    typename std::map<T, U>::iterator it = pmap.begin();
    while (it != pmap.end())
    {
        cout << YELLOW << it->first << " : " << it->second << RESET << endl;
        cout << "first size" << it->first.length() << endl;
        it++;
    }
}

template<typename T, typename U>
void printMap(multimap<T, U> & pmap)
{
    cout << YELLOW << "map size: " << pmap.size() << RESET << endl;
    typename std::multimap<T, U>::iterator it = pmap.begin();
    while (it != pmap.end())
    {
        cout << YELLOW << it->first << " : " << it->second << RESET << endl;
        it++;
    }
}

#endif
