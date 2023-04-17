/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: steh <steh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 17:51:13 by leng-chu          #+#    #+#             */
/*   Updated: 2023/04/17 12:40:09 by leng-chu         ###   ########.fr       */
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

Server::Server(vector<Server_Detail> & d_servers)
	: servers(d_servers), total(d_servers.size()), _sockfds(total),
	_clientfd(), _serverMsg(buildResponse()), _socketAddrs(total), 
	_socketAddr_len(sizeof(_socketAddrs[0])), _pwd(getenv("PWD")), _host(),
	_one_mb(ONE_MB), _limit_size(_one_mb), _index(0),
	tracker(new vector<int>[total]), s_t()
{
	checkServers(servers);
	vector<Server_Detail>::iterator it, ite;
	it = servers.begin();
	ite = servers.end();
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
	server_instance = this;
}

Server::~Server()
{
	cout << RED << "Server close" << RESET << endl;
	delete [] tracker;
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
	//main loop
	while (1)
	{
		usleep(2000);
		int rv = poll(&fds[0], fds.size(), -1);

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

void generate_listing(t_server *s_t, string &listing)
{
	DIR				*dir;
	struct dirent	*ent;

	if ((dir = opendir(s_t->full_path.c_str())) != NULL)
	{
	    listing += "<p>Path: " + s_t->full_path + "</p>";
	    while ((ent = readdir(dir)) != NULL)
	    {
			string name = ent->d_name;
			if (name == "." || name == "..")
				continue;
			string link = s_t->client_uri;
			if (s_t->client_uri[s_t->client_uri.length() - 1] != '/')
				link += "/";
			link += name;
			listing += "<li><a href=\"" + string("http://") + s_t->hostname + "/" + link + "\">" + name + "</a></li>";
	    }
	    closedir(dir);
	}
	 else
	     cerr << "Error opening directory: " << s_t->full_path << endl;
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
	ostringstream	ss;
	string			html_content = "";
	string			c;
	string			fpath;
	int				ff = 0;

	ss << "HTTP/1.1 200 OK\r\n"
	   << "Content-Type: text/html\r\n";
	c = s_t.client_uri;
	if (c[c.length() - 1] == '/')
		c.pop_back();
	if (c != s_t.new_uri)
	{
		string ffile = c.substr(c.rfind("/") + 1, c.length());
		fpath = s_t.full_path + ffile;
		if (checkFileExist(fpath) || checkFileExist(s_t.full_path))
		{
			ff = 1;
			ifstream file;
			if (checkFileExist(s_t.full_path))
				file.open(s_t.full_path);
			else
				file.open(fpath);
			stringstream tmp;
			tmp << file.rdbuf();
			html_content = tmp.str();
			ss << "Content-Length: " << html_content.size()
			   << "\r\n\r\n"
			   << html_content;
			file.close();
		}
	}
	if (html_content == "" && ff == 0)
	{
		string htmlFile = "<html><body>INDEX FILE<ul>";
		generate_listing(&s_t, htmlFile);
		htmlFile += "</ul></body></html>";
		ss << "Content-Length: " << htmlFile.size()
		   << "\r\n\r\n"
		   << htmlFile;
	}
	return ss.str();
}

void Server::redirect_Response(const int client_fd, const string & url)
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

void Server::sendResponse(const int client_fd)
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

void Server::sendErrorResponse(const int client_fd, int statuscode)
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
	bytesSent = send(client_fd, response.c_str(), response.size(), 0);
	if (bytesSent == (long)response.size())
		N_MY::msg("---- Server Error Response sent to client ---- \n\n");
	else if (bytesSent == -1)
		N_MY::msg("Error sending response to client");
	else
		N_MY::msg("Server closed the connection with the client");
}

int	Server::sendCustomErrorResponse(const int client_fd, int statuscode)
{
	map<int, string>::iterator	it, ite;
	stringstream				tmp;
	ostringstream				ss;
	int							bytesSent;
	string						full_path, erroroot, html_content, statusMessage;

	erroroot = s_t.root_path;
	it = servers[s_t.s].errorPageMap.find(statuscode);
	ite = servers[s_t.s].errorPageMap.end();
	if (it != ite)
	{
		if (s_t.root_path[s_t.root_path.length() - 1] == '/' && it->second[0] == '/')
			erroroot.pop_back();
		full_path = erroroot + it->second;
		if (!checkFileExist(full_path))
		{
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
		file.close();
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
	ifstream	ifile(filepath);
	char 		buffertest[BUF_SIZE + 100000];
	
	memset(buffertest, 0, sizeof(buffertest));
	if (ifile.good()) 
	{
		ifile.read(buffertest, sizeof(buffertest));
		if (ifile.gcount() > 0)
			buffertest[ifile.gcount()] = '\0';
		ifile.close();
		return (1);
	}
	ifile.close();
	return (0);
}

int	Server::checkDirectoryExist(const string & filepath)
{
	ifstream	ifile(filepath);
	struct stat	file_stat;

	if (stat(filepath.c_str(), &file_stat) == 0)
	{
		if (S_ISDIR(file_stat.st_mode))
		{
			_serverMsg = buildResponse();
			ifile.close();
			return (1);
		}
	}
	ifile.close();
	return (0);
}

int	Server::checkFileExist(const string & filepath)
{
	ifstream	ifile(filepath);
	char 		buffertest[FILEBUF + 1];
	struct stat file_stat;
	
	if (stat(filepath.c_str(), &file_stat) == 0)
	{
		if (!S_ISDIR(file_stat.st_mode)) // only check is not directory
		{
			memset(buffertest, 0, FILEBUF + 1);
			if (ifile.good()) 
			{
				ifile.read(buffertest, FILEBUF);
				if (ifile.fail() && !ifile.eof())
				{
            		cerr << "Error: File Error" << endl;
					ifile.close();
            		return (0);
        		}
				if (ifile.gcount() > 0)
					buffertest[FILEBUF] = '\0';
				ifile.close();
				return (1);
			}
		}
	}
	ifile.close();
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

int	Server::getServerPoll(const int & client_fd)
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
			removeClientPoll(fds, i--);
		else if (fds[i].revents & POLLIN)
		{ 
			// recv
			total_bytes = unChunkRequest(fds[i].fd, clientRequest);
			if (total_bytes > 0)
			{
				// get url from client
				getHostUrl(clientRequest);
				if (total_bytes <= _limit_size)
				{
					setMethodUrl(method_type, uri_path, clientRequest);
					string cookies = extractCookies(clientRequest);
					Request req(clientRequest, uri_path, cookies);
					//send
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

int		Server::unChunkRequest(const int client_fd, string & clientBuffer)
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
	string	search = "Host: ";
	_host.clear();
	if (clientRequest.find(search) != string::npos)
	{
		pos = clientRequest.find(search);
		tmp = clientRequest.substr(pos + search.length(), clientRequest.size());
		_host = tmp.substr(0, tmp.find("\n"));
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

void	Server::copyFiles(string &file_path, string &root_path)
{
	size_t pos = file_path.find_last_of('/');
	string filename = (pos == string::npos) ? file_path : file_path.substr(pos + 1);
	cout << "file_path: " << file_path << endl;
	cout << "filename: " << filename << endl;
	root_path += filename;
	FILE* source = fopen(file_path.c_str(), "rb");
	FILE* dest = fopen(root_path.c_str(), "ab");
	if (dest == NULL)
	{
		cout << "dest is NULL" << endl;
		return ;
	}
	if (source == NULL)
	{
		cout << "source is NULL" << endl;
		return ;
	}
	if (source == NULL || dest == NULL)
	{
		cout << "Failed to open files for copying.\n" << endl;
	 	return ;	
	}
	fseek(source, 0, SEEK_END);
	long file_size = ftell(source);
	fseek(source, 0L, SEEK_SET);
	char* buffer = new char[file_size * sizeof(char)];
	size_t nread;
	while ((nread = fread(buffer, 1, file_size, source)) > 0)
		fwrite(buffer, 1, nread, dest);
	delete [] buffer;
	fclose(source);
	fclose(dest);
}

void	Server::cleanServer(void)
{
	s_t.cgi_path.clear();
	s_t.root_path.clear();
	s_t.full_path.clear();
	s_t.index_file.clear();
	s_t.new_uri.clear();
	s_t.client_uri.clear();
	s_t.hostname.clear();
	s_t.s = -1;
	s_t.method_type.clear();
}

void	Server::initServer(const int & client_fd, const string & uri_path, const string & method_type)
{
	s_t.s = getServerPoll(client_fd);
	s_t.index_file = servers[s_t.s].index;
	s_t.cgi_path = "";
	s_t.client_uri = uri_path;
	s_t.hostname = _host;
	s_t.method_type = method_type;
	if (s_t.index_file == "")
		s_t.index_file = "index.html";
	if (((s_t.root_path = getLocationRoot(uri_path, s_t.s)) == ""))
		s_t.root_path = servers[s_t.s].root;
	int len = s_t.root_path.length() - 1;
	if (s_t.root_path[len] != '/')
		s_t.root_path += "/";
	s_t.full_path = s_t.root_path + s_t.index_file;
}

void	Server::display(const string & uri_path)
{
	cout << YELLOW << "method_type: " << s_t.method_type << RESET << endl;
	cout << YELLOW << "servername: " << servers[s_t.s].serverName << RESET << endl;
	cout << YELLOW << "uri_path: " << uri_path << RESET << endl;
	cout << YELLOW << "root_path: " << s_t.root_path << RESET << endl;
	cout << YELLOW << "index filename: " << s_t.index_file << RESET << endl;
	cout << YELLOW << "server poll id: " << s_t.s << RESET << endl;
	cout << YELLOW << "AutoIndex: "; isIndexOn(uri_path, s_t.s) ? cout << "ON" : cout << "OFF"; cout << endl;
	cout << YELLOW << "Is Location Existing?: "; isLocationExist(s_t.s, uri_path) ? cout << "YES" : cout << "NOT EXIST"; cout << endl;
	cout << RESET << endl;
	cout << YELLOW << "Full path: " << s_t.full_path << RESET << endl;
	cout << "======Before enter Method=====" << endl;
}


void	Server::sendClient(const int & client_fd, string & method_type,
		const string & uri_path, Request & req)
{
	cleanServer();
	initServer(client_fd, uri_path, method_type);
	//display(uri_path);

	int markCgi = markCgiRequest(uri_path);

	if (!isMethod(method_type))
		sendCustomErrorResponse(client_fd, 400);
	else if (!isAllowUrlMethod(uri_path, s_t.s, method_type))
		sendCustomErrorResponse(client_fd, 405);
	else if ((s_t.root_path == "" || !isLocationExist(s_t.s, uri_path)) && !markCgi)
		sendCustomErrorResponse(client_fd, 404);
	else if (isCgiRequest(uri_path, s_t.s, s_t.cgi_path) || markCgi)
		req.handle_cgi2(client_fd, s_t.cgi_path);
	else if (!checkFileExist(s_t.full_path) && uri_path != "" && !(isIndexOn(uri_path, s_t.s)))
		sendCustomErrorResponse(client_fd, 404);
	else if (method_type == "GET")
		sendGET(client_fd, uri_path);
	else if (method_type == "POST")
	{
		if  (isIndexOn(uri_path, s_t.s))
			sendCustomErrorResponse(client_fd, 500);
		else
		{
			map<string, string> key_value_body = req.getKeyValueBody();
			map<string, string>::iterator it = key_value_body.begin();
			if (it->first == "file" && checkFileExist(it->second))
				copyFiles(it->second, s_t.root_path);
			else if (!checkFileExist(it->second))
				sendCustomErrorResponse(client_fd, 400);
			else
			{
				_store_body.insert(key_value_body.begin(), key_value_body.end());
				sendCustomPostResponse(client_fd, s_t.full_path, _store_body);
			}
		}
	}
	else if (method_type == "DELETE")
		sendDELETE(client_fd, uri_path);
	else
		sendErrorResponse(client_fd, 400);
}

void	Server::checkFullPath(string & s_uri, const int & svr_id,
		string & root_path, string & full_path, const string & indexfile)
{
	int pos = s_uri.find("/");
	string d_url = s_uri.substr(0, pos);
	string file_path = s_uri.substr(pos + 1, s_uri.length());
	if (pos == -1)
		file_path = "";
	if (pos != -1)
		s_uri = s_uri.substr(0, pos);
	string newslash = addslash(d_url);
	string new_root = servers[svr_id].root;
	if (((new_root = getLocationRoot(d_url, svr_id)) == ""))
		new_root = servers[svr_id].root;
	string new_path = new_root + file_path;
	if (root_path != new_root)
		full_path = new_path;
	else if (file_path != indexfile)
		full_path = new_path;
}

int		Server::markCgiRequest(const string & s_uri)
{
	int pos = s_uri.find("/");
	int slash = 0;
	string new_key = s_uri;
	if (pos >= 0)
	{
		new_key = new_key.substr(0, pos);
		slash = 1;
	}
	if (isCgiRequest(new_key, s_t.s, s_t.cgi_path))
	{
		if (slash)
			new_key += "/";
		if (new_key == s_uri)
			return (1);
		new_key = s_t.client_uri.substr(new_key.length(), s_t.client_uri.length());
		s_t.root_path = "cgi-bin/";
		s_t.full_path = s_t.root_path + new_key;
		if (!checkFileExist(s_t.full_path))
			s_t.cgi_path = "error.cgi";
		else
			s_t.cgi_path = s_t.full_path.substr(s_t.full_path.find("/") + 1, s_t.full_path.length());
		return (1);
	}
	return (0);
}

int		Server::isCgiRequest(const string & s_uri, const int & svr_id, string & cgi_path)
{
	map<string, string>::iterator it, ite;
	string new_uri = s_uri;

	if (new_uri.find("?") != string::npos)
		new_uri = new_uri.substr(0, new_uri.find("?"));
	string newslash = addslash(new_uri);

	if (newslash[newslash.length() - 1] == '/')
		newslash.pop_back();

	ite = servers[svr_id].urlCgi.end();
	it = servers[svr_id].urlCgi.find(newslash);
	if (it != ite)
	{
		if (new_uri.find("?") != string::npos)
			cgi_path = s_uri;
		else
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
	string new_key = s_uri;
	int pos = s_uri.find("/");
	if (pos >= 0)
		new_key = s_uri.substr(0, pos);
	string newslash = addslash(new_key);

	ite = servers[svr_id].urlIndexOn.end();
	it = servers[svr_id].urlIndexOn.begin();
	if (::find(it, ite, newslash) != ite)
		return (1);
	return (0);
}

int	Server::isAllowUrlMethod(const string & s_uri, const int & svr_id, string & method_type)
{
	map<string, string>::iterator it, ite;
	int pos = s_uri.find("/");
	string new_key = s_uri;
	if (pos >= 0)
		new_key = s_uri.substr(0, pos);
	string newslash = addslash(new_key);
	vector<string> method_list;
	vector<string>::iterator vit, vite;

	it = servers[svr_id].urlLimitExcept.find(newslash);
	ite = servers[svr_id].urlLimitExcept.end();
	if (it != ite)
	{
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
			return (1);
	}
	return (0);
}

int		Server::isMethod(string & method_type)
{
	if (method_type == "GET" || method_type == "POST" || method_type == "DELETE")
		return (1);
	return (0);
}

void	Server::sendCustomPostResponse(const int client_fd, string & full_path, map<string, string> & key_value_body)
{
	stringstream	tmp;
	ostringstream	ss;
	int				bytesSent;
	string			html_content;

	ifstream file(full_path);
	tmp << file.rdbuf();
	html_content = tmp.str();

	html_content += "<html><body><h1>Posted example</h1>";
	map<string, string>::iterator it;
	for (it = key_value_body.begin(); it != key_value_body.end(); it++)
	{
		html_content += "<p>" + it->first + ": " + it->second + "</p>";
	}
	html_content += "</body></html>";
	ss 
	<< "HTTP/1.1 200 OK\r\n"
	<< "Content-Type: text/html\r\n"
	<< "Content-Length: " << html_content.size()
	<< "\r\n\r\n"
	<< html_content;

	file.close();
	_serverMsg = ss.str();
	bytesSent = send(client_fd, _serverMsg.c_str(), _serverMsg.size(), 0);
	if (bytesSent == -1)
		N_MY::msg("Error sending response to client");
	else if (bytesSent == 0)
		N_MY::msg("Server closed the connection with the client");
	else
		N_MY::msg("Server sent a response to the client\n\n");
}

void	Server::sendCustomPostResponse(const int client_fd, string & full_path, multimap<string, string> & key_value_body)
{
	stringstream	tmp;
	ostringstream	ss;
	int				bytesSent;
	string			html_content;
	ifstream file(full_path);
	tmp << file.rdbuf();
	html_content = tmp.str();
	html_content += "<html><body>";
    html_content += "<section>";
    html_content += "<h1>POST Request Received!</h1>\n";
    html_content += "<p>The following key-value pairs were received:</p>\n";
    html_content += "<ul>\n";

	multimap<string, string>::iterator it;
	for (it = key_value_body.begin(); it != key_value_body.end(); it++)
	{
        html_content += "<li>" + it->first + ": " + it->second + "</li>\n";
	}
	html_content += "</ul>\n";
    html_content += "</section>";
	html_content += "</body></html>";

	ss 
	<< "HTTP/1.1 200 OK\r\n"
	<< "Content-Type: text/html\r\n"
	<< "Content-Length: " << html_content.size()
	<< "\r\n\r\n"
	<< html_content;

	_serverMsg = ss.str();
	bytesSent = send(client_fd, _serverMsg.c_str(), _serverMsg.size(), 0);
	file.close();
	if (bytesSent == -1)
		N_MY::msg("Error sending response to client");
	else if (bytesSent == 0)
		N_MY::msg("Server closed the connection with the client");
	else
		N_MY::msg("Server sent a response to the client\n\n");
}


void	Server::sendCustomResponse(const int client_fd, string & full_path)
{
	stringstream	tmp;
	ostringstream	ss;
	int				bytesSent;
	string			html_content;

	ifstream file(full_path);
	tmp << file.rdbuf();
	html_content = tmp.str();

	ss << "HTTP/1.1 200 Ok\r\n"
	   << "Content-Type: text/html\r\n"
	   << "Content-Length: " << html_content.size() << "\r\n";
	if (servers[s_t.s].serverName != "")
		ss << "Host: " << servers[s_t.s].serverName;
	ss << "\r\n\r\n"
	   << html_content;

	_serverMsg = ss.str();
	bytesSent = send(client_fd, _serverMsg.c_str(), _serverMsg.size(), 0);
	file.close();
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
	string	new_uri = s_uri;

	if (new_uri.find("?") != string::npos)
		new_uri = new_uri.substr(0, new_uri.find("?"));

	int pos = new_uri.find("/");
	string d_url = new_uri.substr(0, pos);
	string file_path = new_uri.substr(pos + 1, new_uri.length());
	if (pos == -1)
		file_path = "";
	string newslash = addslash(d_url);
	
	string root_path = servers[svr_id].root;
	if (((root_path = getLocationRoot(d_url, svr_id)) == ""))
		root_path = servers[svr_id].root;

	string full_path = root_path;
	if (root_path[root_path.length() - 1] != '/' && file_path[file_path.length() - 1] != '/')
		full_path += "/";
	full_path += file_path;

	if (new_uri == "")
		return (1);
	it = servers[svr_id].urlLocation.begin();
	ite = servers[svr_id].urlLocation.end();
	if (::find(it, ite, newslash) != ite && checkPathExist(full_path))
		return (1);
	if (s_t.method_type == "DELETE")
		return (1);
	return (0);
}

bool is_digits(const string & str)
{
    return (str.find_first_not_of("0123456789") == string::npos);
}

int		Server::checkPort(vector<Server_Detail> & servers, string & defaultport)
{
	vector<Server_Detail>::iterator it, ite;

	it = servers.begin();
	ite = servers.end();
	int findefault = 0;
	while (it != ite)
	{
		if ((it->port == "" || defaultport == it->port || !is_digits(it->port)) && findefault)
		{
			servers.erase(it);
			total--;
			return (1);
		}
		else if (defaultport == "")
			defaultport = it->port;
		if (defaultport == it->port && findefault == 0)
			findefault = 1;
		it++;
	}
	return (0);
}

int		Server::isDupPort(void)
{
	vector<Server_Detail>::iterator it, it2, ite;
	string	d_port;

	it = servers.begin(), ite = servers.end();
	while (it != ite)
	{
		d_port = it->port;
		it2 = it;
		while (++it2 != ite)
			if (d_port == it2->port)
				return (1);
		it++;
	}
	return (0);
}

void	Server::checkServers(vector<Server_Detail> & servers)
{
	string	defaultport = "";

	if (isDupPort())
		N_MY::ErrorExit("Duplicate Ports!!");
	while (checkPort(servers, defaultport)) ;
}

void	Server::sendGET(const int & client_fd, const string & uri_path)
{
	s_t.new_uri = uri_path;
	checkFullPath(s_t.new_uri, s_t.s, s_t.root_path, s_t.full_path, s_t.index_file);
	if (checkDirectoryExist(s_t.full_path))
	{
		if (s_t.full_path[s_t.full_path.length() - 1] != '/')
			s_t.full_path += "/";
		s_t.full_path += s_t.index_file;
	}
	if  (isIndexOn(s_t.new_uri, s_t.s))
	{
		int pos = s_t.full_path.find(s_t.index_file);
		s_t.full_path = s_t.full_path.substr(0, pos);
		_serverMsg = buildIndexList(); 
		sendResponse(client_fd);
	}
	else if (servers[s_t.s].redirection != "")
		redirect_Response(client_fd, servers[s_t.s].redirection);
	else if (checkFileExist(s_t.full_path))
		sendCustomResponse(client_fd, s_t.full_path);
	else
		sendCustomErrorResponse(client_fd, 404);
}

void	Server::sendDELETE(const int & client_fd, const string & uri_path)
{
	if  (isIndexOn(uri_path, s_t.s))
		sendCustomErrorResponse(client_fd, 500);
	else
	{
		string newfile = uri_path;
		int pos = uri_path.find("/");
		if (pos >= 0)
			newfile = uri_path.substr(pos);
		string folderpath = uri_path.substr(0, pos);
		s_t.root_path = getLocationRoot(folderpath, s_t.s);
		if (s_t.root_path == "")
			s_t.root_path = servers[s_t.s].root;
		s_t.full_path = s_t.root_path + newfile;
		deleteFile(s_t.full_path.c_str());
		sendCustomResponse(client_fd, s_t.full_path);
	}
}

void	Server::sig_handler(int signo)
{
	cout << "Ah you click signal " << signo << endl;
	server_instance->closeServer();
	exit(0);
}
