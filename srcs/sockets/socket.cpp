/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: steh <steh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/11 20:05:53 by steh              #+#    #+#             */
/*   Updated: 2023/03/12 15:49:45 by steh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

m_socket::m_socket() : _socket_fd(-1), _connection(-1), _log(SOMAXCONN), _yes(1)
{
    cout << "socker default constructor" << endl;
    memset(&_address, 0, sizeof(_address));
    _address.sin_family = AF_INET;
    _address.sin_addr.s_addr = INADDR_ANY;
    _address.sin_port = htons(80);
    _socket_fd = create_socket(AF_INET, SOCK_STREAM, 0);
    test_create_socket();
    set_socket_opt();
    _connection = bind_socket();
    test_bind_socket();
    listen_socket();
    launch();
    return ;
}

/**
 * maybe need to parse only can assign into this constructor
*/
m_socket::m_socket(int domain, int service, int protocol, int port, u_long interface, int log)
{
    cout << "socket parameter constructor" << endl;
    memset(&_address, 0, sizeof(_address));
    _address.sin_family = domain;
    _address.sin_addr.s_addr = htonl(interface);
    _address.sin_port = htons(port);
    _socket_fd = create_socket(domain, service, protocol);
    _log = log;
    _yes = 1;
    if (log == 0)
        _log = SOMAXCONN;
    test_create_socket();
    set_socket_opt();
    _connection = bind_socket();
    test_bind_socket();
    listen_socket();
    launch();
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

void    m_socket::test_create_socket()
{
    if (_socket_fd == -1)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }
    else
        std::cout << "Socket creation success" << std::endl;
}


void    m_socket::set_socket_opt()
{
    if (setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR, &_yes, sizeof(_yes)) == -1)
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    else
        cout << "Setsockopt success" << endl;
}

// int     m_socket::bind_socket(int sockfd, struct sockaddr_in address)
// {
//     return(bind(sockfd, (struct sockaddr*)&address, sizeof(address)));
// }

int     m_socket::bind_socket()
{
    return(bind(_socket_fd, (struct sockaddr*)&_address, sizeof(_address)));
}

void    m_socket::test_bind_socket()
{
     if (_connection == -1)
     {
        perror("In bind");
        exit(EXIT_FAILURE);
     }
    else
        std::cout << "Socket bind success" << std::endl;
}

int    m_socket::listen_socket()
{
    // Start listening for incoming connections
    if (listen(_socket_fd, _log) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }
    else
    {
        cout << "Listening" << endl;
        return (0);
    }
}

void    m_socket::accepter()
{
    int     addrlen;

    addrlen = sizeof(_address);
    _accept_socket_fd = accept(_socket_fd, (struct sockaddr *)&_address, (socklen_t *)&addrlen);
    if (_accept_socket_fd < 0)
    {
        perror("In accept");
        exit(EXIT_FAILURE);
    }
    fill(_buffer, _buffer + buffer_size, 0);
    read(_accept_socket_fd, _buffer, buffer_size);
}

void    m_socket::handler()
{
    cout << _buffer << endl;
}

void    m_socket::responder()
{
    const char* hello = "hello from server";
    write(_accept_socket_fd, hello, strlen(hello));
    close(_accept_socket_fd);
}

void    m_socket::launch()
{
    while (true)
    {
        cout << "+++++++Waiting for new connection+++++++" << endl;
        accepter();
        handler();
        responder();
        cout << "++++++++Done+++++++" << endl;
    }
}

struct sockaddr_in m_socket::get_address() const
{
    return (_address);
}

int m_socket::get_socket_fd()
{
    return (_socket_fd);
}
