/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: steh <steh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 17:50:12 by leng-chu          #+#    #+#             */
/*   Updated: 2023/04/08 15:41:12 by leng-chu         ###   ########.fr       */
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
	size_t						_index;
	vector<int>					*tracker;

	int				startServer(int index);
	void			closeServer();
	void			acceptConnection(int &new_socket, int index);
	string			buildResponse(void);
	string			buildIndexList(void);
	void			sendResponse(int client_fd);
	void			sendErrorResponse(int client_fd, int statuscode);
	int				readClient(int fd, string & finalbuffer);
	void			redirect_Response(int client_fd, const string & url);
	int				checkPathExist(string & filepath);
	int				checkFileExist(const string & filepath);
	void			addSocketPoll(vector<struct pollfd> & fds);
	void			addClientPoll(vector<struct pollfd> & fds);
	void			clientRequestStage(vector<struct pollfd> & fds);
	void			removeClientPoll(vector<struct pollfd> & fds, int pos);
	int				unChunkRequest(int client_fd, string & clientBuffer);
	void			getHostUrl(string & clientRequest);
	void			setMethodUrl(string & method_type, string & uri_path,
					string & clientRequest);
	void			sendClient(int & client_fd, string & method_type,
					const string & uri_path, Request & req);
	int				getServerPoll(int & client_fd);
	int				isCgiRequest(const string & s_uri, const int & svr_id,
			string & cgi_path);
	string			getLocationRoot(const string & s_uri, const int & svr_id);
	string			addslash(const string & s_uri);
	string			getStatusMessage(int statuscode);
	int				isIndexOn(const string & s_uri, const int & svr_id);
	int				isAllowUrlMethod(const string & s_uri, const int & svr_id,
			string & method_type);
	int				isMethod(string & method_type);
	void			sendCustomResponse(int client_fd, string & full_path);
	int				isLocationExist(int const & svr_id, const string & s_uri);
	int				sendCustomErrorResponse(int client_fd, int statuscode,
			int svr_id, const string & root);

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
