#include "cgi.hpp"

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

string  Request::getMethod() const
{
    size_t  method_pos = _request.find(" ");
    if (method_pos == string::npos)
        return "";
    return (_request.substr(0, method_pos));
}

string  Request::getQueryString() const
{
    size_t uri_pos = _request.find(" ");
    if (uri_pos == string::npos)
        return "";

    size_t query_pos = _request.find("?", uri_pos + 1);
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

string parse_cgi_path(string request)
{
    string cgi_path;

    size_t path_start = request.find(" ");
    size_t path_end = request.find(" ", path_start + 1);
    string path = request.substr(path_start + 1, path_end - path_start - 1);
    if (path.find("/cgi-bin/") == 0) {
        cgi_path = path.substr(9); // Skip "/cgi-bin/" prefix
    } else {
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

string  Response::toString() const
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
    server_address.sin_port = htons(8080);

    // create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    int optval = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // bind socket to the specified address
    bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address));

    // listen for incoming connections
    if (listen(server_socket, 5) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }

    // accept a connection

    return (server_socket);
}

int main()
{
    string request = "POST /cgi-bin/hello.cgi HTTP/1.1\r\nHost: localhost:8080\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\nname=John&age=30";

    Request req(request);
    bool    is_cgi_request;
    string  cgi_path;
    int in_pipe[2];
    int out_pipe[2];
    int client_socket;
    int server_socket;
    struct sockaddr_in client_address;

    server_socket = create_server_socket();
    socklen_t client_address_len = sizeof(client_address);

    client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_len);
    if (client_socket < 0)
    {
        perror("In accept");
        exit(EXIT_FAILURE);
    }
    cout << "Client connected from " << inet_ntoa(client_address.sin_addr) << ":" << ntohs(client_address.sin_port) << endl;
    cgi_path = parse_cgi_path(request);
    is_cgi_request = req.isCgiRequest();

    if (is_cgi_request)
    {
        pid_t   pid = fork();
        if (pid == -1)
        {
            perror("fork failed");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            // Set up input and output streams for the child process
   
            if (pipe(in_pipe) == -1 || pipe(out_pipe) == -1)
            {
                perror("pipe failed");
                exit(EXIT_FAILURE);
            }
            // Redirect standard input and output of child process to pipes
            if (dup2(in_pipe[0], STDIN_FILENO) == -1 || dup2(out_pipe[1], STDOUT_FILENO) == -1)
            {
                perror("dup2 failed");
                exit(EXIT_FAILURE);
            }
            close(in_pipe[1]);
            close(out_pipe[0]);
            char* args[2];

            // Set environment variables for the child process
            setenv("REQUEST_METHOD", req.getMethod().c_str(), 1);
            setenv("QUERY_STRING", req.getQueryString().c_str(), 1);
            setenv("CONTENT_TYPE", req.getHeader("Content-Type").c_str(), 1);
            setenv("CONTENT_LENGTH", req.getHeader("Content-Length").c_str(), 1);
            setenv("REMOTE_ADDR", req.getAddress().c_str(), 1);
            setenv("SCRIPT_NAME", cgi_path.c_str(), 1);

            args[0] = const_cast<char*>(cgi_path.c_str());
            args[1] = NULL;
            if (execve(args[0], args, NULL) == -1)
            {
                cerr <<  "Error: " << strerror(errno) << endl;
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            int status;
            if (waitpid(pid, &status, 0) == -1)
            {
                perror("waitpid failed");
                exit(EXIT_FAILURE);
            }
            if (WIFEXITED(status))
            {
                cout << "Child process exit status" << WIFEXITED(status) << endl;
                  // Get the output of the CGI script from the pipe
                char buffer[4096];
                int num_read = read(out_pipe[0], buffer, sizeof(buffer));
                // int num_read = read(client_socket, buffer, sizeof(buffer));
                // int num_read = recv(out_pipe[0], buffer, sizeof(buffer), 0);
                if (num_read == -1)
                {
                    perror("read failed");
                    exit(EXIT_FAILURE);
                }

                // Send the HTTP response back to the client
                Response res;
                res.setStatusCode(200);
                res.setContentType(req.getHeader("Content-Type").c_str());
                res.setContent(buffer, num_read);
                string response_str = res.toString();
                write(client_socket, response_str.c_str(), response_str.length());
            }
        }
    }
    // system("leaks a.out");
}
