/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: steh <steh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 17:50:12 by leng-chu          #+#    #+#             */
/*   Updated: 2023/04/06 16:52:42 by leng-chu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
# define SERVER_H

#include "parse8.hpp"

//#define BUF_SIZE	1048576
//#define BUF_SIZE	10
#define BUF_SIZE	1024
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
	string						_pwd;
	string						_host;
	size_t						_one_mb;
	size_t						_limit_size;

	int				startServer(int index);
	void			closeServer();
	void			acceptConnection(int &new_socket, int index);
	string			buildResponse();
	string			buildResponse2();
	void			sendResponse(int client_fd);
	void			sendErrorResponse(int client_fd, int statuscode);
	int				readClient(int fd, string & finalbuffer);
	void			redirect_Response(int client_fd, const string & url);
	int				checkFileExist(string & filepath);
	void			addSocketPoll(vector<struct pollfd> & fds);
	void			addClientPoll(vector<struct pollfd> & fds);
	void			clientRequestStage(vector<struct pollfd> & fds);
	void			removeClientPoll(vector<struct pollfd> & fds, int pos);
	int				unChunkRequest(int client_fd, string & clientBuffer);
	void			getHostUrl(string & clientRequest);
	void			setMethodUrl(string & method_type, string & uri_path, string & clientRequest);
	void			sendClient(int & client_fd, string & method_type, Request & req);

	// static non-member
	static Server		*server_instance;
	
	public:
		Server(vector<Server_Detail> & d_servers);
		~Server();

		void	startListen();
	
		// static non-member
		static void		sig_handler(int signo);
};

#endif
