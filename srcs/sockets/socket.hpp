/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: steh <steh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/11 19:57:41 by steh              #+#    #+#             */
/*   Updated: 2023/03/11 21:05:38 by steh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP

#include "webserv.hpp"

class m_socket
{
    public:
        m_socket();
        m_socket(int domain, int service, int protocol, int port, u_long interface);
        m_socket(const m_socket& other);
        ~m_socket();
        m_socket& operator=(const m_socket& other);
        int     create_socket(int domain, int type, int protocol);
        int     bind_socket(int sockfd, struct sockaddr_in address);
        int     listen();
        int     accept();
        struct  sockaddr_in get_address() const;
        void    close();
        void    test_create_socket(int socket_fd);

    private:
        struct sockaddr_in  _address;
        int                 _socket_fd;
        int                 _connection;

};

#endif
