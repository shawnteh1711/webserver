#include "parse8.hpp"

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

// vector<Directive> parseBlock(ifstream& file, int& lineNum, Directive& currentDirective)
// {
//     vector<Directive> block;
//     vector<Directive>* currentBlock = &block;
//     stack<vector<Directive>*> blockStack;
//     string line;

//     block = currentDirective.block;
//     blockStack.push(&block);
//     while (getline(file, line))
//     {
//         lineNum++;
//         line = trimLine(line);
//         if (line.empty() || line[0] == '#')
//             continue;
//         Directive directive;
//         directive.line = lineNum;

//         size_t commentPos = line.find('#');
//         if (commentPos != string::npos)
//         {
//             line = line.substr(0, commentPos);
//         }
//         if (line[0] == '}')
//         {
//             if (blockStack.empty())
//             {
//                 throw runtime_error("Unmatched closing brace on line " + to_string(lineNum));
//             }
//             currentBlock = blockStack.top();
//             blockStack.pop();
//             return block;
//         }
//         size_t blockStart = line.find('{');
//         if (blockStart != string::npos)
//         {
//             if (line.find("location") != string::npos || line.find("limit_except") != string::npos)
//             {
//                 size_t  argStart = line.find(' ');
//                 size_t  argEnd = line.find_last_not_of(' ', line.size() - 2);
//                 directive.directive = line.substr(0, argStart);
//                 line = line.substr(argStart + 1, argEnd - argStart);
//                 line = trimLine(line);
//                 directive.args.push_back(line);
//             }
//             else
//             {
//                 directive.directive = line.substr(0, blockStart);
//                 directive.args.push_back(line.substr(blockStart + 1));
//             }
//             directive.block = parseBlock(file, lineNum, directive);
//         }
//         else
//         {
//             size_t argStart = line.find(' ');
//             directive.directive = line.substr(0, argStart);
//             size_t argEnd = line.find_last_not_of(' ', line.size() - 1);
//             line = line.substr(argStart + 1, argEnd - argStart);
//             line = trimLine(line);
//             if (line.back() == ';')
//                 line = line.substr(0, line.size() - 1);
//             directive.args.push_back(line);
//         }
//         currentBlock->push_back(directive);
//         if (line[line.size() - 1] == '{')
//         {
//             blockStack.push(currentBlock);
//             currentBlock = &currentBlock->back().block;
//         }
//     }
//     if (!blockStack.empty())
//     {
//         throw runtime_error("Unmatched opening brace in file");
//     }
//     return block;
// }

vector<Directive> parseBlock(ifstream& file, int& lineNum, Directive& parentDirective)
{
    vector<Directive> block;
    vector<Directive>* currentBlock = &block;
    stack<vector<Directive>*> blockStack;
    string line;

    block = parentDirective.block;
    blockStack.push(&block);
    while (getline(file, line))
    {
        lineNum++;
        line = trimLine(line);
        if (line.empty() || line[0] == '#')
            continue;
        Directive directive;
        directive.line = lineNum;

        size_t commentPos = line.find('#');
        if (commentPos != string::npos)
        {
            line = line.substr(0, commentPos);
        }
        if (line[0] == '}')
        {
            if (blockStack.empty())
            {
                throw runtime_error("Unmatched closing brace on line " + to_string(lineNum));
            }
            currentBlock = blockStack.top();
            blockStack.pop();
            return block;
        }

        // Check if we are encountering a new server directive
        if ((line.find(" server ") != string::npos || line.find("server ") == 0 || line.find(" server") == line.size() - 7) && parentDirective.directive != "server")
        {
            // If so, create a new parent directive and push it onto the block stack
            Directive serverDirective;
            serverDirective.directive = "server";
            serverDirective.line = lineNum;
            if (line.back() == ';')
                line = line.substr(0, line.size() - 1);
            size_t argStart = line.find(' ');
            directive.directive = line.substr(0, argStart);
            size_t argEnd = line.find_last_not_of(' ', line.size() - 1);
            line = line.substr(argStart + 1, argEnd - argStart);
            serverDirective.args.push_back(line);
            serverDirective.block = vector<Directive>();
            block.push_back(serverDirective);
            currentBlock = &currentBlock->back().block;
            blockStack.push(currentBlock);
            parentDirective = serverDirective; // Update the parent directive to be the new server directive
            continue;
        }

        size_t blockStart = line.find('{');
        if (blockStart != string::npos)
        {
            if (line.find("location") != string::npos || line.find("limit_except") != string::npos)
            {
                size_t  argStart = line.find(' ');
                size_t  argEnd = line.find_last_not_of(' ', line.size() - 2);
                directive.directive = line.substr(0, argStart);
                line = line.substr(argStart + 1, argEnd - argStart);
                line = trimLine(line);
                directive.args.push_back(line);
            }
            else
            {
                directive.directive = line.substr(0, blockStart);
                directive.args.push_back(line.substr(blockStart + 1));
            }
            directive.block = parseBlock(file, lineNum, parentDirective); // Pass the parent directive to the child block
        }
        else
        {
            size_t argStart = line.find(' ');
            directive.directive = line.substr(0, argStart);
            size_t argEnd = line.find_last_not_of(' ', line.size() - 1);
            line = line.substr(argStart + 1, argEnd - argStart);
            line = trimLine(line);
            if (line.back() == ';')
                line = line.substr(0, line.size() - 1);
            directive.args.push_back(line);
        }
        currentBlock->push_back(directive);
        if (line[line.size() - 1] == '{')
        {
            blockStack.push(currentBlock);
            currentBlock = &currentBlock->back().block;
        }
    }
    if (!blockStack.empty())
    {
        throw runtime_error("Unmatched opening brace in file");
    }
    return block;
}


