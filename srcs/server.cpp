/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: steh <steh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 17:51:13 by leng-chu          #+#    #+#             */
/*   Updated: 2023/04/06 22:03:32 by leng-chu         ###   ########.fr       */
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
	: servers(d_servers), total(d_servers.size()), _sockfds(total),
	_clientfd(), _serverMsg(buildResponse2()), _socketAddrs(total), 
	_socketAddr_len(sizeof(_socketAddrs[0])), _pwd(getenv("PWD")), _host(),
	_one_mb(1024 * 1024), _limit_size(_one_mb), _index(0), tracker(new vector<int>[total])
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
//	map<string, string>::iterator tryit = servers[0].urlCgi.begin();
//	(void)tryit;
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
	vector<struct pollfd>	fds;

	addSocketPoll(fds);
	while (1)
	{
		//usleep(2000);
		int rv = poll(&fds[0], fds.size(), -1);

		//cout << "POLLLLLLLLL" << endl;
		if (rv == -1)
			N_MY::ErrorExit("poll() failed");
		addClientPoll(fds);
		clientRequestStage(fds);
	}
}

void	Server::acceptConnection(int &new_client, int index)
{
	new_client = accept(_sockfds[index],
			(sockaddr*)&_socketAddrs[index], &_socketAddr_len);
	if (new_client < 0)
	{
		ostringstream ss;
		ss << "Server failed to accept incoming connection from ADDRESS: "
		   << inet_ntoa(_socketAddrs[index].sin_addr)
		   << "; PORT: " << ntohs(_socketAddrs[index].sin_port);
		N_MY::ErrorExit(ss.str());
	}
}

void generate_listing(string path, string &listing)
{
    DIR				*dir;
    struct dirent	*ent;

    // Open the directory
    if ((dir = opendir(path.c_str())) != NULL) {
        // Iterate through the directory entries
		listing += "<p>Path: " + path + "</p>";
        while ((ent = readdir(dir)) != NULL) {
            listing += "<li>" + string(ent->d_name) + "</li>";
        }
        closedir(dir);
    } else {
        cerr << "Error opening directory: " << path << endl;
    }
}

string Server::buildResponse()
{
	string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1> \
					   HOME </h1><p> Hello from your Server :) \
					   </p></body></html>";
	ostringstream ss;
	ss << "HTTP/1.1 200 OK\r\n"
	   << "Content-Type: text/html\r\n"
	   << "Content-Length: " << htmlFile.size() 
	   << "\r\n\r\n"
	   << htmlFile;
	return ss.str();
}

string Server::buildResponse2()
{
	string htmlFile = "<html><body>Trying autoindex<ul>";
	string 	path = "/Users/steh/Documents/own_folder/webserver/kapouet"; // now hardcode
    generate_listing(path, htmlFile); //for autoindex directory
	htmlFile += "</ul></body></html>";
	ostringstream ss;
	ss << "HTTP/1.1 200 OK\r\n"
	   << "Content-Type: text/html\r\n"
	   << "Content-Length: " << htmlFile.size() 
	   << "\r\n\r\n"
	   << htmlFile;
	return ss.str();
}

void Server::redirect_Response(int client_fd, const string & url)
{
	long bytesSent;

    ostringstream ss;
    ss << "HTTP/1.1 302 Found\r\n"
       << "Location: " << url << "\r\n"
       << "Connection: close"
       << "\r\n\r\n";
    bytesSent = send(client_fd, ss.str().c_str(), ss.str().length(), 0);
	if (bytesSent == -1)
		N_MY::msg("Error sending response to client");
	else if (bytesSent == 0)
		N_MY::msg("Server closed the connection with the client");
	else
		N_MY::msg("Server sent a response to the client\n\n");
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
	string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><center><h1>  Error!! </h1><p>" + statusMessage + "</p></center></body></html>";
	ss << statusMessage << "\r\n"
	   << "Content-Length: " << htmlFile.size()
	   << "\r\n\r\n"
	   << "<!DOCTYPE html><html lang=\"en\"><body><center><h1> "
	   << statuscode << " Error!! </h1><p>" << statusMessage
	   << "</p></center></body></html>";
	string response = ss.str();
	cout << response << endl;
	bytesSent = send(client_fd, response.c_str(), response.size(), 0);
	if (bytesSent == (long)response.size())
		N_MY::msg("---- Server Error Response sent to client ---- \n\n");
	else
		N_MY::msg("Error sending response to client");
}

