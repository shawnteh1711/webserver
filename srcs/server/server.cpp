/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: steh <steh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/11 17:43:00 by steh              #+#    #+#             */
/*   Updated: 2023/03/12 21:59:59 by steh             ###   ########.fr       */
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
    int             server_count;
    int             max_fd;
    int             ret;
    fd_set          read_fds, write_fds, except_fds;
    struct timeval  tv;

    // server_count = _config.get_server_count();
    server_count = 3;
    for (int i = 0; i < server_count; ++i)
    {
        // m_socket socket(_config.get_domain(), _config.get_service(), _config.get_protocol(), _config.get_port(), _config.get_interface());
        m_socket    sock;

        _sockets.push_back(sock);
    }
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    FD_ZERO(&except_fds);

    max_fd = -1;
    for (size_t i = 0; i < _sockets.size(); ++i)
    {
        FD_SET(_sockets[i].get_socket_fd(), &read_fds);
        if (_sockets[i].get_socket_fd() > max_fd)
            max_fd = _sockets[i].get_socket_fd();
    }

    while (true)
    {
        tv.tv_sec = 0;
        tv.tv_usec = 0;
        ret = select(max_fd + 1, &read_fds, &write_fds, &except_fds, &tv);
        if (ret == -1)
        {
            perror("select");
            exit(EXIT_FAILURE);
        }
        else if (ret > 0)
        {
            for (size_t i = 0; i < _sockets.size(); ++i)
            {
                if (FD_ISSET(_sockets[i].get_socket_fd(), &read_fds))
                {
                    _sockets[i].accepter();
                }
            }
        }
    }
}
