/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leng-chu <-chu@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 19:32:08 by leng-chu          #+#    #+#             */
/*   Updated: 2023/03/23 16:51:02 by leng-chu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.hpp"

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"      
#define BLUE    "\033[34m"

string trimLine(const string &str)
{
    size_t  start;
    size_t  end;

    start = str.find_first_not_of(" \t\r\n");
    end = str.find_last_not_of(" \t\r\n");
    if (start == string::npos || end == string::npos)
        return "";
    else
        return (str.substr(start, end - start + 1));
}

vector<string> splitLine(string str, char delimiter)
{
    vector<string>  result;
    stringstream    ss(str);
    string          token;
    while (getline(ss, token, delimiter))
    {
        result.push_back(token);
    }
    return (result);
}

Directive parseDirective(string& line, int lineNum)
{
    Directive directive;
    directive.line = lineNum;
    size_t argStart = line.find(' ');
    if (argStart != string::npos)
    {
        directive.directive = line.substr(0, argStart);
        if (directive.directive == "location" || directive.directive == "limit_except" || directive.directive == "upstream")
        {
            size_t  argEnd = line.find_last_not_of(' ', line.size() - 2);
            line = line.substr(argStart + 1, argEnd - argStart);
        }
        else
        {
            line = line.substr(argStart + 1);
            line = trimLine(line);
            if (line.back() == ';')
                line = line.substr(0, line.size() - 1);
        }
        directive.args.push_back(line);
    }
    return (directive);
}

string      removeComment(string line)
{
    line = trimLine(line);
    size_t commentPos = line.find('#');
    if (commentPos != string::npos)
        line = line.substr(0, commentPos);
    return (line);
}

vector<Directive> parseBlock(ifstream& file, int& lineNum, Directive& parentDirective)
{
    vector<Directive>           block;
    vector<Directive>*          currentBlock;
    stack<vector<Directive>*>   blockStack;
    string                      line;
    bool                        isServerBlock;
    bool                        isHttpBlock;

    isHttpBlock = parentDirective.directive == "http";
    currentBlock = &block;
    isServerBlock = false;
    blockStack.push(&block);
    while (getline(file, line))
    {
        lineNum++;
        if (line.empty() || line[0] == '#')
            continue;
        line = removeComment(line);
        Directive directive;
        directive.line = lineNum;
        if (line[0] == '}')
        {
            if (blockStack.empty())
            {
                throw runtime_error("Unmatched closing brace on line " + to_string(lineNum));
            }
            currentBlock = blockStack.top();
            blockStack.pop();
            if (parentDirective.directive == "http")
                isHttpBlock = false;
            if (blockStack.empty())
                return block;
            continue;
        }
        if (line.find("server {") == 0)
        {
            Directive serverDirective;
            serverDirective = parseDirective(line, lineNum);
            serverDirective.block = vector<Directive>();
            blockStack.top()->push_back(serverDirective);
            currentBlock = &blockStack.top()->back().block;
            blockStack.push(currentBlock);
            isServerBlock = true;
            continue;
        }
        size_t blockStart = line.find('{');
        // handle location
        if (blockStart != string::npos)
        {
            directive = parseDirective(line, lineNum);
            directive.block = parseBlock(file, lineNum, directive);
        }
        else
        {
            directive = parseDirective(line, lineNum);
        }
        if (directive.directive != "}")
            currentBlock->push_back(directive);
        else
        {
            blockStack.top()->push_back(directive);
            if (directive.directive == "http")
            {
                currentBlock = &blockStack.top()->back().block;
                blockStack.push(currentBlock);
                isHttpBlock = true;
                isServerBlock = false;
            }
            else if (directive.directive == "server")
            {
                currentBlock = &blockStack.top()->back().block;
                blockStack.push(currentBlock);
                isServerBlock = true;
            }
        }
    }
    throw runtime_error("Missing closing brace");
}

void    findDirectiveRecursive(vector<Directive>& directives, string directiveName, vector<Directive*>& result)
{
    for (vector<Directive>::iterator itDirective = directives.begin(); itDirective != directives.end(); ++itDirective)
    {
        if (itDirective->directive == directiveName)
            result.push_back(&(*itDirective));
        if (!itDirective->block.empty())
            findDirectiveRecursive(itDirective->block, directiveName, result);
    }
}

void print_config_item(const Directive& item, size_t depth = 0)
{
    std::string indent = std::string(depth * 4, ' ');
    std::cout << indent << "line: " << item.line << " | directive: " << setw(20) << std::left << item.directive;

    for (std::vector<std::string>::const_iterator it = item.args.begin(); it != item.args.end(); ++it)
    {
        std::cout << " | arg: " << *it;
    }

    std::cout << std::endl;

    for (std::vector<Directive>::const_iterator it = item.block.begin(); it != item.block.end(); ++it)
    {
        print_config_item(*it, depth + 1);
    }
}

/** Config class **/

Config::Config(void)
	: status(), file(), errors(), configItems(), servers(){}

Config::~Config(void){}

