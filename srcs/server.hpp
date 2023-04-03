/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leng-chu <-chu@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 17:50:12 by leng-chu          #+#    #+#             */
/*   Updated: 2023/04/03 18:17:33 by leng-chu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
# define SERVER_H

#include "parse8.hpp"

//#define BUF_SIZE	1048576
#define BUF_SIZE	10
#define LIMIT_SIZE	1000 // for limit client body size

class	Server
{
	vector<Server_Detail>		servers;
	size_t						total;
//	int							_port;
	vector<int>					_sockfds;
//	int							_sockfd;
	int							_clientfd;
	string						_serverMsg;
//	string					_ip;
	vector<struct sockaddr_in>	_socketAddrs;
//	struct sockaddr_in			_socketAddr;
	unsigned int				_socketAddr_len;

	int				startServer(int index);
	void			closeServer();
	void			acceptConnection(int &new_socket, int index);
	string			buildResponse();
	void			sendResponse(int client_fd);
	void			sendErrorResponse(int client_fd, int statuscode);
	int				readClient(int fd, string & finalbuffer);

	// static non-member
	static Server		*server_instance;
	
	public:
	//	Server(void);
		Server(vector<Server_Detail> & d_servers);
		~Server();
	//	Server(const Server & src);

		void	startListen();

		// getter
		//int	get_port(void) const; // mine string
	
		// static non-member
		static void		sig_handler(int signo);
};

#endif
