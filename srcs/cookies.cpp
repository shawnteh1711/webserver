#include "cookies.hpp"


Request::Request() : _request()
{
    return ;
}

Request::Request(const string& request, const string & cgi_path)
	: _request(request), _cgi_path(cgi_path)
{
    return ;
}


Request::Request(string& request, int num_read)
{
    setBuffer(request, num_read);
    return ;
}

void    Request::setBuffer(string& request, int num_read)
{
    _request = request.substr(0, num_read);
    _read_size = num_read;
    _total_read_size += num_read;
}

Request::~Request(void)
{
    return ;
}

bool    Request::isCgiRequest() const
{
    size_t method_pos;
    size_t content_type_pos;
    size_t content_type_value_pos;
    size_t uri_pos;
    size_t cgi_pos;

    cout << _request << endl;
    method_pos = _request.find("POST");
    if (method_pos == string::npos)
        return (false);
    
    content_type_pos = _request.find("Content-Type:");
    if (content_type_pos == string::npos)
        return (false);

    content_type_value_pos = _request.find("application/x-www-form-urlencoded");
    if (content_type_value_pos == string::npos)
        return (false);
    
    uri_pos = _request.find(" ");
    if (uri_pos == string::npos)
        return (false);
    
    string uri = _request.substr(uri_pos + 1);
    cgi_pos = uri.find(".cgi");
    if (cgi_pos == string::npos)
        return (false);
    return (true);
}

cgi_extension  getEntensionType(const string& extension)
{
    if (extension == ".php")
        return (PHP);
    else if (extension == ".cgi")
        return (CGI);
    else
        return (Unknown);
}

bool Request::is_cgi_request()
{       
    bool            val;
    string          uri;
    size_t          dot_pos;

    val = false;
	cout << CYAN << "Enter is cgi-request" << endl;
    uri = getRequestUrl();
    cout << "uri: " << uri << endl;
    dot_pos = uri.rfind('.');
    if (dot_pos != string::npos)
    {
        _extension = uri.substr(dot_pos);
        cout << "extension: " << _extension << endl;
        for (int i = 0; i < Unknown; ++i)
        {
            if (getEntensionType(_extension) == static_cast<cgi_extension>(i))
            {
                if (uri.find("/cgi-bin/") != std::string::npos) 
                    val = true;
                break;
            }
        }
    }
	cout << RESET << endl;
    cout << "val: " << val << endl;
    return (val);
}

string  Request::getRequestUrl() const
{
    size_t  url_pos;
    size_t  space_pos;
    size_t  query_pos;
    size_t  http_pos;
    string  url;
    size_t  header_end_pos;

    // cout << "Request:" << _request << endl;
    space_pos = _request.find(' ');
    if (space_pos != string::npos)
    {
        url_pos = _request.find('/', space_pos + 1);
        if (url_pos != string::npos)
        {
            header_end_pos = _request.find("\r\n");
            if (header_end_pos != string::npos)
            {
                // query_pos = _request.find('?');
                query_pos = _request.substr(0, header_end_pos).find('?');
                if (query_pos != string::npos)
                {
                    url = _request.substr(url_pos, query_pos - url_pos);
                }
                else
                {
                    http_pos = _request.find("HTTP/", url_pos);
                    if (http_pos != string::npos)
                        url = _request.substr(url_pos, http_pos - url_pos - 1);
                    else
                        url = _request.substr(url_pos);
                }
            }
        }
    }
    return (url);
}

string  Request::getMethod() const
{
    size_t  method_pos = _request.find(" ");
    if (method_pos == string::npos)
        return "";
    return (_request.substr(0, method_pos));
}

// string  Request::getQueryString() const
// {
//     size_t query_pos = _request.find("?");
//     if (query_pos == string::npos)
//         return "";

//     size_t end_query_pos = _request.find(" ", query_pos + 1);
//     if (end_query_pos == string::npos)
//         return "";

//     return _request.substr(query_pos + 1, end_query_pos - query_pos - 1);
// }

void    Request::setQuery(string& query)
{
    _query = query;
}

string  Request::getQueryString() const
{
    string  query;
    size_t query_pos = _request.find("?");
    if (query_pos == string::npos)
        return "";

    size_t end_query_pos = _request.find("\r\n", query_pos + 1);
    if (end_query_pos == string::npos)
        return "";

    query = _request.substr(query_pos + 1, end_query_pos - query_pos - 1);

    size_t http_pos = query.find(" HTTP/1.1");
    if (http_pos != string::npos)
        query.erase(http_pos);
    return (query);
}