Config Config::parseConfigFile(string filename)
{
    Config config;

    config.status = "ok";
    config.errors.push_back("no error yet");
    configItem item;
    item.file = filename;
    item.errors.push_back("no error yet");
    ifstream file(filename);
    if (file.is_open())
    {
        string line;
        int lineNum = 0;
        while (getline(file, line))
        {
            lineNum++;
            line = trimLine(line);
            if (line.empty() || line[0] == '#')
                continue;
            Directive directive;
            directive.line = lineNum;
            size_t commentPos = line.find('#');
            if (commentPos != string::npos)
            {
                line = line.substr(0, commentPos);
            }
            line = trimLine(line);
            size_t blockStart = line.find('{');
            if (blockStart != string::npos)
            {
                directive.directive = line.substr(0, blockStart);
                directive.args.push_back(line.substr(blockStart + 1));
                // directive.block = parseBlock(file, lineNum);
                directive.block = parseBlock(file, lineNum, directive);
            }
            else
            {
                size_t argStart = line.find(' ');
                directive.directive = line.substr(0, argStart);
                size_t argEnd = line.find_last_not_of(' ', line.size() - 1);
                line = line.substr(argStart + 1, argEnd - argStart);
                line = trimLine(line);
                if (line.back() == ';')
                    line = line.substr(0, line.size() - 1);
                directive.args.push_back(line);
            }            
            item.parsed.push_back(directive);
        }
        file.close();
        config.configItems.push_back(item);
    }
    else
    {
        config.errors.push_back("Unable to open file " + filename);
        config.status = "error";
    }
    return config;
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
    for (vector<string>::const_iterator it = config.errors.begin(); it != config.errors.end(); ++it)
    {
        std::cout << "error: " << *it << std::endl;
    }
    for (std::vector<configItem>::const_iterator it = config.configItems.begin(); it != config.configItems.end(); ++it)
    {
        std::cout << "config file: " << it->file << std::endl;
        for (std::vector<Directive>::const_iterator it2 = it->parsed.begin(); it2 != it->parsed.end(); ++it2)
        {
            print_config_item(*it2);
        }
    }
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        cerr << "error" << endl;
        cerr << "Please enter: ./a.out [conf path]" << endl;
        return (1);
    }
    string filename = argv[1];

    Config      config;
    vector<Directive*>  mydirective;


    config = config.parseConfigFile(filename);
    config.print(config);


    // for (vector<configItem>::iterator it = config.configItems.begin(); it != config.configItems.end(); it++)
    // {
    //     for (vector<Directive>::iterator parse_it = it->parsed.begin(); parse_it != it->parsed.end(); parse_it++)
    //     {
    //         for (vector<Directive>::iterator block_it = parse_it->block.begin(); block_it != parse_it->block.end(); block_it++)
    //         {
    //             if (block_it->directive == "server")
    //             {
    //                 Server  new_server;
    //                 for (vector<Directive>::iterator inside_block_it = block_it->block.begin(); inside_block_it != block_it->block.end(); inside_block_it++)
    //                 {
    //                     Directive*   directive = new Directive;

    //                     if (inside_block_it->directive == "listen")
    //                         new_server.port = inside_block_it->args[0];
    //                     else if (inside_block_it->directive == "server_name")
    //                         new_server.serverName = inside_block_it->args[0];
    //                     else if (inside_block_it->directive == "client_max_body_size")
    //                         new_server.clientMaxBodySize = stoi(inside_block_it->args[0]);
    //                     else if (inside_block_it->directive == "error_page")
    //                         new_server.errorPage = inside_block_it->args[0];
    //                     else if (inside_block_it->directive == "location")
    //                     {
    //                         directive->args.push_back(inside_block_it->args[0]);
    //                         for (vector<Directive>::iterator nested_block_it = inside_block_it->block.begin(); nested_block_it != inside_block_it->block.end(); nested_block_it++)
    //                         {
    //                             directive->block.push_back(*nested_block_it);
    //                         }
    //                     }
    //                     new_server.locations.push_back(*directive);
    //                 }
    //                 config.servers.push_back(new_server);
    //             }
    //         }
    //     }
    // }

    // vector<Server>::const_iterator server_it;
    // vector<Directive>::const_iterator location_it, directive_it;
    // vector<string>::const_iterator arg_it;

    // for (server_it = config.servers.begin(); server_it != config.servers.end(); ++server_it)
    // {
    //     cout << "Server Name: " << server_it->serverName << endl;
    //     cout << "Port: " << server_it->port << endl;
    //     cout << "Client Max Body Size: " << server_it->clientMaxBodySize << endl;
    //     cout << "Error Page: " << server_it->errorPage << endl;

    //     for (location_it = server_it->locations.begin(); location_it != server_it->locations.end(); ++location_it)
    //     {
    //         for (arg_it = location_it->args.begin(); arg_it != location_it->args.end(); ++arg_it) {
    //             cout << "path: " << *arg_it << endl;
    //         }
    //         for (directive_it = location_it->block.begin(); directive_it != location_it->block.end(); ++directive_it) {
    //             cout << "Name: " << directive_it->directive << " | Value: " << directive_it->args[0] << endl;
    //         }
    //         cout << endl;
    //     }
    // }

    return 0;
}
