
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