string Request::getHeader(const string& header_name) const
{
    size_t header_start = _request.find(header_name + ":");
    if (header_start == string::npos)
        return "";

    header_start += header_name.length() + 1;

    size_t header_end = _request.find("\r\n", header_start);
    if (header_end == string::npos)
        return "";

    return _request.substr(header_start, header_end - header_start);
}

string Request::getAddress() const
{
    size_t host_start = _request.find("Host:");
    if (host_start == string::npos)
        return "";

    host_start += 6;

    size_t host_end = _request.find("\r\n", host_start);
    if (host_end == string::npos)
        return "";

    string host = _request.substr(host_start, host_end - host_start);

    size_t port_pos = host.find(":");
    if (port_pos != string::npos)
    {
        host = host.substr(0, port_pos);
    }

    return host;
}

string  Request::getExtension() const
{
    return (_extension);
}

string Request::getCgiPath() const
{
    return (_cgi_path);
}

int     Request::getTotalReadSize() const
{
    return (_total_read_size);
}
int     Request::getReadSize() const
{
    return (_read_size);
}
string  Request::getRequest() const
{
    return (_request);
}

string    Request::setCgiPath(string cgi_path)
{
    _pwd = getenv("PWD");
    const char* cgi_bin_path = "/cgi-bin/";
    cgi_path = _pwd + cgi_bin_path + this->parseCgiPath();
    // cout << GREEN << "cgi_path: " << cgi_path << endl;
    // cout << "_cgi_path: " << _cgi_path << endl;
    // cout << RESET << endl;
    _cgi_path = cgi_path;
    return (_cgi_path);
}

void    Request::setRequest(string request)
{
    _request = request;
}

void    Response::setHeader(const string& name, const string& value)
{
    _headers[name] = value;
    map<string, string>::const_iterator it;

    for (it = _headers.begin(); it != _headers.end(); ++it)
    {
        cout << it->first << ": " << it->second << "\r\n";
    }
}

void    Response::printCookies() const
{
    map<string, string>::const_iterator   it;
    for (it = _headers.begin(); it != _headers.end(); ++it)
    {
        if (it->first.substr(0, 11) == "Set-Cookie")
            cout << it->second << endl;
    }
    
}

string Request::parseCgiPath()
{
    string  cgi_path;
    size_t  header_end;
    size_t  path_start;
    size_t  path_end;

    path_start = _request.find(" ");
    header_end = _request.find("\r\n");
    path_end = _request.substr(0, header_end).find('?');
    // path_end = _request.find("?", path_start + 1);
    if (path_end == string::npos)
        path_end = _request.find(" ", path_start + 1);
    if (path_end == string::npos)
        return "";
    string path = _request.substr(path_start + 1, path_end - path_start - 1);
    if (path.find("/cgi-bin/") != string::npos)
    {
        cgi_path = path.substr(9); // Skip "/cgi-bin/" prefix
        if (cgi_path.back() == '/')
            cgi_path.pop_back();
    }
    else
    {
        cout << "Request is not a CGI request" << endl;
    }
    // cout << "cgi_path: " << cgi_path << endl;
    return (cgi_path);
}

void    Response::setStatusCode(int code)
{
    _status_code = code;
}
void    Response::setContentType(const string& type)
{
    _content_type = type;
}
void    Response::setContent(const char* content, int length)
{
    _content = string(content, length);
}

string  Response::restoString() const
{
    map<string, string>::const_iterator it;

    ostringstream   oss;
    oss << "HTTP/1.1 " << _status_code << " " << getReasonPhrase(_status_code) << "\r\n";
    oss << "Content-Type: " << _content_type << "\r\n";
    oss << "Content-Length: " << _content.length() << "\r\n";
    for (it = _headers.begin(); it != _headers.end(); ++it)
    {
        oss << it->first << ": " << it->second << "\r\n";
    }
    oss << "\r\n";
    oss << _content;
    return (oss.str());
}

string   Response::getReasonPhrase(int code)
{
    switch (code)
    {
        case 200: return "OK";
        case 400: return "Bad Request";
        case 404: return "Not Found";
        case 500: return "Internal Server Error";
        default: return "Unknown Status";
    }
}

