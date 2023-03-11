/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: steh <steh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/11 20:05:53 by steh              #+#    #+#             */
/*   Updated: 2023/03/11 21:09:54 by steh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

m_socket::m_socket() : _socket_fd(-1), _connection(-1)
{
    memset(&_address, 0, sizeof(_address));
    _address.sin_family = AF_INET;
    _address.sin_addr.s_addr = INADDR_ANY;
    _address.sin_port = htons(0);
    _socket_fd = create_socket(AF_INET, SOCK_STREAM, 0);
    test_create_socket(_socket_fd);
    bind_socket(_socket_fd, get_address());
    return ;
}

m_socket::m_socket(int domain, int service, int protocol, int port, u_long interface)
{
    memset(&_address, 0, sizeof(_address));
    _address.sin_family = domain;
    _address.sin_addr.s_addr = htonl(interface);
    _address.sin_port = htons(port);
    _socket_fd = create_socket(domain, service, protocol);
    test_create_socket(_socket_fd);
    return ;
}

m_socket::m_socket(const m_socket& other)
{
    if (this != &other)
    {
        _address = other._address;
        _socket_fd = other._socket_fd;
        _connection = other._connection;
    }
}

m_socket::~m_socket()
{
    cout << "socket destructor" << endl;
}

m_socket&   m_socket::operator=(const m_socket& other)
{
    if (this != &other)
    {
        _address = other._address;
        _socket_fd = other._socket_fd;
        _connection = other._connection;
    }
    return (*this);
}

int     m_socket::create_socket(int domain, int type, int protocol)
{
    return(socket(domain, type, protocol));
}

int     m_socket::bind_socket(int sockfd, struct sockaddr_in address)
{
    return(bind(sockfd, (struct sockaddr*)&address, sizeof(address)));
}

void    m_socket::test_create_socket(int socket_fd)
{
    if (socket_fd == -1)
        std::cout << "Socket creation error" << std::endl;
    else
        std::cout << "Socket creation success" << std::endl;
}

struct sockaddr_in m_socket::get_address() const
{
    return (_address);
}
