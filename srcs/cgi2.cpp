#include "cgi2.hpp"


Request::Request() : _request()
{
    return ;
}

Request::Request(const string& request) : _request(request)
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
    string          extension;

    val = false;
    uri = getRequestUrl();
    dot_pos = uri.rfind('.');
    if (dot_pos != string::npos)
    {
        extension = uri.substr(dot_pos);
        for (int i = 0; i < Unknown; ++i)
        {
            if (getEntensionType(extension) == static_cast<cgi_extension>(i))
            {
                if (uri.find("/cgi-bin/") != std::string::npos) 
                    val = true;
                break;
            }
        }
     
    }
    return (val);
}

string  Request::getRequestUrl() const
{
    size_t  space_pos;
    size_t  query_pos;
    string  url;

    space_pos = _request.find(' ');
    if (space_pos != string::npos)
    {
        query_pos = _request.find('?');
        if (query_pos != string::npos)
            url = _request.substr(space_pos + 1, query_pos - space_pos - 1);
        else
            url = _request.substr(space_pos + 1);
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
    ostringstream   oss;
    oss << "HTTP/1.1 " << _status_code << " " << getReasonPhrase(_status_code) << "\r\n";
    oss << "Content-Type: " << _content_type << "\r\n";
    oss << "Content-Length: " << _content.length() << "\r\n";
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

int read_request(int client_socket, Request& req)
{
    string buffer;

    buffer.resize(BUFF_SIZE);
    int num_read = recv(client_socket, &buffer[0], BUFF_SIZE, 0);
    if (num_read == -1) {
        perror("read failed");
        exit(EXIT_FAILURE);
    } else if (num_read == BUFF_SIZE)
    {
        while (true) {
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
    req.setBuffer(buffer, num_read);
    return (0);
}

int handle_cgi(int client_socket, Request& req)
{
    pid_t   pid;
    char*   args[2];
    int     status;
    string  cgi_path;

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        string request_method = "REQUEST_METHOD=" + req.getMethod();
        string query_string = "QUERY_STRING=" + req.getQueryString();
        string content_type = "CONTENT_TYPE=" + req.getHeader("Content-Type");
        string content_length = "CONTENT_LENGTH=" + req.getHeader("Content-Length");
        string remote_addr = "REMOTE_ADDR=" + req.getAddress();
        string script_name = "SCRIPT_NAME=" + req.parseCgiPath();

        char *envp[] = {
            const_cast<char*>(request_method.c_str()),
            const_cast<char*>(query_string.c_str()),
            const_cast<char*>(content_type.c_str()),
            const_cast<char*>(content_length.c_str()),
            const_cast<char*>(remote_addr.c_str()),
            const_cast<char*>(script_name.c_str()),
            NULL
        };

        const char* cgi_bin_path = "../cgi-bin/";
        string cgi_path = cgi_bin_path + req.parseCgiPath();
        args[0] = const_cast<char*>(cgi_path.c_str());
        args[1] = NULL;
        if (execve(args[0], args, envp) == -1)
        {
            cerr <<  "Error: " << strerror(errno) << endl;
            exit(EXIT_FAILURE);
        }
        else
            cout << "running execve" << endl;
    }
    else
    {
        if (WIFEXITED(status))
        {
            cout << "Child process exit status" << WIFEXITED(status) << endl;
            cgi_path = req.parseCgiPath();

            req.setCgiPath(cgi_path);
            cout << "Method: " << req.getMethod() << endl;
            cout << "Query: " << req.getQueryString() << endl;
            cout << "Address: " << req.getAddress() << endl;
            cout << "Type: " << req.getHeader("Content-Type")<< endl;
            cout << "Length: " << req.getHeader("Content-Length") << endl;
            cout << "Path: " << req.parseCgiPath() << endl;
            // cout << "Cookies: " << req.getCookies() << endl;\
            
            // Send the HTTP response back to the client
            Response res;
            res.setStatusCode(200);
            res.setContentType("text/plain");
            res.setHeader("Set-Cookie", "mycookie=12345; Max-Age=3600; Path=/");
            res.setContent(req.getRequest().c_str(), req.getReadSize());
            res.printCookies();
            string response_str = res.restoString();
            send(client_socket, response_str.c_str(), response_str.size(), 0);
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
    (void)req;
    Response res;
    res.setStatusCode(200);
    res.setContentType("text/plain");
    res.setContent(req.getRequest().c_str(), req.getReadSize());
    string response_str = res.restoString();
    send(client_socket, response_str.c_str(), response_str.size(), 0);
}

//  curl -X POST -H "Content-Type: application/x-www-form-urlencoded" -d "name=shawn&age=30" http://localhost:8080/cgi-bin/hello.cgi
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
        read_request(client_socket, req);
        if (req.is_cgi_request())
            client_socket = handle_cgi(client_socket, req);
        else
        {
            handle_non_cgi(client_socket, req);
        }
        close(client_socket);
        cout << "++++++++Done+++++++" << endl;
        // system("leaks a.out");
    }
    close(server_socket);
}