
// void    Config::print(Config config)
// {
//     std::cout << "status: " << config.status << std::endl;
//     for (size_t i = 0; i < config.errors.size(); i++)
//     {
//         std::cout << "error: " << config.errors[i] << std::endl;  
//     }
//     for (size_t i = 0; i < config.configItems.size(); i++)
//     {
//         std::cout << "config file: " << config.configItems[i].file << std::endl;
//         for (size_t j = 0; j < config.configItems[i].parsed.size(); j++)
//         {
//             std::cout << std::setw(3) << std::left << "line: " << std::setw(3) << config.configItems[i].parsed[j].line;
//             std::cout << std::left << " | directive: " << std::setw(30) << config.configItems[i].parsed[j].directive;

//             for (size_t k = 0; k < config.configItems[i].parsed[j].args.size(); k++)
//             {
//                 std::cout << std::setw(5) << std::left << " |  arg: " << std::setw(10) <<config.configItems[i].parsed[j].args[k];
//             }
//             if (!config.configItems[i].parsed[j].block.empty())
//             {
//                 cout << endl;
//                 std::cout << std::setw(20) << std::left << "parsed block: " << std::endl;
//                 for (size_t k = 0; k < config.configItems[i].parsed[j].block.size(); k++)
//                 {
//                     std::cout << std::setw(3) << std::left << "line: " << std::setw(3) << config.configItems[i].parsed[j].block[k].line;
                    
//                     std::cout << std::left << " | directive: " << std::setw(30) << config.configItems[i].parsed[j].block[k].directive;
//                     for (size_t m = 0; m < config.configItems[i].parsed[j].block[k].args.size(); m++)
//                     {
//                         std::cout << std::setw(5) << std::left << " |  arg: " << std::setw(10) <<config.configItems[i].parsed[j].block[k].args[m] << " ";
//                     }
                    
//                     for (size_t l = 0; l < config.configItems[i].parsed[j].block[k].block.size(); l++)
//                     {
//                         cout << endl;

//                         std::cout << std::setw(3) << std::left << "line: " << std::setw(3) << config.configItems[i].parsed[j].block[k].block[l].line;
//                         std::cout << std::left << " | directive: " << std::setw(30) << config.configItems[i].parsed[j].block[k].block[l].directive;
//                         for (size_t p = 0; p < config.configItems[i].parsed[j].block[k].block[l].args.size(); p++)
//                         {
//                             std::cout << std::setw(5) << std::left << " |  arg: " << std::setw(10) <<config.configItems[i].parsed[j].block[k].block[l].args[p];
//                         }

//                         for (size_t n = 0; n < config.configItems[i].parsed[j].block[k].block[l].block.size(); n++)
//                         {
//                             cout << endl;

//                             std::cout << std::setw(3) << std::left << "line: " << std::setw(3) << config.configItems[i].parsed[j].block[k].block[l].block[n].line;
//                             std::cout << std::left << " | directive: " << std::setw(30) << config.configItems[i].parsed[j].block[k].block[l].block[n].directive;
//                             for (size_t o = 0; o < config.configItems[i].parsed[j].block[k].block[l].args.size(); o++)
//                             {
//                                 std::cout << std::setw(5) << std::left << " |  arg: " << std::setw(10) <<config.configItems[i].parsed[j].block[k].block[l].block[n].args[o];
//                             }
//                         }
//                     }
//                 }
//             }
//             cout << endl;
//         }
//     }
// }

// for (vector<Server>::iterator server_it = config.servers.begin(); server_it != config.servers.end(); server_it++)
// {
//     cout << "Server Name: " << server_it->serverName << endl;
//     cout << "Port: " << server_it->port << endl;
//     cout << "Client Max Body Size: " << server_it->clientMaxBodySize << endl;
//     cout << "Error Page: " << server_it->errorPage << endl;
//     for (vector<Directive>::iterator location_it = server_it->locations.begin(); location_it != server_it->locations.end(); location_it++)
//     {
//         for (vector<string>::iterator arg_it = location_it->args.begin(); arg_it != location_it->args.end(); arg_it++)
//         {
//             cout << "path: " << *arg_it << endl;
//         }
//         for (vector<Directive>::iterator directive_it = location_it->block.begin(); directive_it != location_it->block.end(); directive_it++)
//         {
//             cout << "Name: " << directive_it->directive << " | Value: " << directive_it->args[0] << endl;
//         }
//         cout << endl;
//     }
// }


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

// vector<Directive> parseBlock(ifstream& file, int& lineNum, Directive& parentDirective)
// {
//     vector<Directive> block;
//     vector<Directive>* currentBlock = &block;
//     stack<vector<Directive>*> blockStack;
//     string line;
//     bool    isServerBlock = false;
//     size_t  serverBlockLevel = -1;
//     // bool    isFirstServerDirective = true;

//     block = parentDirective.block;
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
//             line = line.substr(0, commentPos);
//         if (line[0] == '}')
//         {
//             if (blockStack.empty())
//                 throw runtime_error("Unmatched closing brace on line " + to_string(lineNum));
//             currentBlock = blockStack.top();
//             blockStack.pop();
//             return block;
//         }
//         // std::cout << "parent directive: " << parentDirective.directive << std::endl;
//         if ((line.find(" server ") != string::npos || line.find("server ") == 0 || line.find(" server") == line.size() - 7) && parentDirective.directive != "server")
//         {
//             // cout << line << endl;
//             if (serverBlockLevel == (size_t)-1)
//             {
//                 isServerBlock = true;
//                 serverBlockLevel = blockStack.size();
//             }
//             Directive serverDirective;
//             serverDirective.directive = "server";
//             serverDirective.line = lineNum;
//             if (line.back() == ';')
//                 line = line.substr(0, line.size() - 1);
//             size_t argStart = line.find(' ');
//             line = line.substr(argStart + 1);
//             serverDirective.args.push_back(line);
//             serverDirective.block = vector<Directive>();
//             blockStack.top()->push_back(serverDirective);
//             currentBlock = &blockStack.top()->back().block;
//             blockStack.push(currentBlock);
//             continue;
//         }
//         else if (serverBlockLevel != (size_t)-1 && blockStack.size() == serverBlockLevel)
//         {
//             // cout << "here" << endl;
//             isServerBlock = false;
//             serverBlockLevel = -1;
//             currentBlock = blockStack.top();
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
//             directive.block = parseBlock(file, lineNum, parentDirective); // Pass the parent directive to the child block
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
//         throw runtime_error("Unmatched opening brace in file");
//     return block;
// }
