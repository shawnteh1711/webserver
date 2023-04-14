/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: steh <steh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 20:29:55 by leng-chu          #+#    #+#             */
/*   Updated: 2023/04/14 16:13:55 by steh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.hpp"

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
        result.push_back(token);
    return (result);
}

string      removeComment(string line)
{
    line = trimLine(line);
    size_t commentPos = line.find('#');
    if (commentPos != string::npos)
        line = line.substr(0, commentPos);
    return (line);
}

bool    isValidDirective(string directive)
{
    vector<string> acceptedDirectives;
    acceptedDirectives.push_back("http");
    acceptedDirectives.push_back("server");
    acceptedDirectives.push_back("listen");
    acceptedDirectives.push_back("server_name");
    acceptedDirectives.push_back("index");
    acceptedDirectives.push_back("location");
    acceptedDirectives.push_back("root");
    acceptedDirectives.push_back("autoindex");
    acceptedDirectives.push_back("proxy_pass");
    acceptedDirectives.push_back("limit_except");
    acceptedDirectives.push_back("fastcgi_pass");
    acceptedDirectives.push_back("client_max_body_size");
    acceptedDirectives.push_back("internal");
    acceptedDirectives.push_back("error_page");
    acceptedDirectives.push_back("return");
    acceptedDirectives.push_back("deny");
    return (find(acceptedDirectives.begin(), acceptedDirectives.end(), directive) != acceptedDirectives.end());
}

Directive parseDirective(string& line, int lineNum) // i think here got issue
{
    Directive directive;
    directive.line = lineNum;
    size_t argStart = line.find(' ');
    if (argStart != string::npos)
    {
        directive.directive = line.substr(0, argStart);
        bool valid = isValidDirective(directive.directive);
        if (valid == false)
        {
            printf("Invalid directive \"%s\" on line %d\n", directive.directive.c_str(), lineNum);
            exit(EXIT_FAILURE);
        }
        if (directive.directive == "location" || directive.directive == "limit_except" || directive.directive == "upstream")
        {
            size_t  argEnd = line.find_last_not_of("{", line.size() - 2);
            line = line.substr(argStart + 1, argEnd - argStart);
            line = trimLine(line);
        }
        else
        {
            line = line.substr(argStart + 1);
            line = trimLine(line);
            if (line.back() == ';')
                line = line.substr(0, line.size() - 1);
        }
    }
    else
    {
        if (line.back() == ';') // <---- got issue
            line = line.substr(0, line.size() - 1);
        if (line == "internal")
            directive.directive = "internal";
        line.clear();
    }
    directive.args.push_back(line);
    return (directive);
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
		line = trimLine(line);
        if (line.empty() || line[0] == '#' || line == "\t"
				|| line == "\r" || line == " ")
            continue;
        line = removeComment(line);
        Directive directive;
        directive.line = lineNum;
        if (line[0] == '}')
        {
            if (blockStack.empty())
                throw runtime_error("Unmatched closing brace on line " + to_string(lineNum));
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

const vector<Server_Detail>&   Config::getServers() const
{
    return (servers);
}

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

vector<Directive*>  Config::findDirective(Config& config, string directiveName)
{
    vector<Directive*> result;
    for (vector<configItem>::iterator it = config.configItems.begin(); it < config.configItems.end(); ++it)
    {
       findDirectiveRecursive(it->parsed, directiveName, result);
    }
    return (result);
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

Server_Detail Config::createServer(const Directive& directive)
{
	static int i = 1;
    Server_Detail new_server;
    vector<Directive>::const_iterator block_directive_it;

	new_server.id = i++;

    for (block_directive_it = directive.block.begin(); block_directive_it != directive.block.end(); ++block_directive_it)
    {
        if (block_directive_it->directive == "listen")
            new_server.port = block_directive_it->args[0];
        else if (block_directive_it->directive == "server_name")
            new_server.serverName = block_directive_it->args[0];
        else if (block_directive_it->directive == "client_max_body_size")
            new_server.clientMaxBodySize = block_directive_it->args[0];
        else if (block_directive_it->directive == "error_page")
        {
            new_server.errorPage.push_back(block_directive_it->args[0]);
            string code_str = block_directive_it->args[0].substr(0,  block_directive_it->args[0].find(' '));
            int code = atoi(code_str.c_str());
            string file = block_directive_it->args[0].substr(block_directive_it->args[0].find(' ') + 1);
            new_server.errorPageMap.insert(make_pair(code, file));
        }
        else if (block_directive_it->directive == "root")
            new_server.root = block_directive_it->args[0];
        else if (block_directive_it->directive == "index")
            new_server.index = block_directive_it->args[0];
        else if (block_directive_it->directive == "return")
            new_server.redirection = block_directive_it->args[0];
        else if (block_directive_it->directive == "location") {
            Directive location_directive;
            location_directive.directive = block_directive_it->directive;
            location_directive.args.push_back(block_directive_it->args[0]);
            new_server.urlLocation.push_back(location_directive.args[0]);
            vector<Directive>::const_iterator nested_directive_it;

            for (nested_directive_it = block_directive_it->block.begin(); nested_directive_it != block_directive_it->block.end(); ++nested_directive_it)
            {
                if (nested_directive_it->directive == "autoindex" && nested_directive_it->args[0] == "on")
                        new_server.urlIndexOn.push_back(location_directive.args[0]);
                if (nested_directive_it->directive == "limit_except")
                        new_server.urlLimitExcept.insert(make_pair(location_directive.args[0], nested_directive_it->args[0]));
                if (nested_directive_it->directive == "root")
                        new_server.urlRoot.insert(make_pair(location_directive.args[0], nested_directive_it->args[0]));
                if (nested_directive_it->directive == "fastcgi_pass")
                        new_server.urlCgi.insert(make_pair(location_directive.args[0], nested_directive_it->args[0]));
                location_directive.block.push_back(*nested_directive_it);
            }
            new_server.locations.push_back(location_directive);
        }
    }
    return new_server;
}

void    Config::printServer(vector<Server_Detail>& servers)
{
    vector<Server_Detail>::const_iterator server_it;
    vector<Directive>::const_iterator location_it, directive_it;
    vector<string>::const_iterator arg_it, error_it;

    for (server_it = servers.begin(); server_it != servers.end(); ++server_it)
    {
        cout << GREEN << "server {" << endl;
        cout << "\tlisten " << server_it->port << ";" << endl;
        cout << "\tserver_name " << server_it->serverName << ";" << endl;
        cout << "\tclient_max_body_size " << server_it->clientMaxBodySize << ";" << endl;
        cout << "\terror_page: ";
        for (error_it = server_it->errorPage.begin(); error_it != server_it->errorPage.end(); error_it++)
            cout << *error_it << " ";
        cout << endl;
        cout << "\troot " << server_it->root << ";" << endl;
        cout << "\tindex " << server_it->index << ";" << endl;
        cout << "\treturn  " << server_it->redirection << ";" << endl;

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

void	Server_Detail::search(string search)
{
    string	location;
	string	value;

	for (multimap<string, multimap<string, string> >::iterator mit = this->mylocations.begin(); mit != this->mylocations.end(); ++mit)
	{
		for (multimap<string, string>::iterator sit = mit->second.begin(); sit != mit->second.end(); ++sit)
		{
			if (sit->first == search)
			{
				location = mit->first;
				value = sit->second;
	            urlSearch.insert(make_pair(location, value));
			}
		}
	}
}
