/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: steh <steh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/11 16:24:54 by steh              #+#    #+#             */
/*   Updated: 2023/03/11 18:08:19 by steh             ###   ########.fr       */
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

        // Member function
        void    parse(ifstream& config_file);

};

# endif