Config Config::parseConfigFile(string filename)
{
    Config      config;
    configItem  item;
    ifstream    file(filename);

    config.status = "ok";
    config.file = filename;
    config.errors.push_back("no error yet");
    if (file.is_open())
    {
        string line;
        int lineNum = 0;
        while (getline(file, line))
        {
            lineNum++;
            if (line.empty() || line[0] == '#')
                continue;
            line = removeComment(line);
            Directive directive;
            directive.line = lineNum;
            size_t blockStart = line.find('{');
            if (blockStart != string::npos)
            {
                directive = parseDirective(line, lineNum);
                directive.block = parseBlock(file, lineNum, directive);
            }
            else
                directive = parseDirective(line, lineNum);
            item.parsed.push_back(directive);
        }
        file.close();
        config.configItems.push_back(item);
    }
    else
    {
        config.errors.clear();
        config.errors.push_back("Unable to open file " + filename);
        config.status = "error unable to open file";
    }
    return (config);
}

vector<Directive*>  Config::findDirective(Config& config, string directiveName)
{
    vector<Directive*> result;
    for (vector<configItem>::iterator it = config.configItems.begin(); it < config.configItems.end(); ++it)
    {
       findDirectiveRecursive(it->parsed, directiveName, result);
    }
    return (result);
}

vector<Server_Detail> Config::setServer(Config& config)
{
    vector<Server_Detail> servers;
    vector<configItem>::const_iterator item_it;
    vector<vector<Directive> >::const_iterator parsed_it;
    vector<Directive>::const_iterator directive_it, nested_directive_it;

    for (item_it = config.configItems.begin(); item_it != config.configItems.end(); ++item_it)
    {
        for (directive_it = item_it->parsed.begin(); directive_it != item_it->parsed.end(); ++directive_it)
        {
            if (directive_it->directive == "server")
                servers.push_back(createServer(*directive_it));
            else
            {
                for (nested_directive_it = directive_it->block.begin(); nested_directive_it != directive_it->block.end(); ++nested_directive_it)
                {
                    if (nested_directive_it->directive == "server")
                        servers.push_back(createServer(*nested_directive_it));
                }
            }
        }
    }
    return servers;
}

void    Config::printServer(vector<Server_Detail>& servers)
{
    vector<Server_Detail>::const_iterator server_it;
    vector<Directive>::const_iterator location_it, directive_it;
    vector<string>::const_iterator arg_it;

    for (server_it = servers.begin(); server_it != servers.end(); ++server_it)
    {
        cout << GREEN << "server {" << endl;
        cout << "\tlisten " << server_it->port << ";" << endl;
        cout << "\tserver_name " << server_it->serverName << ";" << endl;
        cout << "\tclient_max_body_size " << server_it->clientMaxBodySize << ";" << endl;
        cout << "\terror_page " << server_it->errorPage << ";" << endl;
        cout << "\troot " << server_it->root << ";" << endl;
        cout << "\tindex " << server_it->index << ";" << endl;

        cout << RESET << BLUE;
        for (location_it = server_it->locations.begin(); location_it != server_it->locations.end(); ++location_it)
        {
            for (arg_it = location_it->args.begin(); arg_it != location_it->args.end(); ++arg_it)
            {
                std::cout << "\t" << location_it->directive << " " << *arg_it << " {" << std::endl;
            }

            for (directive_it = location_it->block.begin(); directive_it != location_it->block.end(); ++directive_it) 
            {
                cout << "\t\t" << directive_it->directive << " " << directive_it->args[0] << ";" << endl;
            }
        }
        cout << "\t" << "}" << endl;
        cout << "}" << endl << endl;
    }
}

void    Config::print(Config config)
{
    std::cout << "status: " << config.status << std::endl;
    std::cout << "config file: " << config.file << std::endl;

    for (vector<string>::const_iterator it = config.errors.begin(); it != config.errors.end(); ++it)
    {
        std::cout << "error: " << *it << std::endl;
    }
    for (std::vector<configItem>::const_iterator it = config.configItems.begin(); it != config.configItems.end(); ++it)
    {
        for (std::vector<Directive>::const_iterator it2 = it->parsed.begin(); it2 != it->parsed.end(); ++it2)
        {
            print_config_item(*it2);
        }
    }
}

const vector<Server_Detail>&   Config::getServers() const
{
    return (servers);
}

Server_Detail Config::createServer(const Directive& directive)
{
    Server_Detail new_server;
    vector<Directive>::const_iterator block_directive_it;

    for (block_directive_it = directive.block.begin(); block_directive_it != directive.block.end(); ++block_directive_it) {
        if (block_directive_it->directive == "listen") {
            new_server.port = block_directive_it->args[0];
        } else if (block_directive_it->directive == "server_name") {
            new_server.serverName = block_directive_it->args[0];
        } else if (block_directive_it->directive == "client_max_body_size") {
            new_server.clientMaxBodySize = block_directive_it->args[0];
        } else if (block_directive_it->directive == "error_page") {
            new_server.errorPage = block_directive_it->args[0];
        } else if (block_directive_it->directive == "root") {
            new_server.root = block_directive_it->args[0];
        } else if (block_directive_it->directive == "index") {
            new_server.index = block_directive_it->args[0];
        } else if (block_directive_it->directive == "location") {
            Directive location_directive;
            location_directive.directive = block_directive_it->directive;
            location_directive.args.push_back(block_directive_it->args[0]);
            vector<Directive>::const_iterator nested_directive_it;

            for (nested_directive_it = block_directive_it->block.begin(); nested_directive_it != block_directive_it->block.end(); ++nested_directive_it) {
                location_directive.block.push_back(*nested_directive_it);
            }
            new_server.locations.push_back(location_directive);
        }
    }
    return new_server;
}