int create_server_socket()
{
    int server_socket;

    // struct sockaddr_in server_address;
    struct sockaddr_in server_address;

    // configure server address
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(80);

    // create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
        perror("socker create error");
    else
        cout << "socket created" << endl;

    int optval = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    else
        cout << "Set sockopt success" << endl;
    memset(server_address.sin_zero, '\0', sizeof(server_address.sin_zero));
    // bind socket to the specified address
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }

    // listen for incoming connections.
    if (listen(server_socket, SOMAXCONN) == -1)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }

    // accept a connection

    return (server_socket);
}

// int accept_connection(char* buffer, int server_socket, struct sockaddr_in client_address)
// {
//     int client_socket;

//     client_socket = 0;
//     socklen_t client_address_len = sizeof(client_address);
//     client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_len);
//     if (client_socket < 0)
//     {
//         perror("In accept");
//         exit(EXIT_FAILURE);
//     }
//     fill(buffer, buffer + BUFF_SIZE, 0);
//     read(client_socket, buffer, BUFF_SIZE);
//     cout << buffer << endl;
//     const char* hello = "hello from server";
//     write(client_socket, hello, strlen(hello));
//     return (client_socket);
// }

int accept_connection(int server_socket)
{
    int client_socket;
    struct sockaddr_in client_address;

    client_socket = 0;
    socklen_t client_address_len = sizeof(client_address);
    client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_len);
    if (client_socket < 0)
    {
        perror("In accept");
        exit(EXIT_FAILURE);
    }
    cout << "Client connected from " << inet_ntoa(client_address.sin_addr) << ":" << ntohs(client_address.sin_port) << endl;
    return (client_socket);
}

int Request::readRequest(int client_socket)
{
    string buffer;

    buffer.resize(BUFF_SIZE);
    int num_read = recv(client_socket, &buffer[0], BUFF_SIZE, 0);
    if (num_read == -1) {
        perror("read failed");
        exit(EXIT_FAILURE);
    } else if (num_read == BUFF_SIZE)
    {
        while (true)
        {
            char temp_buffer[BUFF_SIZE];
            int temp_read = recv(client_socket, temp_buffer, BUFF_SIZE, 0);
            if (temp_read == -1) {
                perror("large read failed");
                exit(EXIT_FAILURE);
            }
            buffer.append(temp_buffer, temp_read);
            num_read += temp_read;
            if (temp_read < BUFF_SIZE) {
                break;
            }
        }
    }
    this->setBuffer(buffer, num_read);
    return (0);
}

void Request::setEnvp()
{
    string request_method = "REQUEST_METHOD=" + this->getMethod();
    string query_string = "QUERY_STRING=" + this->getQueryString();
    string content_type = "CONTENT_TYPE=" + this->getHeader("Content-Type");
    string content_length = "CONTENT_LENGTH=" + this->getHeader("Content-Length");
    string remote_addr = "REMOTE_ADDR=" + this->getAddress();
    string script_name = "SCRIPT_NAME=" + this->parseCgiPath();
    string script_path = "SCRIPT_PATH=" + _cgi_path;

    // char** envp = (char**) malloc((ENV_SIZE + 1) * sizeof(char*));
    vector<char *> envp2(8);

    if (envp2.size() == 0)
    {
        cerr << "Error: Failed to allocate memory to envp." << endl;
        exit(EXIT_FAILURE);
    }
    // envp[0] = strdup(request_method.c_str());
    // envp[1] = strdup(query_string.c_str());
    // envp[2] = strdup(content_type.c_str());
    // envp[3] = strdup(content_length.c_str());
    // envp[4] = strdup(remote_addr.c_str());
    // envp[5] = strdup(script_name.c_str());
    // envp[6] = strdup(script_path.c_str());
    // envp[7] = NULL;

    envp2[0] = strdup(request_method.c_str());
    envp2[1] = strdup(query_string.c_str());
    envp2[2] = strdup(content_type.c_str());
    envp2[3] = strdup(content_length.c_str());
    envp2[4] = strdup(remote_addr.c_str());
    envp2[5] = strdup(script_name.c_str());
    envp2[6] = strdup(script_path.c_str());
    envp2[7] = NULL;

    // for (int i = 0; i < ENV_SIZE; i++)
    // {
    //     _envp[i] = envp[i];
    // }
    // _envp[ENV_SIZE - 1] = NULL;
    for (int i = 0; i < (int)envp2.size(); i++)
    {
        _envp[i] = envp2[i];
    }
    _envp[envp2.size() - 1] = NULL;
    // this->freeEnvp(envp);
}

