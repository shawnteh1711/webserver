/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tcpserver.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leng-chu <-chu@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 17:50:12 by leng-chu          #+#    #+#             */
/*   Updated: 2023/03/17 19:32:12 by leng-chu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
# define SERVER_H

#include "lib.hpp"

#define BUF_SIZE 30720

class	Server
{
	int			_port;
	int			_sockfd;
	int			_clientfd;
	string			_serverMsg;
	string			_ip;
	struct sockaddr_in	_socketAddr;
	unsigned int		_socketAddr_len;

	int			startServer();
	void			closeServer();
	void			acceptConnection(int &new_socket);
	string			buildResponse();
	void			sendResponse(int client_fd);
	
	public:
		Server(std::string ip_address, int port);
		~Server();

		void	startListen();
};

#endif
