/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: steh <steh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/11 16:24:54 by steh              #+#    #+#             */
/*   Updated: 2023/03/13 12:00:15 by steh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "webserv.hpp"

class server
{
    public:
        server();
        server(ifstream& config_file);
        server(const server& other);
        ~server();
        server& operator=(const server& other);

        // Member function
        void    setup();
    
    private:
        config                  _config;
        vector<m_socket>        _sockets;
};

# endif
