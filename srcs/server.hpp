/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: steh <steh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 17:50:12 by leng-chu          #+#    #+#             */
/*   Updated: 2023/04/11 16:43:37 by leng-chu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
# define SERVER_H

#include "parse8.hpp"

//#define BUF_SIZE	1048576
//#define BUF_SIZE	10
#define BUF_SIZE	1024
#define LIMIT_SIZE	1000 // for limit client body size

typedef struct s_server
{
	string	cgi_path;
	string	root_path;
	string	full_path;
	string	index_file;
	string	new_uri;
	string	client_uri;
	string	hostname;
	string	method_type;
	int		s;
}	t_server;

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
	multimap<string, string>	_store_body;
	t_server					s_t;

	int				startServer(int index);
	void			closeServer();
	void			acceptConnection(int &new_socket, int index);
	string			buildResponse(void);
	string			buildIndexList(void);
	void			sendResponse(const int client_fd);
	void			sendErrorResponse(const int client_fd, int statuscode);
	int				readClient(int fd, string & finalbuffer);
	void			redirect_Response(const int client_fd, const string & url);
	int				checkPathExist(string & filepath);
	int				checkDirectoryExist(const string & filepath);
	int				checkFileExist(const string & filepath);
	void			addSocketPoll(vector<struct pollfd> & fds);
	void			addClientPoll(vector<struct pollfd> & fds);
	void			clientRequestStage(vector<struct pollfd> & fds);
	void			removeClientPoll(vector<struct pollfd> & fds, int pos);
	int				unChunkRequest(const int client_fd, string & clientBuffer);
	void			getHostUrl(string & clientRequest);
	void			setMethodUrl(string & method_type, string & uri_path,
					string & clientRequest);
	void			sendClient(const int & client_fd, string & method_type,
					const string & uri_path, Request & req);
	int				getServerPoll(const int & client_fd);
	int				isCgiRequest(const string & s_uri, const int & svr_id,
			string & cgi_path);
	string			getLocationRoot(const string & s_uri, const int & svr_id);
	string			addslash(const string & s_uri);
	string			getStatusMessage(int statuscode);
	int				isIndexOn(const string & s_uri, const int & svr_id);
	int				isAllowUrlMethod(const string & s_uri, const int & svr_id,
			string & method_type);
	int				isMethod(string & method_type);
	void			sendCustomResponse(const int client_fd, string & full_path);
	int				isLocationExist(int const & svr_id, const string & s_uri);
	int				sendCustomErrorResponse(const int client_fd, int statuscode);
	void			sendCustomPostResponse(const int client_fd, string & full_path, map<string, string> & key_value_body);
	void			sendCustomPostResponse(const int client_fd, string & full_path, multimap<string, string> & key_value_body);
	void			checkFullPath(string & s_uri, const int & svr_id,
			string & root_path, string & full_path, const string & indexfile);
	void			checkServers(vector<Server_Detail> & servers);
	int				checkPort(vector<Server_Detail> & servers, string & defaultport);
	void			copyFiles(string &file_path, string &root_path);
	void			cleanServer(void);
	void			initServer(const int & client_fd, const string & uri_path, const string & method_type);
	void			sendGET(const int & client_fd, const string & uri_path);
	void			sendDELETE(const int & client_fd, const string & uri_path);

	// static non-member
	static Server		*server_instance;
	
	public:
		Server(vector<Server_Detail> & d_servers);
		~Server();

		void	startListen();
	
		// static non-member
		static void		sig_handler(int signo);
};

void	generate_listing(const string & path, string & listing);
void	generate_listing(t_server *s_t, string & listing);

#endif
