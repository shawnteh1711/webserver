/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cookies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: steh <steh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/30 19:28:36 by leng-chu          #+#    #+#             */
/*   Updated: 2023/04/03 20:50:35 by steh             ###   ########.fr       */
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

#define BUFF_SIZE 4096
#define ENV_SIZE 8

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
        string                          _cookies;
        string                          _query;

    public:
        Request();
        Request(const string& request, const string & cgi_path);
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
        void    setQuery(string& query);
        void    setRequest(string request);
        string  parseCgiPath();
        void    setBuffer(string& buffer, int num_read);
        void    setEnvp();
        int     readRequest(int client_socket);
        char**  getEnvp();
        void    printEnvp() const;
        void    freeEnvp(char **envp);
        int     handle_cgi(int client_socket);
        char**  handleArgs(const string& cgi_path);
        bool    hasCookies();
        string  getCookies() const;

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

    private:
        int                     _status_code;
        string                  _content_type;
        string                  _content;
        map<string, string>     _headers;

        static string           getReasonPhrase(int code);

};

void	handle_non_cgi(int client_socket, Request & req);

#endif
