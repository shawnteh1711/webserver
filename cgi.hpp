// Parse the incoming HTTP request and identify if it's a CGI request
// Create a child process to handle the CGI request
// Set environment variables for the child process, such as the request method, URL, headers, etc.
// Set up input and output streams for the child process to read and write data
// Execute the CGI script in the child process and pass the necessary data
// Capture the output of the CGI script and send it back to the client as an HTTP response

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Request
{
    private:
        string _request;
    public:
        Request(const string& request);
        ~Request();
        bool isCgiRequest() const;
};
