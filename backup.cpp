
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
