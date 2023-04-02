#include "cookies.hpp"


Request::Request() : _request()
{
    for (int i = 0; i < ENV_SIZE; i++)
        _envp[i] = NULL;
    return ;
}

Request::Request(const string& request) : _request(request)
{
    for (int i = 0; i < ENV_SIZE; i++)
        _envp[i] = NULL;
    return ;
}


Request::Request(string& request, int num_read)
{
    for (int i = 0; i < ENV_SIZE; i++)
        _envp[i] = NULL;
    setBuffer(request, num_read);
    return ;
}

void    Request::setBuffer(string& request, int num_read)
{
     for (int i = 0; i < ENV_SIZE; i++)
        _envp[i] = NULL;
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
    dot_pos = uri.rfind('.');
    if (dot_pos != string::npos)
    {
        _extension = uri.substr(dot_pos);
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
    return (val);
}

string  Request::getRequestUrl() const
{
    size_t  url_pos;
    size_t  space_pos;
    size_t  query_pos;
    size_t  http_pos;
    string  url;

    cout << "Request:" << _request << endl;
    space_pos = _request.find(' ');
    if (space_pos != string::npos)
    {
        url_pos = _request.find('/', space_pos + 1);
        if (url_pos != string::npos)
        {
            query_pos = _request.find('?');
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
    return (url);
}

string  Request::getMethod() const
{
    size_t  method_pos = _request.find(" ");
    if (method_pos == string::npos)
        return "";
    return (_request.substr(0, method_pos));
}

string  Request::getQueryString() const
{
    size_t query_pos = _request.find("?");
    if (query_pos == string::npos)
        return "";

    size_t end_query_pos = _request.find(" ", query_pos + 1);
    if (end_query_pos == string::npos)
        return "";

    return _request.substr(query_pos + 1, end_query_pos - query_pos - 1);
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

void    Request::setCgiPath(string path)
{
    _cgi_path = path;
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
    string cgi_path;

    size_t path_start = _request.find(" ");
    size_t path_end = _request.find("?", path_start + 1);
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

    char** envp = (char**) malloc((ENV_SIZE + 1) * sizeof(char*));
    if (envp == NULL)
    {
        cerr << "Error: Failed to allocate memory to _envp." << endl;
        exit(EXIT_FAILURE);
    }
    envp[0] = strdup(request_method.c_str());
    envp[1] = strdup(query_string.c_str());
    envp[2] = strdup(content_type.c_str());
    envp[3] = strdup(content_length.c_str());
    envp[4] = strdup(remote_addr.c_str());
    envp[5] = strdup(script_name.c_str());
    envp[6] = NULL;

    for (int i = 0; i < ENV_SIZE; i++)
    {
        _envp[i] = envp[i];
    }
    _envp[ENV_SIZE - 1] = NULL;
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

char** Request::handleArgs(const string& extension, const string& cgi_path)
{
    vector<string>              commandArgs;
    char**                      args;
    int                         i;
    vector<string>::iterator    it;

    _extension_map[".cgi"].push_back(cgi_path);
    _extension_map[".php"].push_back("/usr/bin/php");
    if (_extension_map.find(extension) == _extension_map.end())
    {
        cerr << "Error: No CGI program found for extension " << extension << endl;
        exit(EXIT_FAILURE);
    }
    commandArgs = _extension_map[extension];
    args = new char*[commandArgs.size() + 2];
    i = 0;
    for (it = commandArgs.begin(); it != commandArgs.end(); it++)
    {
        args[i] = const_cast<char*>(it->c_str());;
        i++;
    }
    args[i++] = const_cast<char*>(cgi_path.c_str());
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

int Request::handle_cgi(int client_socket)
{
    pid_t                       pid;
    int                         status;
    string                      cgi_path;
    string                      extension;
    char**                      args;
    // int                         pipes[2];

    // if (pipe(pipes) == -1)
    // {
    //     perror("pipe");
    //     exit(EXIT_FAILURE);
    // }
    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        // close(pipes[0]);
        // dup2(pipes[1], STDOUT_FILENO);
        // close(pipes[1]);
        this->setEnvp();
        const char* cgi_bin_path = "../cgi-bin/";
        string cgi_path = cgi_bin_path + this->parseCgiPath();
        extension = this->getExtension();
        args = handleArgs(extension, cgi_path);
        if (execve(args[0], args, this->getEnvp()) == -1)
        {
            cerr <<  "Error: " << strerror(errno) << endl;
            exit(EXIT_FAILURE);
        }
        else
            cout << "running execve" << endl;
    }
    else
    {   
        // close(pipes[1]);
        // char buffer[BUFSIZ];
        // ssize_t count = read(pipes[0], buffer, BUFSIZ);
        // if (count == -1)
        // {
        //     perror("read");
        //     exit(EXIT_FAILURE);
        // }
        // else if (count == 0)
        // {
        //     cout << "No data read" << endl;
        // }
        // else
        // {
        //     cout << "Read " << count << " bytes" << endl;
        //     cout << "Data read: " << buffer << endl;
        // }
        if (WIFEXITED(status))
        {
            cout << "Child process exit status " << WIFEXITED(status) << endl;
            
            // Send the HTTP response back to the client
            Response res;
            res.setStatusCode(200);
            res.setContentType("text/plain");
            if (this->hasCookies())
                res.setHeader("Set-Cookie", this->getCookies());
            res.setContent(this->getRequest().c_str(), this->getReadSize());
            // res.setContent(buffer, count);
            res.printCookies();
            string response_str = res.restoString();
            int bytes_sent;
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
        if (waitpid(pid, &status, 0) == -1)
        {
            perror("waitpid failed");
            exit(EXIT_FAILURE);
        }
    }
    return (client_socket);
}

void handle_non_cgi(int client_socket, Request& req)
{
    cout << "Handling non CGI request" << endl;
    Response res;
    res.setStatusCode(200);
    res.setContentType("text/plain");
    if (req.hasCookies())
        res.setHeader("Set-Cookie", req.getCookies());
    res.setContent(req.getRequest().c_str(), req.getReadSize());
    string response_str = res.restoString();
    send(client_socket, response_str.c_str(), response_str.size(), 0);
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
int main()
{
   int     client_socket;
   int     server_socket;
   Request req;

   server_socket = create_server_socket();
   while (true)
   {
       cout << "+++++++Waiting for new connection+++++++" << endl;
       client_socket = accept_connection(server_socket);
       req.readRequest(client_socket);
       req.hasCookies();
       if (req.is_cgi_request())
           client_socket = req.handle_cgi(client_socket);
       else
       {
           handle_non_cgi(client_socket, req);
       }
       close(client_socket);
    //    system("leaks a.out");
       cout << "++++++++Done+++++++" << endl;
   }
   close(server_socket);
}