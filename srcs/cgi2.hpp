/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi2.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: steh <steh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/30 19:28:36 by leng-chu          #+#    #+#             */
/*   Updated: 2023/04/01 20:34:10 by steh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Parse the incoming HTTP request and identify if it's a CGI request
// Create a child process to handle the CGI request
// Set environment variables for the child process, such as the request method, URL, headers, etc.
// Set up input and output streams for the child process to read and write data
// Execute the CGI script in the child process and pass the necessary data
// Capture the output of the CGI script and send it back to the client as an HTTP response

#ifndef CGI_HPP
#define CGI_HPP

#define BUFF_SIZE 4096
#define ENV_SIZE 7

#include "lib.hpp"

enum cgi_extension
{
    PHP,
    CGI,
    Unknown
};

class Request
{

    private:
        string                          _request;
        string                          _cgi_path;
        string                          _extension;
        int                             _read_size;
        int                             _total_read_size;
        char*                           _envp[ENV_SIZE];
        map<string, vector<string> >    _extension_map;

    public:
        Request();
        Request(const string& request);
        Request(string& request, int num_read);
        ~Request();
        bool isCgiRequest() const;
        bool is_cgi_request();
        string  getRequestUrl() const;
        string  getMethod() const;
        string  getQueryString() const;
        string  getHeader(const string& header_name) const;
        string  getAddress() const;
        string  getExtension() const;
        string  getCgiPath() const;
        int     getTotalReadSize() const;
        int     getReadSize() const;
        string  getRequest() const;
        void    setCgiPath(string path);
        void    setRequest(string request);
        string  parseCgiPath();
        void    setBuffer(string& buffer, int num_read);
        void    setEnvp();
        int     read_request(int client_socket);
        char**  getEnvp();
        void    printEnvp() const;
        void    freeEnvp(char **envp);
        int     handle_cgi(int client_socket);
        char**  handleArgs(const string& extension, const string& cgi_path);

};

class Response
{
    public:
        void    setStatusCode(int code);
        void    setContentType(const string& type);
        void    setContent(const char* content, int length);
        string  restoString() const;
        void    setHeader(const string& name, const string& value);
        void    printCookies() const;

    private:
        int                     _status_code;
        string                  _content_type;
        string                  _content;
        map<string, string>     _headers;

        static string           getReasonPhrase(int code);

};

void	handle_non_cgi(int client_socket, Request & req);

#endif