int	Server::checkFileExist(string & filepath)
{
	ifstream	ifile(filepath);
	char 		buffertest[256];
	ifile.read(buffertest, sizeof(buffertest));
	if (ifile) 
		return (1);
	return (0);
}

void	Server::addSocketPoll(vector<struct pollfd> & fds)
{
	struct pollfd	new_poll;
	ostringstream	ss;
	for (size_t i = 0; i < total; i++)
	{
		int flags = fcntl(_sockfds[i], F_GETFL, 0);
		fcntl(_sockfds[i], F_SETFL, flags |= O_NONBLOCK);
		if (listen(_sockfds[i], 20) < 0)
			N_MY::ErrorExit("Socket listen failed");
		ss << "\n*** Listening on ADDRESS: "
			<< inet_ntoa(_socketAddrs[i].sin_addr) << " PORT: " << servers[i].port;
		N_MY::msg(ss.str());
		new_poll.fd = _sockfds[i];
		new_poll.events = POLLIN;
		fds.push_back(new_poll);
	}
}

void	Server::addClientPoll(vector<struct pollfd> & fds)
{
	struct pollfd	new_poll;

	for (size_t i = 0; i < total; i++)
	{
		if (fds[i].revents & POLLIN)
		{
			cout << YELLOW << "NEW CLIENT FD" << RESET << endl;
			acceptConnection(_clientfd, i);
			tracker[i].push_back(_clientfd);
			new_poll.fd = _clientfd;
			new_poll.events = POLLIN | POLLERR | POLLHUP | POLLOUT;
			fds.push_back(new_poll);
			N_MY::msg("--- New client connected ---");
			if (servers[i].clientMaxBodySize != "")
				_limit_size = stoi(servers[i].clientMaxBodySize) * _one_mb;
			else 
				_limit_size = _one_mb;
			_index = i;
		}
	}
}

int	Server::getServerPoll(int & client_fd)
{
	vector<int>::iterator it, ite;
	for (size_t i = 0; i < total; i++)
	{
		it = tracker[i].begin();
		ite = tracker[i].end();
		while (it != ite)
		{
			if (*it == client_fd)
				return (i);
			it++;
		}
	}
	return (-1);
}

void	Server::removeClientPoll(vector<struct pollfd> & fds, int pos)
{
	int	cfd = fds[pos].fd;
	int s = getServerPoll(cfd);
	
	tracker[s].erase(::find(tracker[s].begin(), tracker[s].end(), cfd));
	close(cfd);
	fds.erase(fds.begin() + pos);
}

void	Server::clientRequestStage(vector<struct pollfd> & fds)
{
	size_t					total_bytes = 0;
	string					clientRequest;
	string					method_type;
	string					uri_path;
	
	for (size_t i = total; i < fds.size(); i++)
	{
		if (fds[i].revents & (POLLERR | POLLHUP))
		{
			cout << "Enter POLLER || POLLHUP!!" << endl;
			removeClientPoll(fds, i--);
		}
		else if (fds[i].revents & POLLIN)
		{ 
			std::cout << "client in POLLIN" << std::endl;
			total_bytes = unChunkRequest(fds[i].fd, clientRequest);
			if (total_bytes > 0)
			{
				cout << CYAN"BYTES from client: " << total_bytes << endl;
				cout << "clientRequest: " << clientRequest << endl;
				
				// get url from client
				getHostUrl(clientRequest);
				cout << "bodySize(total bytes): " << total_bytes << endl;
				cout << "_limit_size: " << _limit_size << " bytes" << RESET << endl;
				if (total_bytes <= _limit_size)
				{
					// here i add check if is cgi request
					// coz check is cgi request is my task to do here server
					// the server need check cgi request or not before send to you
					setMethodUrl(method_type, uri_path, clientRequest);
					Request req(clientRequest, uri_path);
					sendClient(fds[i].fd, method_type, uri_path, req);
				}
				else
				{
					N_MY::msg("Client body size exceeded the limit\n\n");
					sendErrorResponse(fds[i].fd, 400);
				}
			}
			removeClientPoll(fds, i--);
		}
	}
}

