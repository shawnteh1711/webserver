# include <string>
# include <vector>
# include <iostream>
# include <sstream>
# include <fstream>
# include <map>
# include <utility>

#ifndef TOKEN_HPP
# define  TOKEN_HPP
using namespace std;

struct configItem;
struct Directive;

struct Server
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
    vector<Server>      servers;

    Config                  parseConfigFile(string filename);
    vector<Directive*>      findDirective(Config& config, string directiveName);
    vector<Server>          setServer(Config& config);
    void                    printServer(vector<Server>& servers);
    void                    print(Config config);
    const vector<Server>&   getServers() const;
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
