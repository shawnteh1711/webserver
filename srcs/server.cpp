/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: steh <steh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 17:51:13 by leng-chu          #+#    #+#             */
/*   Updated: 2023/04/04 22:08:00 by leng-chu         ###   ########.fr       */
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
	: servers(d_servers), total(d_servers.size()), 
	_sockfds(total), _clientfd(),
	_serverMsg(buildResponse()), _socketAddrs(total) , _socketAddr_len(sizeof(_socketAddrs[0]))
//	_socketAddr(), _socketAddr_len(sizeof(_socketAddr))
{
	cout << "seervers: " << total << endl;
	vector<Server_Detail>::iterator it, ite;
	it = d_servers.begin();
	ite = d_servers.end();
	int	i = 0;
	while (it != ite)
	{
		_socketAddrs[i].sin_family = AF_INET;
		if (servers[i].port == "")
			servers[i].port = "80";
		_socketAddrs[i].sin_port = htons(stoi(servers[i].port));
		_socketAddrs[i].sin_addr.s_addr = INADDR_ANY;
		if (startServer(i) != 0)
		{
			ostringstream ss;
			ss << "Failed to start server with PORT: " << ntohs(_socketAddrs[i].sin_port);
			N_MY::msg(ss.str());
		}
		it++;
		i++;
	}
//	_socketAddr.sin_family = AF_INET;
//	_socketAddr.sin_port = htons(stoi(servers[0].port));
//	_socketAddr.sin_addr.s_addr = INADDR_ANY;

//	if (startServer() != 0)
//	{
//		ostringstream ss;
//		ss << "Failed to start server with PORT: " << ntohs(_socketAddr.sin_port);
//		N_MY::msg(ss.str());
//	}
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

int	Server::startServer(int index)
{
	int	reuse = 1;
	_sockfds[index] = socket(AF_INET, SOCK_STREAM, 0); // create sockfd 
	if (_sockfds[index] < 0)
		return N_MY::ErrorExit("Cannot create socket");
	if (setsockopt(_sockfds[index], SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
		return N_MY::ErrorExit("Cannot set socket option");
	if (bind(_sockfds[index], (sockaddr*)&_socketAddrs[index], _socketAddr_len) < 0)
	{
		std::perror(std::strerror(errno));
		return N_MY::ErrorExit("Cannot connect socket to address");
	}
	return (0);
}

void	Server::closeServer()
{
	for (size_t i = 0; i < _sockfds.size(); i++)
		close(_sockfds[i]);
	close(_clientfd);
//	exit(0);
}

void	Server::startListen()
{
	ostringstream	ss;
	int				bytes;
	int				total_bytes = 0;
	char			buffer[BUF_SIZE + 1];
	const int		MAX_CLIENTS = 1000;
	vector<struct pollfd>	fds(MAX_CLIENTS + total);
	//struct pollfd	*fds = new struct pollfd[MAX_CLIENTS + total];
	int				nfds = total; // total of socket descriptors
	size_t			one_mb = 1024 * 1024;
	size_t			limit_size = 1 * one_mb;
	string			finalbuffer;

	// this block for checking the cgi request
	string			cgi_path = "";
	int				found_cgi = 0;
	multimap<string, multimap<string, string> >::iterator it, ite;
	multimap<string, string>::iterator f, fe;

	// implementation to check if server has cgi request or not
	it = servers[0].mylocations.begin(), ite = servers[0].mylocations.end();
	while (it != ite)
	{
		cout << "===NEW ITERATOR===" << endl;
		cout << CYAN << "KEY: ";
		cout << YELLOW << it->first << endl;
		f = it->second.begin(); fe = it->second.end();
		f = it->second.find("fastcgi_pass");
		if (f != fe)
		{
			cout << GREEN << "FIND: " << f->first << " ==> " << f->second << RESET << endl;
			found_cgi = 1;
		}
		if (found_cgi)
		{
			cgi_path = it->first;
			cout << GREEN << cgi_path << RESET << endl;
		}
		it++;
	}
	found_cgi = 0;
	cout << RESET << endl;

//	memset(fds, 0, sizeof(*fds) * (MAX_CLIENTS + total));
	for (size_t i = 0; i < total; i++)
	{
		int flags = fcntl(_sockfds[i], F_GETFL, 0);
		fcntl(_sockfds[i], F_SETFL, flags |= O_NONBLOCK);
		if (listen(_sockfds[i], 20) < 0)
			N_MY::ErrorExit("Socket listen failed");
		ss << "\n*** Listening on ADDRESS: "
			<< inet_ntoa(_socketAddrs[i].sin_addr) << " PORT: " << servers[i].port;
		N_MY::msg(ss.str());
		fds[i].fd = _sockfds[i];
		fds[i].events = POLLIN;
	}

	while (1)
	{
		int rv = poll(&fds[0], nfds, -1);
		//cout << "POLLLLLLLLL" << endl;
		if (rv == -1)
			N_MY::ErrorExit("poll() failed");

		// Check if there is a new connection waiting on ther listening
		// check server fd's side
		for (size_t i = 0; i < total; i++)
		{
			if (fds[i].revents & POLLIN)
			{
				cout << YELLOW << "NEW CLIENT FD" << RESET << endl;
				acceptConnection(_clientfd, i);
		//		int flags = fcntl(_clientfd, F_GETFL, 0);
		//		fcntl(_clientfd, F_SETFL, flags |= O_NONBLOCK);
				fds[nfds].fd = _clientfd;
				fds[nfds].events = POLLIN | POLLERR | POLLHUP | POLLOUT;
			//	fds[nfds].revents = 0;
			//	cout << "fds[" << nfds << "] events: " << fds[nfds].events << " revents: " << fds[nfds].revents << endl; 
				nfds++;
				N_MY::msg("--- New client connected ---");
				cout << "clientMaxBodySize: " << servers[i].clientMaxBodySize << endl;
				if (servers[i].clientMaxBodySize != "")
					limit_size = stoi(servers[i].clientMaxBodySize) * one_mb;
				else 
					limit_size = one_mb;
			}
		}
		
				//cout << "fds[" << nfds - 1<< "] events: " << fds[nfds -1].events << " revents: " << fds[i].revents << endl; 
		
		// Check client fd's side
		// recv from client & also send to client fd too
		for (int i = total; i < nfds; i++)
		{
			if (fds[i].revents & (POLLERR | POLLHUP))
			{
				cout << "Enter POLLER || POLLHUP!!" << endl;
				close(fds[i].fd);
				nfds--;
				fds.erase(fds.begin() + i);
				//fds[i] = fds[nfds]; // what is this line for ?
				i--;
			}
			else if (fds[i].revents & POLLIN)
			{ 
				std::cout << "client in POLLIN" << std::endl;
				finalbuffer.clear();
				bzero(buffer, BUF_SIZE);
				total_bytes = 0;
				while ((bytes = recv(fds[i].fd, buffer, BUF_SIZE, 0)) > 0)
				{
					buffer[bytes] = '\0';
//					cout << "buffer: " << buffer << endl;
//					cout << YELLOW << "new loop" << RESET << endl;
//					cout << GREEN << "total bytes: " << total_bytes << endl;
//					cout << "new bytes: " << bytes << endl;
					finalbuffer += string(buffer, BUF_SIZE);
					total_bytes += bytes;
					if (buffer[BUF_SIZE - 1] == '\0')
						break ;
//					cout << CYAN << "finalbuffer: " << finalbuffer << RESET << endl;
					bzero(buffer, BUF_SIZE);
//					cout << "bzero done" << endl;
				}
				cout << YELLOW << "" << RESET << endl;
				if (bytes  == -1 && finalbuffer.size() == 0)
					N_MY::msg(RED"Failed to read bytes from client socket connection"RESET);
				else if (bytes == 0)
					N_MY::msg("The client has closed the connection"); // it triggered
				else
				{
					// Check the limit of the client body size
					//string clientRequest(buffer, bytes);
					
					string clientRequest = finalbuffer;
					cout << "BYTES from client: " << total_bytes << endl;
					cout << "clientRequest: " << clientRequest << endl;
					cout << "clientRequest.find(\"\\r\\n\\r\\\"):" << clientRequest.find("\r\n\r\n") << endl;
					size_t bodyPos = clientRequest.find("\r\n\r\n") + 4;
					cout << "bodyPos: " << bodyPos << endl;
					size_t bodySize = total_bytes;

					cout << "bodySize: " << bodySize << endl;
					cout << "limit_size: " << limit_size << " bytes" << endl;
					if (bodySize <= limit_size) // bodysize from client, limit_size from conf
					{
						// can you print this?
						cout << RED << "clientRequest"<< clientRequest  << RESET  << endl;
						Request req(clientRequest, cgi_path);
						size_t methodPos = clientRequest.find(" ");
						cout << YELLOW << "methodPos: " << methodPos << RESET << endl;
						// how to check if this is cgi request or not?
						req.hasCookies();
						if (req.is_cgi_request())
						{
							cout << "it has cgi request" << endl;
							req.handle_cgi(fds[i].fd);
						}
					//	if (methodPos == string::npos)
					//		sendErrorResponse(fds[i].fd, 400); 
				//		if (methodPos == 10)
				//			sendErrorResponse(fds[i].fd, 400); 
						else
						{
							N_MY::msg("--- Received Request from client ---");
							cout << "after close fds[" << i << "] events: " << fds[i].events << " revents: " << fds[i].revents << endl; 
					//		handle_non_cgi(fds[i].fd, req); // it is your request class
					//		if (fds[i].revents & (POLLOUT))
					//			cout << "POLLOUUUT!!" << endl;
							sendResponse(fds[i].fd);			
						}
					}
					else
						N_MY::msg("Client body size exceeded the limit\n\n"); // should this return or not?
				}
				close(fds[i].fd);
				// Remove the client socket from the array
				nfds--;
				fds[i] = fds[nfds];
				i--;
			}
		//	else if (fds[i].revents & (POLLERR | POLLHUP))
		//	{
		//		cout << "Enter POLLER || POLLHUP!!" << endl;
		//		close(fds[i].fd);
		//		close(fds[i].fd);
		//		nfds--;
		//		fds[i] = fds[nfds];
		//		i--;
		//	}
		}
	}
	//delete [] fds;
}

void	Server::acceptConnection(int &new_client, int index)
{
	new_client = accept(_sockfds[index], (sockaddr*)&_socketAddrs[index], &_socketAddr_len);
	if (new_client < 0)
	{
		ostringstream ss;
		ss << "Server failed to accept incoming connection from ADDRESS: " << inet_ntoa(_socketAddrs[index].sin_addr) << "; PORT: " << ntohs(_socketAddrs[index].sin_port);
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
