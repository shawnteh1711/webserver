#include "cookies.hpp"

string  Request::_prev_cookies = "";
map<string, string> Request::_session_cookies;

Request::Request() : _request()
{
    return ;
}

Request::Request(const string& request, const string & cgi_path)
	: _request(request), _cgi_path(cgi_path)
{
    this->ParseReqBody();
    this->is_cgi_request();
    return ;
}

Request::Request(const string& request, const string & cgi_path, string & cookies)
	: _request(request), _cgi_path(cgi_path), _cookies(cookies)
{
    istringstream       iss(_cookies);
    string              cookie;
    bool                session_id_found; 
    bool                result;
    string              session_id;

    session_id_found = false;
    while (getline(iss, cookie, ';'))
    {
        cookie.erase(0, cookie.find_first_not_of(' '));
        cookie.erase(cookie.find_last_not_of(' ') + 1);

        istringstream   cookieIss(cookie);
        string          cookieName, cookieValue;
        if (getline(cookieIss, cookieName, '=') && getline(cookieIss, cookieValue))
        {
            result = compare_cookies(_cookies, _prev_cookies);
            if (cookieName == "session_id" && !result)
            {
                session_id_found = true;
                _req_session_id = "";
                _prev_cookies = _cookies;
            }
            else
            {
                if (!session_id_found && result)
                {
                    session_id = generateSessionId();
                    _session_cookies[session_id] = _cookies + ";"; 
                    session_id_found = true;
                    _req_session_id = session_id;
                }
            }
        }
    }
    this->ParseReqBody();
    this->is_cgi_request();
    return ;
}

Request::Request(string& request, int num_read)
{
    setBuffer(request, num_read);
    return ;
}

void    split(string& str, char delimiter, vector<string>& key_value)
{
    size_t pos = 0;
    string token;
    while ((pos = str.find(delimiter)) != string::npos)
    {
        token = str.substr(0, pos);
        key_value.push_back(token);
        str.erase(0, pos + 1);
    }
    key_value.push_back(str);
}

void    Request::ParseReqBody()
{
    _key_value.clear();
    size_t black_line_index = _request.find("\r\n\r\n");
    if (black_line_index != string::npos)
    {
        _req_body = _request.substr(black_line_index + 4);
        if (_req_body.find("&") == string::npos && _req_body.find('=') == string::npos)
        {
            _key_value["value"] = _req_body;;
            return ;
        }
        vector<string> key_value;
        split(_req_body, '&', key_value);
        for (size_t i = 0; i < key_value.size(); i++)
        {
            vector<string> key_value2;
            split(key_value[i], '=', key_value2);
            if (key_value2.size() == 2) // check if key and value are present
            {
                _key_value[key_value2[0]] = key_value2[1];
            }
        }
    }
    else
    {
        _req_body  = "";
    }
}

string    Request::getReqBody() const //raw body
{
    return (_req_body);
}

map<string, string>    Request::getKeyValueBody() const // map key value
{
    return (_key_value);
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
    else if (extension == ".py")
        return (PYTHON);
    else
        return (Unknown);
}

