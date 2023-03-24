# include <string>
# include <vector>
# include <iostream>
# include <sstream>
# include <fstream>
# include <map>
# include <utility>
#include "lib.hpp"

#ifndef TOKEN_HPP
# define  TOKEN_HPP
using namespace std;

struct configItem;
struct Directive;

struct Server_Detail
{
    string              serverName;
    string              port;
    string              clientMaxBodySize;
    vector<string>      errorPage;
    string              root;
    string              index;
    string              autoIndex;
    string              redirection;
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
