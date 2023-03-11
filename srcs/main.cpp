/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: steh <steh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 20:55:27 by steh              #+#    #+#             */
/*   Updated: 2023/03/11 19:42:05 by steh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int main(int argc, char* argv[])
{
    string  config;

    if (argc < 2 || argc > 2)
    {
        cerr << "./webserv [configuration file]" << endl;
        return (EXIT_FAILURE);
    }
    config = argv[1];
    ifstream config_file(config);
    if (!config_file.is_open())
    {
        cerr << "Error: Failed to open configuration file" << endl;
        return (EXIT_FAILURE);
    }
    else
    {
        cout << "reading...." << endl;
        server server(config_file);
    }
    
    // Parse configuration file
    // TODO: implement configuration file parsing

    // Start webserver
    // TODO: implement webserver

    // system("leaks webserv");
    return (EXIT_SUCCESS);
}