bool Request::is_cgi_request()
{       
    bool            val;
    string          uri;
    size_t          dot_pos;

    val = false;
    uri = getRequestUrl();
    dot_pos = uri.rfind('.');
    if (dot_pos != string::npos)
    {
        _extension = uri.substr(dot_pos);
        cout << "extension: " << _extension << endl;
        for (int i = 0; i < Unknown; ++i)
        {
            if (getEntensionType(_extension) == static_cast<cgi_extension>(i))
            {
                val = true;
                break;
            }
        }
    }
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

    space_pos = _request.find(' ');
    if (space_pos != string::npos)
    {
        url_pos = _request.find('/', space_pos + 1);
        if (url_pos != string::npos)
        {
            header_end_pos = _request.find("\r\n");
            if (header_end_pos != string::npos)
            {
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

string    Request::setCgiPath()
{
    _pwd = getenv("PWD");
    const char* cgi_bin_path = "/cgi-bin/";
    _path_info = _pwd + cgi_bin_path + this->parseRequestedFile();
    return (_path_info);
}

void    Request::setCgiPath2(string path)
{
    _pwd = getenv("PWD");
    const char* cgi_bin_path = "/cgi-bin/";
    _path_info = _pwd + cgi_bin_path + path;
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

string Request::parseRequestedFile()
{
    string  cgi_file;
    size_t  header_end;
    size_t  path_start;
    size_t  path_end;

    path_start = _request.find(" ");
    header_end = _request.find("\r\n");
    path_end = _request.substr(0, header_end).find('?');
    if (path_end == string::npos)
        path_end = _request.find(" ", path_start + 1);
    if (path_end == string::npos)
        return "";
    string path = _request.substr(path_start + 1, path_end - path_start - 1);
    if (path.front() == '/')
        cgi_file = path.erase(0, 1);
    if (path.find("cgi-bin/") != string::npos)
    {
        cgi_file = path.substr(8); // Skip "/cgi-bin/" prefix
        if (cgi_file.back() == '/')
            cgi_file.pop_back();
    }
    return (cgi_file);
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
    return (server_socket);
}

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
    vector<string> env_vars;
    env_vars.push_back("REQUEST_METHOD=" + this->getMethod());
    env_vars.push_back("QUERY_STRING=" + this->getQueryString());
    env_vars.push_back("CONTENT_TYPE=" + this->getHeader("Content-Type"));
    env_vars.push_back("CONTENT_LENGTH=" + this->getHeader("Content-Length"));
    env_vars.push_back("REMOTE_ADDR=" + this->getAddress());
    env_vars.push_back("SCRIPT_NAME=" + this->parseRequestedFile());
    env_vars.push_back("SCRIPT_PATH=" + _path_info);
    env_vars.push_back("HTTP_COOKIE=" + this->getCookies());

    string session_cookies;
    for (map<string, string>::const_iterator it = _session_cookies.begin(); it != _session_cookies.end(); ++it)
        session_cookies += it->first + ":" + it->second + "\n";

    env_vars.push_back("HTTP_COOKIES_MAP=" + session_cookies);

    vector<char*> envp2(env_vars.size() + 1, NULL);

    for (int i = 0; i < (int)env_vars.size(); i++)
    {
        envp2[i] = new char[env_vars[i].size() + 1];
        strcpy(envp2[i], env_vars[i].c_str());
    }

    _envp2 = envp2;
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

vector<char*> Request::getEnvp2()
{
    return (_envp2);
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

map<string, string>    Request::getSessionCookies() const
{
    return (_session_cookies);
}



char** Request::handleArgs()
{
    vector<string>              commandArgs;
    char**                      args;
    int                         i;
    vector<string>::iterator    it;

    _extension_map[".cgi"].push_back(_path_info);
    _extension_map[".php"].push_back("/usr/bin/php");
    _extension_map[".py"].push_back("/usr/bin/python");
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
    args[i++] = const_cast<char*>(_path_info.c_str());
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
    string cookies = request.getHeader("Cookie");
    if (cookies.empty())
    {
        cout << "No cookies found" << endl;
        return (false);
    }
    else
    {
        cout << "Cookies found: " << cookies << endl;
        return (true);
    }
}

string  Response::getRequestCookies(Request& request)
{
    return (request.getCookies());
}

string  Request::getPrevCookies() const
{
    return (_prev_cookies);
}

void    Response::sendCgiResponse(Request& request, int client_socket, const char *buffer, int count)
{
    string      response_str;
    int         bytes_sent;
    size_t      addCount;

    addCount = 0;
    _entered_session_id = "";
    setStatusCode(200);
    setContentType("text/html");
    if (request.hasCookies())
        setHeader("Set-Cookie", request.getCookies());
    if (request.getCookies() != "" && request.getReqSessionId() != "")
    {
        _res_session_id = request.getReqSessionId();
        addCount = strlen("hello, user with session_id: ") + _res_session_id.length() + 2; // add 2 for "\r\n"
    }
    if (request.getReqBody() != "")
    {
        size_t pos = request.getReqBody().find("session_id=");
        if (pos != string::npos)
        {
            map<string, string> session_cookies_map;
            
            session_cookies_map = request.getSessionCookies();
            _entered_session_id = request.getReqBody().substr(pos + strlen("session_id="));
            addCount += _entered_session_id.length() + 2; // add 2 for "\r\n"
            _entered_session_id.resize(16);     
            if (session_cookies_map.find(_entered_session_id) != session_cookies_map.end())
                _ret_cookies = session_cookies_map[_entered_session_id];
            cout << "session_cookie: " << _ret_cookies << endl;
        }
    }
    if (_ret_cookies != "")
    {
        string content = string(buffer, count) + _ret_cookies;
        setContent(content.c_str(), content.length());
    }
    else
        setContent(buffer, count + addCount);
    response_str = restoString();
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
    (void)buffer;
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
    char**      args;
    int         pipes[2];
    Response    res;

    _path_info = this->setCgiPath();
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
        close(pipes[0]);
        if (dup2(pipes[1], STDOUT_FILENO) == -1)
        {
            perror("dup2");
            exit(EXIT_FAILURE);
        }
        close(pipes[1]);
        this->setEnvp();
        args = handleArgs();
        if (execve(args[0], args, this->getEnvp()) == -1)
        {
            cerr <<  "Error: " << strerror(errno) << endl;
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        int         exit_status;
        struct stat file_stat;
        int         result;
        char        buffer[BUFF_SIZE + 1];
        string      root;
        string      path;
        int         count;
        int         t_count;
        string      final_buffer;

        memset(buffer, 0, BUFF_SIZE + 1);
        if (waitpid(pid, &status, 0) == -1)
        {
            perror("waitpid failed");
            exit(EXIT_FAILURE);
        }
        if (WIFEXITED(status))
        {
            cout << "Child process exit status " << WIFEXITED(status) << endl;
            exit_status = WEXITSTATUS(status);
            close(pipes[1]);
            t_count = 0;
            if (this->getHeader("Content-Length").empty())
            {
                while ((count = read(pipes[0], buffer, BUFF_SIZE)) > 0)
                {
                    buffer[count] = '\0';
                    final_buffer += string(buffer, BUFF_SIZE);
                    t_count += count;
                    if (buffer[BUFF_SIZE - 1] == EOF)
						break ;
                    memset(buffer, 0, BUFF_SIZE + 1);
                }
            }
            else
            {
                _content_length = atoi(this->getHeader("Content-Length").c_str());
                while (t_count < _content_length)
                {
                    count = read(pipes[0], buffer, min(BUFF_SIZE, _content_length - t_count));
                    final_buffer += string(buffer, BUFF_SIZE);
                    if (count == 0) 
                        break ;
                    t_count += count;
                    memset(buffer, 0, BUFF_SIZE + 1);
                }
            }
            result = stat(_cgi_path.c_str(), &file_stat);
            if (exit_status != 0)
            {
                if (result == -1)
                {
                    root = "/root/404.html";
                    path = _pwd + root;
                    res.sendErrorResponse(client_socket, 404, path);
                }
                else
                {
                    root = "/root/500.html";
                    path = _pwd + root;
                    res.sendErrorResponse(client_socket, 500, path);
                }
            }
            else
                res.sendCgiResponse(*this, client_socket, final_buffer.c_str(), t_count);
        }
    }
    return (client_socket);
}

string  Request::getReqSessionId() const
{
    return (_req_session_id);
}


int Request::handle_cgi2(int client_socket, string full_path)
{
    pid_t       pid;
    int         status;
    char**      args;
    int         pipes[2];
    Response    res;
    size_t      pos;

    pos = full_path.find("?");
    if (pos != string::npos)
        full_path = full_path.substr(0, pos);
    this->setCgiPath2(full_path);
    _extension = full_path.substr(full_path.find_last_of("."));
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
        close(pipes[0]);
        if (dup2(pipes[1], STDOUT_FILENO) == -1)
        {
            perror("dup2");
            exit(EXIT_FAILURE);
        }
        close(pipes[1]);
        this->setEnvp();
        args = handleArgs();
        vector<char*> envp = this->getEnvp2();
        char **envp_array = new char *[envp.size() + 1];
        for (size_t i = 0; i < envp.size(); ++i)
            envp_array[i] = envp[i];
        envp_array[envp.size()] = nullptr;
        if (execve(args[0], args, envp_array) == -1)
        {
            cerr <<  "Error: " << strerror(errno) << endl;
            exit(EXIT_FAILURE);
        }
        delete[] envp_array;
    }
    else
    {
        int         exit_status;
        struct stat file_stat;
        int         result;
        char        buffer[BUFF_SIZE + 1];
        string      root;
        string      path;
        int         count;
        int         t_count;
        string      final_buffer;

        memset(buffer, 0, BUFF_SIZE + 1);
        if (waitpid(pid, &status, 0) == -1)
        {
            perror("waitpid failed");
            exit(EXIT_FAILURE);
        }
        if (WIFEXITED(status))
        {
            cout << "Child process exit status " << WIFEXITED(status) << endl;
            exit_status = WEXITSTATUS(status);
            close(pipes[1]);
            t_count = 0;
            if (this->getMethod() == "POST")
            {
                while ((count = read(pipes[0], buffer, BUFF_SIZE)) > 0)
                {
                    buffer[count] = '\0';
                    final_buffer += string(buffer, BUFF_SIZE);
                    t_count += count;
                    if (buffer[BUFF_SIZE - 1] == EOF)
						break ;
                    memset(buffer, 0, BUFF_SIZE + 1);
                }
            }
            if (this->getHeader("Content-Length").empty())
            {
                while ((count = read(pipes[0], buffer, BUFF_SIZE)) > 0)
                {
                    buffer[count] = '\0';
                    final_buffer += string(buffer, BUFF_SIZE);
                    t_count += count;
                    if (buffer[BUFF_SIZE - 1] == EOF)
						break ;
                    memset(buffer, 0, BUFF_SIZE + 1);
                }
            }
            else if (this->getMethod() == "GET")
            {
                _content_length = atoi(this->getHeader("Content-Length").c_str());
                while (t_count < _content_length)
                {
                    count = read(pipes[0], buffer, min(BUFF_SIZE, _content_length - t_count));
                    final_buffer += string(buffer, BUFF_SIZE);
                    if (count == 0) 
                        break ;
                    t_count += count;
                    memset(buffer, 0, BUFF_SIZE + 1);
                }
            }
            result = stat(_cgi_path.c_str(), &file_stat);
            if (exit_status != 0)
            {
                if (result == -1)
                {
                    root = "/root/404.html";
                    path = _pwd + root;
                    res.sendErrorResponse(client_socket, 404, path);
                }
                else
                {
                    root = "/root/500.html";
                    path = _pwd + root;
                    res.sendErrorResponse(client_socket, 500, path);
                }
            }
            else
                res.sendCgiResponse(*this, client_socket, final_buffer.c_str(), t_count);
        }
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
}

bool Request::hasCookies()
{
    string cookies = this->getHeader("Cookie");
    if (cookies.empty())
    {
        return (false);
    }
    else
    {
        if (_cookies != cookies)
        {
            _prev_cookies = _cookies;
            _cookies = cookies;
        }
        return (true);
    }
}

string  Request::getCookies() const
{
    return (_cookies);
}

void    deleteFile(const char *path)
{
    cout << RED << "path: " << path << endl;
    int result = remove(path);
    if (result == 0)
        cout << "File deleted successfully" << endl;
    else
        cout << "Error: unable to delete the file" << endl;
}

string  generateSessionId()
{
    srand(time(NULL));
    stringstream ss;
    for (int i = 0; i < 16; i++)
    {
        int r = rand() % 16;
        ss << hex << r;
    }
    return (ss.str());
}

void    Response::setSessionId(string &session_id)
{
    _res_session_id = session_id;
}


string  Response::getResSessionId() const
{
    return (_res_session_id);
}

string  extractSessionId(string& cookies)
{
    string  session_id;
    size_t start = cookies.find("session_id=");
    if (start != string::npos)
    {
        start += sizeof("session_id=") - 1;
        size_t end = cookies.find(";", start);
        if (end == string::npos)
            end = cookies.size();
        session_id = cookies.substr(start, end - start);
    }
    return (session_id);
}

string  extractCookies(string& request)
{
    string  cookies;
    size_t start = request.find("Cookie: ");
    if (start != string::npos)
    {
        start += sizeof("Cookie: ") - 1;
        size_t end = request.find("\r\n", start);
        if (end == string::npos)
            end = request.size();
        cookies = request.substr(start, end - start);
    }
    return (cookies);
}

bool    compare_cookies(const string& cookies, const string& prev_cookies)
{
    map<string, string> cookies_map;
    istringstream iss(cookies);
    string         token;

    while (getline(iss, token, ';'))
    {
        size_t pos = token.find('=');
        if (pos != string::npos)
        {
            string key = token.substr(0, pos);
            string value = token.substr(pos + 1);
            cookies_map[key] = value;
        }
    }

    map<string, string> prev_cookies_map;
    iss.clear();
    iss.str(prev_cookies);
    while (getline(iss, token, ';'))
    {
        size_t pos = token.find('=');
        if (pos != string::npos)
        {
            string key = token.substr(0, pos);
            string value = token.substr(pos + 1);
            prev_cookies_map[key] = value;
        }
    }
    if (cookies_map.size() != prev_cookies_map.size())
        return (true);
    for (map<string, string>::const_iterator it = cookies_map.begin(); it != cookies_map.end(); it++)
    {
        if (it->first != "session_id" && prev_cookies_map[it->first] != it->second)
        {
            cout << "enter here " << endl;
            return (true);
        }
    }
    return (false);
}
