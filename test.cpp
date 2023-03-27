#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

string getQueryString(string _request)
{
    size_t query_pos = _request.find("?");
    if (query_pos == string::npos)
        return "";

    return _request.substr(query_pos + 1);
}

// string getQueryString(string _request)
// {
//     size_t uri_pos = _request.find(" ");
//     cout << "uri_pos: " << uri_pos << endl;
//     if (uri_pos == string::npos)
//         return "";

//     size_t query_pos = _request.find("?", uri_pos + 1);
//     cout << "query_pos: " << query_pos << endl;
//     if (query_pos == string::npos)
//         return "";

//     size_t end_query_pos = _request.find(" ", query_pos + 1);
//     cout << "end_query_pos: " << end_query_pos << endl;
//     if (end_query_pos == string::npos)
//         return "";

//     return _request.substr(query_pos + 1, end_query_pos - query_pos - 1);
// }

// string getQueryString(string request)
// {
//     size_t body_pos = request.find("\r\n\r\n");
//     if (body_pos == string::npos) {
//         return "";
//     }
//     return request.substr(body_pos + 4);
// }


int main()
{
    // string request = "POST /cgi-bin/hello.cgi HTTP/1.1\r\nHost: localhost:8080\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\nname=John&age=30";
    string request = "http://127.0.0.1/cgi-bin/hello.cgi?name=shawn";
    string  ret;

    ret = getQueryString(request);
    cout << ret << endl;

    return 0;
}
