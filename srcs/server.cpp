/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leng-chu <-chu@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 13:10:50 by leng-chu          #+#    #+#             */
/*   Updated: 2023/03/20 20:49:18 by leng-chu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

Server::Server(string ip_address, int port)
	: server(), sockfd(), clientfd(), port(port),
	server_len(sizeof(server)), ip(ip_address), serverMsg(buildResponse())
{
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = inet_addr(ip.c_str());

	if (activeServer() != 0)
	{
		ostringstream ss;
		ss << "Failed to start server with PORT: "
			<< ntohs(server.sin_port);
		N_LOG::log(ss.str());
	}
}

Server::~Server(void)
{
	close(sockfd);
	cout << "server destructor" << endl;
}

Server::Server(Server const & src)
:server(src.get_server()), sockfd(src.get_sockfd()),
	clientfd(src.get_clientfd()), port(src.get_port()),
	server_len(src.get_len()), ip(src.get_ip()), serverMsg(src.get_sMsg())
{
	cout << "copy constructor" << endl;
}

Server & Server::operator=(Server const & rhs)
{
	if (this != &rhs)
	{
		server = rhs.get_server();
		sockfd = rhs.get_sockfd();
		clientfd = rhs.get_clientfd();
		port = rhs.get_port();
		server_len = rhs.get_len();
		ip = rhs.get_ip();
		serverMsg = rhs.get_sMsg();
	}
	return (*this);
}

struct sockaddr_in Server::get_server(void) const
{
	return (server);
}

int	Server::get_sockfd(void) const
{
	return (sockfd);
}

int Server::get_clientfd(void) const
{
	return (clientfd);
}

int Server::get_port(void) const
{
	return (port);
}

int Server::get_len(void) const
{
	return (server_len);
}

string Server::get_ip(void) const
{
	return (ip);
}

string Server::get_sMsg(void) const
{
	return (serverMsg);
}

int	Server::activeServer(void)
{
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		N_LOG::ErrorExit("Cannot create socket");
		return (1);
	}

	int b = ::bind(sockfd, (sockaddr*)&server, sizeof(server));
	if (b < 0)
	{
		N_LOG::ErrorExit("Cannot connect socket to address");
		return (1);
	}
	return (0);
}

void	Server::listening(void)
{
	int				bytes;
	char			*buffer;
	ostringstream	ss;

	if ((::listen(sockfd, 5)) != 0)
		N_LOG::ErrorExit("Socket listen failed");

	cout << "\n*** Listening on ADDRESS: " << inet_ntoa(server.sin_addr) << endl;
	
	while(true)
	{
		N_LOG::log("===Waiting for a new connection===\n\n\n");
		accept(clientfd); // create function for class

		buffer = (char*)malloc(sizeof(char) * (BUF_SIZE + 1));
		bytes = read(clientfd, buffer, BUF_SIZE);
		if (bytes < 0)
			N_LOG::ErrorExit("Failed to read bytes from client socket");

		ss << "----- Received Request from client -----\n\n";
		N_LOG::log(ss.str());
		send(); // create this
		close(sockfd);
		close(clientfd);
	}
}

void	Server::accept(int & clientfd)
{
	cout << "mylen: " << server_len << endl;
	clientfd = ::accept(sockfd, (sockaddr*)&server, &server_len);
	if (clientfd < 0)
	{
		ostringstream ss;
		ss << "Server failed to accept the incoming connection from ADDRESS: " << inet_ntoa(server.sin_addr) << "; PORT: " << ntohs(server.sin_port);
		N_LOG::ErrorExit(ss.str());
	}
}

void	Server::send(void)
{
	long	bytes;

	bytes = write(clientfd, serverMsg.c_str(), serverMsg.size());
	if (bytes == (long)serverMsg.size())
		N_LOG::log("Successfully sent to the client");
	else
		N_LOG::log("Error sending to the client");
}

string	Server::buildResponse(void)
{
	string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1> HOME </h1><p> Hello from your Server :) </p></body></html>";
	ostringstream ss;
	ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << htmlFile.size() << "\n\n" << htmlFile;
	return ss.str();
}