void Request::freeEnvp(char **envp)
{
    for (int i = 0; i < ENV_SIZE; i++)
    {
        if (envp[i] != NULL)
        {
            free(envp[i]);
        }
    }
    free(envp);
}

char**  Request::getEnvp()
{
    return (_envp);
}

void    Request::printEnvp() const
{
    for (int i = 0; i < ENV_SIZE; i++) 
    {
        if (_envp[i] != NULL) 
        {
            cout << _envp[i] << endl;
        }
    }
}

char** Request::handleArgs(const string& cgi_path)
{
    vector<string>              commandArgs;
    char**                      args;
    int                         i;
    vector<string>::iterator    it;

    _extension_map[".cgi"].push_back(cgi_path);
    _extension_map[".php"].push_back("/usr/bin/php");
    if (_extension_map.find(this->getExtension()) == _extension_map.end())
    {
        cerr << "Error: No CGI program found for extension " << this->getExtension() << endl;
        exit(EXIT_FAILURE);
    }
    commandArgs = _extension_map[this->getExtension()];
    args = new char*[commandArgs.size() + 2];
    i = 0;
    for (it = commandArgs.begin(); it != commandArgs.end(); it++)
    {
        args[i] = const_cast<char*>(it->c_str());;
        i++;
    }
    args[i++] = const_cast<char*>(_cgi_path.c_str());
    args[i] = NULL;

    char** args_copy = new char*[i + 1];
    for (int j = 0; j < i; j++)
    {
        args_copy[j] = new char[strlen(args[j]) + 1];
        strcpy(args_copy[j], args[j]);
    }
    args_copy[i] = NULL;
    delete[] args;
    return (args_copy);
}

string    Response::getContent() const
{
    return (_content);
}

bool       Response::checkRequestCookies(Request& request)
{
    _cookies = request.getHeader("Cookie");
    if (_cookies.empty())
    {
        cout << "No cookies found" << endl;
        return (false);
    }
    else
    {
        cout << "Cookies found: " << _cookies << endl;
        return (true);
    }

}

string  Response::getRequestCookies(Request& request)
{
    return (request.getCookies());
}


void    Response::sendCgiResponse(Request& request, int client_socket, char *buffer, int count)
{
    Response    res;
    string      response_str;
    int         bytes_sent;

    res.setStatusCode(200);
    res.setContentType("text/html");
    if (this->checkRequestCookies(request))
        res.setHeader("Set-Cookie", this->getRequestCookies(request));
    // res.setContent(this->getRequest().c_str(), this->getReadSize());
    res.setContent(buffer, count);
    // res.printCookies();
    response_str = res.restoString();
    bytes_sent = send(client_socket, response_str.c_str(), response_str.size(), 0);
    if (bytes_sent == -1)
    {
        perror("send");
        exit(EXIT_FAILURE);
    }
    else if (bytes_sent != (int)response_str.size())
    {
        cerr << "Error: sent " << bytes_sent << " out of " << response_str.size() << " bytes" << endl;
        exit(EXIT_FAILURE);
    }
}

void    Response::sendErrorResponse(int client_socket, int status_code, string path)
{
    Response        res;
    stringstream    ss;
    string          html_content;
    string          response_str;
    int             bytes_sent;
    string          pwd;

    ifstream file(path);
    ss << file.rdbuf();
    html_content = ss.str();
    res.setStatusCode(status_code);
    res.setContentType("text/html");
    res.setContent(html_content.c_str(), html_content.size());
    response_str = res.restoString();
    bytes_sent = send(client_socket, response_str.c_str(), response_str.size(), 0);
    if (bytes_sent == -1)
    {
        perror("send");
        exit(EXIT_FAILURE);
    }
    else if (bytes_sent != (int)response_str.size())
    {
        cerr << "Error: sent " << bytes_sent << " out of " << response_str.size() << " bytes" << endl;
        exit(EXIT_FAILURE);
    }
}

