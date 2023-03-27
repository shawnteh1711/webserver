/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leng-chu <-chu@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 17:50:12 by leng-chu          #+#    #+#             */
/*   Updated: 2023/03/27 15:23:04 by leng-chu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
# define SERVER_H

#include "parse8.hpp"

#define BUF_SIZE 30720
#define LIMIT_SIZE 1000 // for limit client body size

class	Server
{
	vector<Server_Detail>	servers;
//	int						_port;
	int						_sockfd;
	int						_clientfd;
	string					_serverMsg;
//	string					_ip;
	struct sockaddr_in		_socketAddr;
	unsigned int			_socketAddr_len;

	int			startServer();
	void			closeServer();
	void			acceptConnection(int &new_socket);
	string			buildResponse();
	void			sendResponse(int client_fd);
	void			sendErrorResponse(int client_fd, int statuscode);

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
