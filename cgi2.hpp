// Parse the incoming HTTP request and identify if it's a CGI request
// Create a child process to handle the CGI request
// Set environment variables for the child process, such as the request method, URL, headers, etc.
// Set up input and output streams for the child process to read and write data
// Execute the CGI script in the child process and pass the necessary data
// Capture the output of the CGI script and send it back to the client as an HTTP response

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

class Request
{
    private:
        string _request;
        string _cgi_path;
    public:
        Request();
        Request(const string& request);
        ~Request();
        bool isCgiRequest() const;
        bool is_cgi_request();
        string  getMethod() const;
        string  getQueryString() const;
        string  getHeader(const string& header_name) const;
        string  getAddress() const;
        string  getCgiPath() const;
        void    setCgiPath(string path);
        void    setRequest(string request);
        string  parseCgiPath();
};

class Response
{
    public:
        void    setStatusCode(int code);
        void    setContentType(const string& type);
        void    setContent(const char* content, int length);
        string  restoString() const;

    private:
        int             _status_code;
        string          _content_type;
        string          _content;
        static string   getReasonPhrase(int code);
};
