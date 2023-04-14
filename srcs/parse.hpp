/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: steh <steh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 19:40:59 by leng-chu          #+#    #+#             */
/*   Updated: 2023/04/13 20:33:32 by leng-chu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef TOKEN_HPP
# define  TOKEN_HPP

#include "cookies.hpp"

struct	configItem;
struct	Directive;

struct	Server_Detail
{
	int											id;
    string              						serverName;
    string              						port;
    string              						clientMaxBodySize;
    vector<string>      						errorPage;
    map<int, string>    						errorPageMap;
    string              						root;
    string              						index;
    string              						redirection;
    vector<Directive>   						locations;
	multimap<string, multimap<string, string> >	mylocations;
    map<string, string>							urlSearch;
    map<string, string> 						urlLimitExcept;
    vector<string>      						urlIndexOn;
    map<string, string> 						urlRoot;
    map<string, string> 						urlCgi;
    vector<string>      						urlLocation;

    void search(string search);
};

struct	Config
{
    string					status;
    string					file;
    vector<string>			errors;
    vector<configItem>		configItems;
    vector<Server_Detail>	servers;

    Config parseConfigFile(string filename);
    vector<Directive*> findDirective(Config& config, string directiveName);
    vector<Server_Detail> setServer(Config& config);
    void printServer(vector<Server_Detail>& servers);
    void  print(Config config);
    const vector<Server_Detail>& getServers() const;
    Server_Detail createServer(const Directive& directive);

};

struct	configItem
{
    vector<Directive>	parsed;
};

struct	Directive
{
    string				directive;
    vector<string>		args;
    int					line;
    vector<Directive>	block;
};

#endif
