/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: steh <steh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/11 19:57:41 by steh              #+#    #+#             */
/*   Updated: 2023/03/12 15:42:12 by steh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP

#include "webserv.hpp"

class m_socket
{
    public:
        m_socket();
        m_socket(int domain, int service, int protocol, int port, u_long interface, int log);
        m_socket(const m_socket& other);
        ~m_socket();
        m_socket& operator=(const m_socket& other);
        int     create_socket(int domain, int type, int protocol);
        void    test_create_socket();
        void    set_socket_opt();
        int     bind_socket();
        void    test_bind_socket();
        int     listen_socket();
        void    accepter();
        void    handler();
        void    responder();
        void    launch();
        struct  sockaddr_in get_address() const;
        int     get_socket_fd();

    private:
        struct sockaddr_in  _address;
        int                 _socket_fd;
        int                 _accept_socket_fd;
        int                 _connection;
        int                 _log;
        int                 _yes;
        char                _buffer[buffer_size];

};

#endif
