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


}

// #include <iostream>
// #include <string>
// #include <vector>

// using namespace std;

// class Request {
// public:
//     Request(const string& request) : request_(request) {}
//     bool isCGIRequest() const;

// private:
//     string request_;
// };

// bool Request::isCGIRequest() const {
//     // Check if request method is POST and has a valid content-type header
//     size_t method_pos = request_.find("POST");
//     if (method_pos == string::npos) {
//         return false;
//     }

//     size_t content_type_pos = request_.find("Content-Type:");
//     if (content_type_pos == string::npos) {
//         return false;
//     }

//     size_t content_type_value_pos = request_.find("application/x-www-form-urlencoded");
//     if (content_type_value_pos == string::npos) {
//         return false;
//     }

//     // Check if request URI is a CGI script
//     size_t uri_pos = request_.find(" ");
//     if (uri_pos == string::npos) {
//         return false;
//     }

//     string uri = request_.substr(uri_pos + 1);
//     size_t cgi_pos = uri.find(".cgi");
//     if (cgi_pos == string::npos) {
//         return false;
//     }

//     return true;
// }

// int main() {
//     // Sample HTTP request
//     string request = "POST /cgi-bin/hello.cgi HTTP/1.1\r\nHost: localhost:8080\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\nname=John&age=30";

//     // Create Request object
//     Request req(request);

//     // Check if request is a CGI request
//     bool is_cgi_request = req.isCGIRequest();

//     if (is_cgi_request) {
//         cout << "Request is a CGI request" << endl;
//     } else {
//         cout << "Request is not a CGI request" << endl;
//     }

//     return 0;
// }
