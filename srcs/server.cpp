/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: steh <steh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 17:51:13 by leng-chu          #+#    #+#             */
/*   Updated: 2023/04/08 21:06:14 by steh             ###   ########.fr       */
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
	_clientfd(), _serverMsg(buildResponse()), _socketAddrs(total), 
	_socketAddr_len(sizeof(_socketAddrs[0])), _pwd(getenv("PWD")), _host(),
	_one_mb(1024 * 1024), _limit_size(_one_mb), _index(0), tracker(new vector<int>[total])
{
	// cout << "seervers: " << total << endl;
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

string Server::buildResponse(void)
{
	string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1> \
					   DEFAULT PAGE </h1><p> Hello!! :DD \
					   </p></body></html>";
	ostringstream ss;
	ss << "HTTP/1.1 200 OK\r\n"
	   << "Content-Type: text/html\r\n"
	   << "Content-Length: " << htmlFile.size()
	   << "\r\n\r\n"
	   << htmlFile;
	return ss.str();
}

string Server::buildIndexList(void)
{
	string htmlFile = "<html><body>Trying autoindex<ul>";
	// need to change path using ROOT?
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

	bytesSent = send(client_fd, _serverMsg.c_str(), _serverMsg.size(), 0);
	if (bytesSent == -1)
		N_MY::msg("Error sending response to client");
	else if (bytesSent == 0)
		N_MY::msg("Server closed the connection with the client");
	else
		N_MY::msg("Server sent a response to the client\n\n");
}

string	Server::getStatusMessage(int statuscode)
{
	string statusMessage;
	switch (statuscode)
	{
		case 400:
			statusMessage = "Bad Request";
			break ;
		case 404:
			statusMessage = "Not Found";
			break ;
		case 405:
			statusMessage = "Method Not Allowed";
			break ;
		case 500:
			statusMessage = "Internal Server Error";
			break ;
		default:
			statusMessage = "Unknown Status Code";
	}
	return (statusMessage);
}

void Server::sendErrorResponse(int client_fd, int statuscode)
{
	ostringstream ss;
	stringstream i2s;
	string statusMessage, code;
	long	bytesSent;

	i2s << statuscode;
	i2s >> code;
	statusMessage = getStatusMessage(statuscode);
	string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><center><h1> " + code + " Default Error!! </h1><p>" + statusMessage + "</p></center></body></html>";
	ss << "HTTP/1.1 " << statuscode << " "
	   << statusMessage << "\r\n"
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

int	Server::sendCustomErrorResponse(int client_fd, int statuscode,
		int svr_id, const string & root)
{
	map<int, string>::iterator	it, ite;
	stringstream				tmp;
	ostringstream				ss;
	int							bytesSent;
	string						full_path, erroroot, html_content, statusMessage;

	erroroot = root;
	it = servers[svr_id].errorPageMap.find(statuscode);
	ite = servers[svr_id].errorPageMap.end();
	if (it != ite)
	{
		if (root[root.length() - 1] == '/' && it->second[0] == '/')
			erroroot.pop_back();
		full_path = erroroot + it->second;
		cout << "full path to custom error: " << full_path << endl;
		if (!checkFileExist(full_path))
		{
			cout << "Custom Error path not exist!!" << endl;
			sendErrorResponse(client_fd, statuscode);
			return (0);
		}
		ifstream file(full_path);
		tmp << file.rdbuf();
		html_content = tmp.str();
		
		ss << "HTTP/1.1 " << statuscode << " "
		   << getStatusMessage(statuscode) << "\r\n"
		   << "Content-Length: " << html_content.size()
		   << "\r\n\r\n"
		   << html_content;
		_serverMsg = ss.str();
		bytesSent = send(client_fd, _serverMsg.c_str(), _serverMsg.size(), 0);
		if (bytesSent == -1)
			N_MY::msg("Error sending response to client");
		else if (bytesSent == 0)
			N_MY::msg("Server closed the connection with the client");
		else
			N_MY::msg("Server sent a response to the client\n\n");
		return (1);
	}
	sendErrorResponse(client_fd, statuscode);
	return (0);
}

int	Server::checkPathExist(string & filepath)
{
	cout << "filepath in checkPathExist: " << filepath << endl;
	ifstream	ifile(filepath);
	char 		buffertest[BUF_SIZE + 100000];
	
	// ifile.read(buffertest, sizeof(buffertest));
	// cout << YELLOW << "buffertest: " << buffertest << RESET << endl;
	memset(buffertest, 0, sizeof(buffertest)); // initialize buffer with zeros
	if (ifile.good()) 
	{
		ifile.read(buffertest, sizeof(buffertest));
		if (ifile.gcount() > 0)
			buffertest[ifile.gcount()] = '\0';
		cout << YELLOW << "buffertest: " << buffertest << RESET << endl;
		return (1);
	}
	cout << RED << "File not exist" << RESET << endl;
	return (0);
}

int	Server::checkDirectoryExist(const string & filepath)
{
	cout << "filepath in checkDirectoryExist: " << filepath << endl;
	ifstream	ifile(filepath);
	struct stat	file_stat;

	if (stat(filepath.c_str(), &file_stat) == 0)
	{
		if (S_ISDIR(file_stat.st_mode))
		{
			_serverMsg = buildResponse();
			return (1);
		}
	}
	return (0);
}

int	Server::checkFileExist(const string & filepath)
{
	cout << "filepath in checkFileExist: " << filepath << endl;
	ifstream	ifile(filepath);
	char 		buffertest[BUF_SIZE + 100000];
	struct stat file_stat;
	
	if (stat(filepath.c_str(), &file_stat) == 0)
	{
		if (!S_ISDIR(file_stat.st_mode)) // only check is not directory
		{
			memset(buffertest, 0, sizeof(buffertest));
			if (ifile.good()) 
			{
				ifile.read(buffertest, sizeof(buffertest));
				if (ifile.gcount() > 0)
					buffertest[ifile.gcount()] = '\0';
				cout << YELLOW << "buffertest: " << buffertest << RESET << endl;
				return (1);
			}
			else
				return (0);
		}
	}
	cout << "File NOT FOUND" << endl;
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
		RED;
		N_MY::msg("Failed to read client request");
		RESET;
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
	uri_path = uri_path.substr(1, pos - 1);
}

void	Server::sendClient(int & client_fd, string & method_type,
		const string & uri_path, Request & req)
{
	string	cgi_path, root_path, full_path, index_file;
	int		s;
	//string full_path = servers[s].root + uri_path;

	s = getServerPoll(client_fd);
	index_file = servers[s].index;
	cgi_path = "";
	if (index_file == "")
		index_file = "index.html";
	cout << "STARTTTT" << endl;
	if (((root_path = getLocationRoot(uri_path, s)) == ""))
		root_path = servers[s].root;
	
	cout << YELLOW << "method_type: " << method_type << RESET << endl;
	cout << YELLOW << "uri_path: " << uri_path << RESET << endl;
	cout << YELLOW << "root_path: " << root_path << RESET << endl;
	cout << YELLOW << "index filename: " << index_file << RESET << endl;
	cout << YELLOW << "server poll id: " << s << RESET << endl;
	cout << YELLOW << "AutoIndex: "; isIndexOn(uri_path, s) ? cout << "ON" : cout << "OFF"; cout << endl;
	cout << YELLOW << "Is Location Existing?: "; isLocationExist(s, uri_path) ? cout << "YES" : cout << "NOT EXIST"; cout << endl;
	cout << RESET << endl;

	full_path = root_path + index_file;
	cout << YELLOW << "Full path: " << full_path << RESET << endl;
	cout << "======Before enter Method=====" << endl;
	if (!isMethod(method_type))
		sendCustomErrorResponse(client_fd, 400, s, root_path);
	else if (root_path == "" || !isLocationExist(s, uri_path))
	{
		sendCustomErrorResponse(client_fd, 404, s, root_path);
	}
	else if (isCgiRequest(uri_path, s, cgi_path))
	{
		cout << GREEN << "it has cgi request" << endl;
		cout << "cgi_path i give to you: " << cgi_path << RESET << endl;
		req.handle_cgi2(client_fd, cgi_path);
	}
	else if (!isAllowUrlMethod(uri_path, s, method_type))
		sendCustomErrorResponse(client_fd, 405, s, root_path);
	else if (!checkFileExist(full_path) && uri_path != "" && !(isIndexOn(uri_path, s)))
		sendCustomErrorResponse(client_fd, 404, s, root_path);
	else if (method_type == "GET")
	{
		checkFullPath(uri_path, s, root_path, full_path);
		if  (isIndexOn(uri_path, s))
		{
			cout << CYAN"build display Index on" << endl;
			_serverMsg = buildIndexList(); 
			sendResponse(client_fd); // not defalt, it get index
		}
		else if (servers[s].redirection != "")
			redirect_Response(client_fd, servers[s].redirection);
		else if (checkDirectoryExist(full_path))
			sendResponse(client_fd);
		else
			sendCustomResponse(client_fd, full_path);
	}
	else if (method_type == "POST")
	{
		// LOCAL VARIABLE? NEED TO USE UR REQ'S CLIENTREQUEST
		cout << "server client_request:\n" << req.getRequest() << endl;
		// how do i get request here or you save request body?
		if  (isIndexOn(uri_path, s))
			sendErrorResponse(client_fd, 500);
		cout << GREEN << "METHOD_TYPE: " << YELLOW << method_type << RESET << endl; 
		sendCustomResponse(client_fd, full_path);
	}
	else if (method_type == "DELETE")
	{
		if  (isIndexOn(uri_path, s))
			sendCustomErrorResponse(client_fd, 500, s, root_path);
		else
		{
			int pos = uri_path.find("/");
			string newfile = uri_path.substr(pos);
			string folderpath = uri_path.substr(0, pos);
			root_path = getLocationRoot(folderpath, s);
			if (root_path == "")
				root_path = servers[s].root;
			full_path = root_path + newfile;
			deleteFile(full_path.c_str());
			cout << GREEN << "METHOD_TYPE: " << YELLOW << method_type << RESET << endl; 
			sendCustomResponse(client_fd, full_path);
		}
	}
	else
		sendErrorResponse(client_fd, 400);
}

void	Server::checkFullPath(const string & s_uri, const int & svr_id,
		string & root_path, string & full_path)
{
	int pos = s_uri.find("/");
	string d_url = s_uri.substr(0, pos);
	string file_path = s_uri.substr(pos + 1, s_uri.length());
	if (pos == -1)
		file_path = "";
	string newslash = addslash(d_url);

	string new_root = servers[svr_id].root;
	if (((new_root = getLocationRoot(d_url, svr_id)) == ""))
		new_root = servers[svr_id].root;
	string new_path = new_root + file_path;
	if (root_path != new_root)
		full_path = new_path;
}

int		Server::isCgiRequest(const string & s_uri, const int & svr_id, string & cgi_path)
{
	map<string, string>::iterator it, ite;
	string newslash = addslash(s_uri);

	ite = servers[svr_id].urlCgi.end();
	it = servers[svr_id].urlCgi.find(newslash);
	if (it != ite)
	{
		cgi_path = it->second;
		return (1);
	}
	return (0);
}

string	Server::getLocationRoot(const string & s_uri, const int & svr_id)
{
	map<string, string>::iterator it, ite;
	string newslash = addslash(s_uri);

	if (newslash[newslash.length() - 1] == '/')
		newslash.pop_back();
	ite = servers[svr_id].urlRoot.end();
	it = servers[svr_id].urlRoot.find(newslash);
	if (it != ite)
	{
		cout << RED << it->second << RESET << endl;
		if (it->second[it->second.length() - 1] != '/')
			return (it->second + "/");
		else
			return (it->second);
	}
	return ("");
}

string	Server::addslash(const string & s_uri)
{
	return ("/" + s_uri);
}

int		Server::isIndexOn(const string & s_uri, const int & svr_id)
{
	vector<string>::iterator it, ite;
	string newslash = addslash(s_uri);

	ite = servers[svr_id].urlIndexOn.end();
	it = servers[svr_id].urlIndexOn.begin();
	if (::find(it, ite, newslash) != ite)
		return (1);
	return (0);
}

int	Server::isAllowUrlMethod(const string & s_uri, const int & svr_id, string & method_type)
{
	map<string, string>::iterator it, ite;
	string newslash = addslash(s_uri);
	vector<string> method_list;
	vector<string>::iterator vit, vite;

	it = servers[svr_id].urlLimitExcept.find(newslash);
	ite = servers[svr_id].urlLimitExcept.end();
	if (it != ite)
	{
		cout << it->first << " --- " << it->second << endl;
		string tmp = it->second;
		while (tmp.find(" ") != string::npos)
		{
			string sub = tmp.substr(0, tmp.find(" "));
			tmp = tmp.substr(tmp.find(" ") + 1, tmp.length());
			method_list.push_back(sub);
		}
		method_list.push_back(tmp);
	}
	else
		return (1);
	vit = method_list.begin(), vite = method_list.end();
	while (vit != vite)
	{
		if (*vit++ == method_type)
		{
			cout << method_type << "is allowed!" << endl;
			return (1);
		}
	}
	cout << method_type << " is not allowed sad" << endl;
	return (0);
}

int		Server::isMethod(string & method_type)
{
	if (method_type == "GET" || method_type == "POST" || method_type == "DELETE")
		return (1);
	return (0);
}

void	Server::sendCustomResponse(int client_fd, string & full_path)
{
	stringstream	tmp;
	ostringstream	ss;
	int				bytesSent;
	string			html_content;

	ifstream file(full_path);
	tmp << file.rdbuf();
	html_content = tmp.str();

	// you get in server reason swithc? if success all 200. here dont handle error, if file not exits?
	ss << "HTTP/1.1 200 Ok\r\n"
	   << "Content-Type: text/html\r\n"
	   << "Content-Length: " << html_content.size()
	   << "\r\n\r\n"
	   << html_content;

	_serverMsg = ss.str();
	bytesSent = send(client_fd, _serverMsg.c_str(), _serverMsg.size(), 0);
	if (bytesSent == -1)
		N_MY::msg("Error sending response to client");
	else if (bytesSent == 0)
		N_MY::msg("Server closed the connection with the client");
	else
		N_MY::msg("Server sent a response to the client\n\n");
}

int		Server::isLocationExist(int const & svr_id, const string & s_uri)
{
	vector<string>::iterator it, ite;

	int pos = s_uri.find("/");
	string d_url = s_uri.substr(0, pos);
	string file_path = s_uri.substr(pos + 1, s_uri.length());
	if (pos == -1)
		file_path = "";
	string newslash = addslash(d_url);
	
	string root_path = servers[svr_id].root;
	if (((root_path = getLocationRoot(d_url, svr_id)) == ""))
		root_path = servers[svr_id].root;

	string full_path = root_path + file_path;
	
	if (s_uri == "")
		return (1);
	it = servers[svr_id].urlLocation.begin();
	ite = servers[svr_id].urlLocation.end();
	if (::find(it, ite, newslash) != ite && checkPathExist(full_path))
		return (1);
	return (0);
}

void	Server::sig_handler(int signo)
{
	cout << "Ah you click signal " << signo << endl;
	server_instance->closeServer();
	exit(0);
}
