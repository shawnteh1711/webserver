#include "cgi2.hpp"

#define BUFF_SIZE 4096

Request::Request() : _request()
{
    return ;
}

Request::Request(const string& request) : _request(request)
{
    return ;
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

bool Request::is_cgi_request()
{
    bool    val;

    val = (_request.find("/cgi-bin/") != string::npos);
    return (val);
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
//     cout << "query string function" << endl;
//     size_t uri_pos = _request.find(" ");
//     if (uri_pos == string::npos)
//         return "";

//     size_t query_pos = _request.find("?", uri_pos + 1);
//     if (query_pos == string::npos)
//         return "";

//     size_t end_query_pos = _request.find(" ", query_pos + 1);
//     if (end_query_pos == string::npos)
//         return "";

//     return _request.substr(query_pos + 1, end_query_pos - query_pos - 1);
// }

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

void    Request::setCgiPath(string path)
{
    _cgi_path = path;
}

void    Request::setRequest(string request)
{
    _request = request;
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

int accept_connection(char* buffer, int server_socket, struct sockaddr_in client_address)
{
    int client_socket;

    client_socket = 0;
    socklen_t client_address_len = sizeof(client_address);
    client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_len);
    if (client_socket < 0)
    {
        perror("In accept");
        exit(EXIT_FAILURE);
    }
    fill(buffer, buffer + BUFF_SIZE, 0);
    read(client_socket, buffer, BUFF_SIZE);
    cout << buffer << endl;
    const char* hello = "hello from server";
    write(client_socket, hello, strlen(hello));
    return (client_socket);
}

int accept_cgi_connection(char* buffer, int server_socket, struct sockaddr_in client_address)
{
    pid_t   pid;
    char*   args[2];
    int     status;
    int     client_socket;
    int     num_read;
    bool    is_cgi_request;
    string  cgi_path;
    Request req;
    int     fd[2];
    int     bytes_read;
    int     bytes_write;

    client_socket = 0;
    if (pipe(fd) == -1)
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
        Request req;

        dup2(fd[0], STDIN_FILENO);
        bytes_read = read(fd[0], &req, sizeof(Request));
        if (bytes_read == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        cout << "adfsdada: " << req.parseCgiPath() << endl;
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
        args[0] = const_cast<char*>(req.parseCgiPath().c_str());
        // args[0] = const_cast<char*>(script_name.c_str());
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
        socklen_t client_address_len = sizeof(client_address);
        client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_len);
        if (client_socket < 0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        if (waitpid(pid, &status, 0) == -1)
        {
            perror("waitpid failed");
            exit(EXIT_FAILURE);
        }
        req.setRequest(buffer);
        // close(fd[0]);
        // bytes_write = write(fd[1], &req, sizeof(Request));
        if (WIFEXITED(status))
        {

            cout << "Child process exit status" << WIFEXITED(status) << endl;
            // num_read = read(out_pipe[0], buffer, BUFF_SIZE);
            bzero(buffer, BUFF_SIZE);
            num_read = read(client_socket, buffer, BUFF_SIZE);
            if (num_read == -1)
            {
                perror("read failed");
                exit(EXIT_FAILURE);
            }
         
            // if (bytes_write == -1)
            // {
            //     perror("Write");
            //     exit(EXIT_FAILURE);
            // }
            is_cgi_request = req.is_cgi_request();
         
            if (is_cgi_request)
            {
                cgi_path = req.parseCgiPath();
                req.setCgiPath(cgi_path);
                cout << "Method: " << req.getMethod() << endl;
                cout << "Query: " << req.getQueryString() << endl;
                cout << "Address: " << req.getAddress() << endl;
                cout << "Type: " << req.getHeader("Content-Type")<< endl;
                cout << "Length: " << req.getHeader("Content-Length") << endl;
                cout << "Path: " << req.parseCgiPath() << endl;
            }

            // Send the HTTP response back to the client
            Response res;
            res.setStatusCode(200);
            res.setContentType("text/plain");
            // res.setContentType(req.getHeader("Content-Type").c_str());
            res.setContent(buffer, num_read);
            // res.setContent(response_buffer.str().c_str(), response_buffer.str().length());
            string response_str = res.restoString();
            // cout << response_str << endl;
            send(client_socket, response_str.c_str(), response_str.size(), 0);
        }
        // close(in_pipe[1]);
        // close(out_pipe[0]);
    }
    return (client_socket);
}

//  curl -X POST -H "Content-Type: application/x-www-form-urlencoded" -d "name=shawn&age=30" http://localhost:8080/cgi-bin/hello.cgi
int main()
{

    int client_socket;
    int server_socket;
    struct sockaddr_in client_address;

    server_socket = create_server_socket();

    char buffer[BUFF_SIZE];

    while (true)
    {
        cout << "+++++++Waiting for new connection+++++++" << endl;
        // client_socket = accept_connection(buffer, server_socket, client_address);
        // client_socket = accept_cgi_connection(req, buffer, server_socket, client_address);
        client_socket = accept_cgi_connection(buffer, server_socket, client_address);
        cout << "Client connected from " << inet_ntoa(client_address.sin_addr) << ":" << ntohs(client_address.sin_port) << endl;
        close(client_socket);
        cout << "++++++++Done+++++++" << endl;
    }
}
