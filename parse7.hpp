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

struct Config
{
    string              status;
    vector<string>      errors;
    vector<configItem>  configItems;

    Config              parseConfigFile(string filename);
    vector<Directive*>  findDirective(Config& config, string directiveName);
void                    print(Config config);
};

struct configItem
{
    string              file;
    string              directive;
    vector<string>      errors;
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
