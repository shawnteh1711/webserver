/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leng-chu <-chu@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 13:10:50 by leng-chu          #+#    #+#             */
/*   Updated: 2023/03/20 13:28:18 by leng-chu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

Server::Server(void)
{
	cout << "server constructotr" << endl;
}

Server::~Server(void)
{
	cout << "server destructor" << endl;
}

Server::Server(Server const & src)
	: server(src.get_server()), client(src.get_client()), sockfd(src.get_sockfd()), len(src.get_len())
{
	cout << "copy constructor" << endl;
}

Server & Server::operator=(Server const & rhs)
{
	if (this != &rhs)
	{
		server = rhs.get_server();
		client = rhs.get_client();
		sockfd = rhs.get_sockfd();
		len = rhs.get_len();
	}
	return (*this);
}

struct sockaddr_in Server::get_server(void) const
{
	return (this->server);
}

struct sockaddr_in Server::get_client(void) const
{
	return (this->client);
}

int	Server::get_sockfd(void) const
{
	return (this->sockfd);
}

int	Server::get_len(void) const
{
	return (this->len);
}
