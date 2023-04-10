/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cookies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: steh <steh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/30 19:28:36 by leng-chu          #+#    #+#             */
/*   Updated: 2023/04/10 13:35:04 by steh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Cookies
// If (http request use the cookie header)
// {    use the cookie header to extract the cookie data }
// else if (no cookie header)
// {    create a new cookie header set its value and send response}
// else if (cookies present in request)
// {    read the cookie data and use it to customize response }

#ifndef COOKIES_HPP
#define COOKIES_HPP

#define BUFF_SIZE 1024
#define ENV_SIZE 8

#include "lib.hpp"

class Server;

enum cgi_extension
{
    PHP,
    CGI,
    PYTHON,
    Unknown
};

class Request
{
    private:
        string                          _request;
        string                          _cgi_path;
        string                          _path_info;
        string                          _extension;
        int                             _read_size;
        int                             _total_read_size;
        char*                           _envp[ENV_SIZE];
        map<string, vector<string> >    _extension_map;
        string                          _cookies;
        string                          _query;
        string                          _pwd;
        int                             _content_length;
        string                          _req_body;
        map<string, string>             _key_value;

    public:
        Request();
        Request(const string& request, const string & cgi_path);
        Request(const string& request, const string & cgi_path, map<string, string> & cgi_url_location);
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
        string  setCgiPath();
        void    setCgiPath2(string path);
        void    setQuery(string& query);
        void    setRequest(string request);
        string  parseRequestedFile();
        void    setBuffer(string& buffer, int num_read);
        void    setEnvp();
        int     readRequest(int client_socket);
        char**  getEnvp();
        void    printEnvp() const;
        void    freeEnvp(char **envp);
        int     handle_cgi(int client_socket);
        int     handle_cgi2(int client_socket, string full_path);
        char**  handleArgs();
        bool    hasCookies();
        void    readPipe(int count, char* buffer);
        string  getCookies() const;
        string  getReqBody() const;
        void    ParseReqBody();
        map<string, string>    getKeyValueBody() const;

};

class Response
{
    public:
        void    setStatusCode(int code);
        void    setContentType(const string& type);
        void    setContent(const char* content, int length);
        string  getContent() const;
        string  restoString() const;
        void    setHeader(const string& name, const string& value);
        void    printCookies() const;
        void    sendCgiResponse(Request& request, int client_socket, const char *buffer, int num_read);
        void    sendErrorResponse(int client_socket,  int status_code, string path);
        bool    checkRequestCookies(Request& request);
        string  getRequestCookies(Request& request);

    private:
        int                     _status_code;
        string                  _content_type;
        string                  _content;
        string                  _cookies;
        map<string, string>     _headers;

        static string           getReasonPhrase(int code);

};

void	handle_non_cgi(int client_socket, Request & req);
void    deleteFile(const char* path);
#endif
