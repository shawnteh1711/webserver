/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leng-chu <-chu@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 19:32:16 by leng-chu          #+#    #+#             */
/*   Updated: 2023/03/22 19:53:19 by leng-chu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_HPP
# define  TOKEN_HPP

#include "lib.hpp"
#include "server.hpp"

using namespace std;

struct configItem;
struct Directive;

struct Server_Detail
{
    string              serverName;
    string              port;
    string              clientMaxBodySize;
    string              errorPage;
    string              root;
    string              index;
    vector<Directive>   locations;
};

struct Config
{
    string              status;
    string              file;
    vector<string>      errors;
    vector<configItem>  configItems;
    vector<Server_Detail>      servers;
    Config                  parseConfigFile(string filename);
    vector<Directive*>      findDirective(Config& config, string directiveName);
    vector<Server_Detail>          setServer(Config& config);
    void                    printServer(vector<Server_Detail>& servers);
    void                    print(Config config);
    const vector<Server_Detail>&   getServers() const;
    Server_Detail                  createServer(const Directive& directive);

};

struct configItem
{
    // string              directive;
    // vector<string>      errors;
    vector<Directive>   parsed;
};

struct Directive
{
    string              directive;
    vector<string>      args;
    int                 line;
    vector<Directive>   block;
};

#endif