int		Server::unChunkRequest(int client_fd, string & clientBuffer)
{
	char					buffer[BUF_SIZE + 1];
	string					finalbuffer;
	int						bytes;
	size_t					total_bytes = 0;
	
	finalbuffer.clear();
	bzero(buffer, BUF_SIZE);
	while ((bytes = recv(client_fd, buffer, BUF_SIZE, 0)) > 0)
	{
		buffer[bytes] = '\0';
		finalbuffer += string(buffer, BUF_SIZE);
		total_bytes += bytes;
		if (buffer[BUF_SIZE - 1] == '\0')
			break ;
		bzero(buffer, BUF_SIZE);
	}
	if (bytes == -1 && finalbuffer.size() == 0)
	{
		N_MY::msg(RED"Failed to read client request"RESET);
		return (bytes);
	}
	else if (bytes == 0)
	{
		N_MY::msg("The client has closed the connection");
		return (bytes);
	}
	clientBuffer = finalbuffer;
	return (total_bytes);
}

void	Server::getHostUrl(string & clientRequest)
{
	string	tmp;
	int		pos;
	if (clientRequest.find("localhost") != string::npos)
	{
		pos = clientRequest.find("localhost");
		tmp = clientRequest.substr(pos, clientRequest.size());
		_host = tmp.substr(0, tmp.find(" "));
	}
}

void	Server::setMethodUrl(string & method_type, string & uri_path, string & clientRequest)
{
	int	pos;

	pos = clientRequest.find(" ");
	method_type = clientRequest.substr(0, pos);
	uri_path = clientRequest.substr(pos + 1, clientRequest.length());
	pos = uri_path.find(" ");
	uri_path = uri_path.substr(0, pos - 1);
}

void	Server::sendClient(int & client_fd, string & method_type,
		string & uri_path, Request & req)
{
	cout << YELLOW << "method Type: " << method_type << RESET << endl;
	cout << YELLOW << "uri_path: " << uri_path << RESET << endl;
	int s = getServerPoll(client_fd);
	cout << YELLOW << "server poll id: " << s << RESET << endl;

	cout << "server " << servers[s].port << " 's root => " << servers[s].root << endl;
	
	string full_path = servers[s].root + uri_path; // pls test this
	map<string, string>::iterator it, ite;
	it = servers[s].urlCgi.begin();
	ite= servers[s].urlCgi.end();
	// your cgi can return error page if the full path i passed to you is wrong or not found file. thats all.
	while (it != ite)
	{
		cout << CYAN << endl;
		cout << it->first << " => " << it->second << endl;
		it++;
	}
	cout << RESET << endl;
	if (!checkFileExist(uri_path) && uri_path != "")
		sendErrorResponse(client_fd, 404);
	else if (req.is_cgi_request())
	{
		cout << GREEN << "it has cgi request" << RESET << endl;
		req.handle_cgi(client_fd);
		// or req.handle_cgi(client_fd, full_path)
		// create handle_cgi2 with new parameter full_path to path_info
	}
	else if (method_type == "GET")
	{
		N_MY::msg("--- Received Request from client ---");
//		if (_host.find("localhost:8080") != string::npos)
//			redirect_Response(client_fd, "http://localhost:1024");
//		else
			sendResponse(client_fd);
	}
	else
		sendErrorResponse(client_fd, 400);
}

void	Server::sig_handler(int signo)
{
	cout << "Ah you click signal " << signo << endl;
	server_instance->closeServer();
	exit(0);
}
