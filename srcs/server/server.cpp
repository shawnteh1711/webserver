/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: steh <steh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/11 17:43:00 by steh              #+#    #+#             */
/*   Updated: 2023/03/12 11:06:38 by steh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

server::server()
{
    cout << "server default constructor" << endl;
}

server::server(ifstream& config_file) : _config(config_file)
{
    cout << "server config constructor" << endl;
}

server::server(const server& other)
{
    if (this != &other)
        this->_config = other._config;
}

server::~server()
{
    return ;
}

server& server::operator=(const server& other)
{
    if (this != &other)
        this->_config = other._config;
    return (*this);
}

void    server::setup(void)
{
    // for (size_t i = 0; i < _config.get_server_count(); ++i)
    // {
    //     // Create a socket object
    //     m_socket socket(_config.get_domain(), _config.get_service(), _config.get_protocol(), _config.get_port(), _config.get_interface());

    //     // Bind the socket to an address
    //     socket.bind_socket(socket.get_fd(), socket.get_address());

    //     // Listen to incoming connections
    //     socket.listen();
        
    //     // Add the socket object to the vector of sockets
    //     _sockets.push_back(socket);
    // }
}
