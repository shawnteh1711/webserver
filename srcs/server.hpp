/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leng-chu <-chu@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 12:52:06 by leng-chu          #+#    #+#             */
/*   Updated: 2023/03/20 13:21:23 by leng-chu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "lib.hpp"

class Server
{
	struct sockaddr_in	server;
	struct sockaddr_in	client;
	int					sockfd;
	int					len;
	public:
		Server(void);
		~Server(void);
		Server(Server const & src);
		Server & operator=(Server const & rhs);

		//getter
		struct sockaddr_in get_server(void) const;
		struct sockaddr_in get_client(void) const;
		int get_sockfd(void) const;
		int	get_len(void) const;
};

#endif
