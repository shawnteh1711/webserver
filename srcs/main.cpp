/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: steh <steh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 20:55:27 by steh              #+#    #+#             */
/*   Updated: 2023/03/12 11:32:43 by steh             ###   ########.fr       */
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
    
    // Create a new socket for incoming connections
    m_socket listen_socket;

    // Start listening for incoming connections
    // if (listen(listen_socket.get_socket_fd(), 10) < 0)
    // {
    //     cerr << "Failed to listen for incoming connections" << endl;
    //     return 1;
    // }
    // else
    //     cout << "Listening" << endl;

    // Parse configuration file
    // TODO: implement configuration file parsing

    // Start webserver
    // TODO: implement webserver

    // system("leaks webserv");
    return (EXIT_SUCCESS);
}
