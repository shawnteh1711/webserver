/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leng-chu <-chu@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 12:52:06 by leng-chu          #+#    #+#             */
/*   Updated: 2023/03/20 20:40:22 by leng-chu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "lib.hpp"

# define BUF_SIZE 30720

namespace N_LOG
{
	inline void	log(const string & msg)
	{
		cout << msg << endl;
	}

	inline void	ErrorExit(const string & msg)
	{
		log("ErrorExit: " + msg);
		exit(1);
	}
}

class Server
{
	struct sockaddr_in	server;
	int					sockfd;
	int					clientfd; // client's fd
	int					port;
	unsigned int		server_len;
	string				ip;
	string				serverMsg;
	public:
		Server(string ip_address, int port);
		~Server(void);
		Server(Server const & src);
		Server & operator=(Server const & rhs);

		//getter
		struct sockaddr_in get_server(void) const;
		int get_sockfd(void) const;
		int	get_clientfd(void) const;
		int	get_port(void) const;
		int	get_len(void) const;
		string get_ip(void) const;
		string get_sMsg(void) const;

		int	activeServer(void);
		void accept(int & clientfd);
		void send(void);
		string buildResponse(void);
		void listening(void);
};

#endif
