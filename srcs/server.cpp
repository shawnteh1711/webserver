/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leng-chu <-chu@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 17:51:13 by leng-chu          #+#    #+#             */
/*   Updated: 2023/03/27 15:22:56 by leng-chu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

namespace N_MY
{
	void	msg(const string &message)
	{
		std::cout << message << std::endl;
	}

	int	ErrorExit(const string &errorMessage)
	{
		cout << RED;
		msg("ERROR: " + errorMessage);
		cout << RESET;
		exit(1);
		return (1);
	}
}

Server *Server::server_instance = NULL;

//Server::Server(void)
//	: servers(), _port(-1), _sockfd(-1), _clientfd(-1), _serverMsg(),
//	_ip(), _socketAddr(), _socketAddr_len(){}

// d_servers's port is string type
Server::Server(vector<Server_Detail> & d_servers)
	: servers(d_servers), _sockfd(), _clientfd(),
	_serverMsg(buildResponse()),
	_socketAddr(), _socketAddr_len(sizeof(_socketAddr))
{
	cout << "servers[0].port: " << servers[0].port << endl;
	_socketAddr.sin_family = AF_INET;
	_socketAddr.sin_port = htons(stoi(servers[0].port));
	//_socketAddr.sin_addr.s_addr = inet_addr(_ip.c_str());
	//_socketAddr.sin_addr.s_addr = in6addr_any;
	_socketAddr.sin_addr.s_addr = INADDR_ANY;
	// ip is string, s_addr is unsigned int
	// because of dot. so need inet_addr. 

	if (startServer() != 0)
	{
		ostringstream ss;
		ss << "Failed to start server with PORT: " << ntohs(_socketAddr.sin_port);
		N_MY::msg(ss.str());
	}
	server_instance = this;
}

//Server::Server(const Server & src)
//: _port(src._port), _sockfd(src._sockfd), _clientfd(src._clientfd),
//	_serverMsg(src._serverMsg), _ip(src._ip),
//	_socketAddr(src._socketAddr), _socketAddr_len(src._socketAddr_len)
//{}

Server::~Server()
{
	cout << RED << "Server close" << RESET << endl;
	closeServer();
}

int	Server::startServer()
{
	int	reuse = 1;
	_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (_sockfd < 0)
		return N_MY::ErrorExit("Cannot create socket");
	if (setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
		return N_MY::ErrorExit("Cannot set socket option");
	if (bind(_sockfd, (sockaddr*)&_socketAddr, _socketAddr_len) < 0)
	{
		std::perror(std::strerror(errno));
		return N_MY::ErrorExit("Cannot connect socket to address");
	}
	return (0);
}

void	Server::closeServer()
{
	close(_sockfd);
	close(_clientfd);
//	exit(0);
}

void	Server::startListen()
{
	ostringstream	ss;
	int		bytes;
	char		buffer[BUF_SIZE];
	
	if (listen(_sockfd, 20) < 0)
		N_MY::ErrorExit("Socket listen failed");
	ss << "\n*** Listening on ADDRESS: " 
		<< inet_ntoa(_socketAddr.sin_addr) << " PORT: " << servers[0].port;
	N_MY::msg(ss.str());

	// Create an array of pollfd structs,
	// one for the listening socket
	const int	MAX_CLIENTS = 10;
	struct pollfd	fds[MAX_CLIENTS + 1];
	memset(fds, 0, sizeof(fds));
	fds[0].fd = _sockfd;
	fds[0].events = POLLIN;

	int nfds = 1; // Number of active socket fds.
	while (1)
	{
		int rv = poll(fds, nfds, -1);
		if (rv == -1)
			N_MY::ErrorExit("poll() failed");

		// Check if there is a new connection waiting on ther listening
		if (fds[0].revents & POLLIN)
		{
			acceptConnection(_clientfd);
			fds[nfds].fd = _clientfd;
			fds[nfds].events = POLLIN;
			nfds++;
			N_MY::msg("--- New client connected ---");
		}

		for (int i = 1; i < nfds; i++)
		{
			if (fds[i].revents & POLLIN)
			{
				bzero(buffer, BUF_SIZE);
				bytes = recv(fds[i].fd, buffer, BUF_SIZE, 0);
				if (bytes  == -1)
					N_MY::msg(RED"Failed to read bytes from client socket connection"RESET);
				if (bytes == 0)
					N_MY::msg("The client has closed the connection");
				else
				{

					// Check the limit of the client body size
					string clientRequest(buffer, bytes);
					cout << "clientRequest: " << clientRequest << endl;
					size_t bodyPos = clientRequest.find("\r\n\r\n") + 4;
					size_t bodySize = bytes - bodyPos;
	
					if (bodySize <= LIMIT_SIZE)
					{
						size_t methodPos = clientRequest.find(" ");
						cout << "methodPos: " << methodPos << endl;
					//	if (methodPos == string::npos)
					//		sendErrorResponse(fds[i].fd, 400); 
						if (methodPos == 10)
							sendErrorResponse(fds[i].fd, 400); 
						else
						{
							N_MY::msg("--- Received Request from client ---");
							sendResponse(fds[i].fd);			
						}
					}
					else
						N_MY::msg("Client body size exceeded the limit\n\n");
				}

				close(fds[i].fd);

				// Remove the client socket from the array
				nfds--;
				fds[i] = fds[nfds];
				i--;
			}
		}
	}
}

void	Server::acceptConnection(int &new_client)
{
	new_client = accept(_sockfd, (sockaddr*)&_socketAddr, &_socketAddr_len);
	if (new_client < 0)
	{
		ostringstream ss;
		ss << "Server failed to accept incoming connection from ADDRESS: " << inet_ntoa(_socketAddr.sin_addr) << "; PORT: " << ntohs(_socketAddr.sin_port);
		N_MY::ErrorExit(ss.str());
	}
}

string Server::buildResponse()
{
	string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1> HOME </h1><p> Hello from your Server :) </p></body></html>";
	ostringstream ss;
	ss << "HTTP/1.1 200 OK\r\n"
	   << "Content-Type: text/html\r\n"
	   << "Content-Length: " << htmlFile.size() 
	   << "\r\n\r\n"
	   << htmlFile;
	return ss.str();
}

void Server::sendResponse(int client_fd)
{
	long	bytesSent;

	// send(client_fd, _serverMsg.c_str()n
	bytesSent = send(client_fd, _serverMsg.c_str(), _serverMsg.size(), 0);
	if (bytesSent == -1)
		N_MY::msg("Error sending response to client");
	else if (bytesSent == 0)
		N_MY::msg("Server closed the connection with the client");
	else
		N_MY::msg("Server sent a response to the client\n\n");
}

void Server::sendErrorResponse(int client_fd, int statuscode)
{
	string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><center><h1> Bad Request </h1><c/center></body></html>";
	ostringstream ss;
	ss << "HTTP/1.1 " << statuscode << " ";
	string statusMessage;
	long	bytesSent;

	switch (statuscode)
	{
		case 400:
			statusMessage = "Bad Request";
			break ;
		case 404:
			statusMessage = "Not Found";
			break ;
		case 500:
			statusMessage = "Internal Server Error";
			break ;
		default:
			statusMessage = "Unknown Status Code";
	}
	ss << statusMessage << "\r\n"
	   << "Content-Length: " << htmlFile.size()
	   << "\r\n\r\n"
	   << htmlFile;
	string response = ss.str();
	cout << response << endl;
	bytesSent = send(client_fd, response.c_str(), response.size(), 0);
	if (bytesSent == (long)response.size())
		N_MY::msg("---- Server Error Response sent to client ---- \n\n");
	else
		N_MY::msg("Error sending response to client");
}

void	Server::sig_handler(int signo)
{
	cout << "Ah you click signal " << signo << endl;
	server_instance->closeServer();
	exit(0);
}
