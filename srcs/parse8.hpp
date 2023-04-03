/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse8.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leng-chu <-chu@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 19:40:59 by leng-chu          #+#    #+#             */
/*   Updated: 2023/04/03 12:55:15 by leng-chu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef TOKEN_HPP
# define  TOKEN_HPP

#include "cookies.hpp"

struct configItem;
struct Directive;

struct Server_Detail
{
	int					id;
    string              serverName; // this is good, direct get without loop
    string              port;
    string              clientMaxBodySize;
    vector<string>      errorPage; // this is ok, one time loop
    string              root;
    string              index;
    string              autoIndex;
    string              redirection;
    vector<Directive>   locations; // lol this is using 6 loops
	multimap<string, multimap<string, string> >	mylocations; // this is using 3 loops only
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
    vector<string>      args; // one loop
    int                 line;
    vector<Directive>   block; // 4 loops 
};

#endif