void Request::readPipe(int count, char* buffer)
{
    if (count == -1)
    {
        perror("read");
        exit(EXIT_FAILURE);
    }
    else if (count == 0)
        cout << "No data read" << endl;
    else
    {
        cout << "Read " << count << " bytes" << endl;
        cout << "Data read: " << buffer << endl;
    }
}


int Request::handle_cgi(int client_socket)
{
    pid_t       pid;
    int         status;
    string      cgi_path;
    char**      args;
    int         pipes[2];
    Response    res;
    ssize_t     count;

    cgi_path = this->setCgiPath(cgi_path);
    if (pipe(pipes) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
		cout << GREEN << "ENTER CHILD PROCESS" << RESET << endl;
        close(pipes[0]);
        if (dup2(pipes[1], STDOUT_FILENO) == -1)
        {
            perror("dup2");
            exit(EXIT_FAILURE);
        }
        close(pipes[1]);
        this->setEnvp();
        args = handleArgs(cgi_path);
        if (execve(args[0], args, this->getEnvp()) == -1)
        {
            cerr <<  "Error: " << strerror(errno) << endl;
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        cout << GREEN << "ENTER PARENT PROCESS" << RESET << endl;
        if (waitpid(pid, &status, 0) == -1)
        {
            perror("waitpid failed");
            exit(EXIT_FAILURE);
        }
        if (WIFEXITED(status))
        {
            int         exit_status;
            struct stat file_stat;
            int         result;
            char        buffer[BUFF_SIZE];

            cout << "Child process exit status " << WIFEXITED(status) << endl;
            exit_status = WEXITSTATUS(status);
            close(pipes[1]);
            count = read(pipes[0], buffer, BUFF_SIZE);
            this->readPipe(count, buffer);
            result = stat(_cgi_path.c_str(), &file_stat);
            if (exit_status != 0)
            {
                if (result == -1)
                {
                    string root = "/root/404.html";
                    string path = _pwd + root;
                    res.sendErrorResponse(client_socket, 404, path);
                }
                else
                {
                    string root = "/root/500.html";
                    string path = _pwd + root;
                    res.sendErrorResponse(client_socket, 500, path);
                }
            }
            else
                res.sendCgiResponse(*this, client_socket, buffer, count);
        }
        // system("leaks webserv");
    }
    return (client_socket);
}

void handle_non_cgi(int client_socket, Request& req)
{
    cout << "Handling non CGI request" << endl;
    Response res;
    res.setStatusCode(200);
    res.setContentType("text/html");
    if (req.hasCookies())
        res.setHeader("Set-Cookie", req.getCookies());
    res.setContent(req.getRequest().c_str(), req.getReadSize());
    string response_str = res.restoString();
	cout << "response_str : " << response_str << endl;
    send(client_socket, response_str.c_str(), response_str.size(), 0);
	// non-cgi dont need pipe, mine works. 
}

//  curl -X POST -H "Content-Type: application/x-www-form-urlencoded" -d "name=shawn&age=30" http://localhost:8080/cgi-bin/hello.cgi

bool Request::hasCookies()
{
    _cookies = this->getHeader("Cookie");
    if (_cookies.empty())
    {
        cout << "No cookies found" << endl;
        return (false);
    }
    else
    {
        cout << "Cookies found: " << _cookies << endl;
        return (true);
    }
}

string  Request::getCookies() const
{
    return (_cookies);
}   


//  curl --cookie "name=shawn; name2=alec" http://localhost:80
//  curl -b "name=shawn; name2=alec" http://localhost:80
// curl -X POST -H "Content-Type: application/x-www-form-urlencoded" -d "name=shawn&age=30" "127.0.0.1/cgi-bin/hello.cgi?name=shawn&age=23&hobby=sport"
//int main()
//{
//   int     client_socket;
//   int     server_socket;
//   Request req;
//
//   server_socket = create_server_socket();
//   while (true)
//   {
//       cout << "+++++++Waiting for new connection+++++++" << endl;
//       client_socket = accept_connection(server_socket);
//       req.readRequest(client_socket);
//       req.hasCookies();
//       if (req.is_cgi_request())
//           client_socket = req.handle_cgi(client_socket);
//       else
//       {
//           handle_non_cgi(client_socket, req);
//       }
//       close(client_socket);
//    //    system("leaks a.out");
//       cout << "++++++++Done+++++++" << endl;
//   }
//   close(server_socket);
//}
