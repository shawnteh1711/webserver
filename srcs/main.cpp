/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: steh <steh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 17:53:17 by leng-chu          #+#    #+#             */
/*   Updated: 2023/04/13 20:28:49 by leng-chu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

static void	setup(vector<Server_Detail> & d_servers, char **argv)
{
	Config				config;
	vector<Directive*>	mydirective;
    string				filename = argv[1];
	
	config = config.parseConfigFile(filename);
	d_servers = config.setServer(config); 
}

static void	launchWebServer(char **argv)
{
	vector<Server_Detail>	d_servers;
	setup(d_servers, argv);

	signal(SIGINT, Server::sig_handler);
	signal(SIGTSTP, Server::sig_handler);
	
	Server	webserver(d_servers);
	webserver.startListen();
}

static bool isValidConfigFile(const char* path)
{
    ifstream configFile(path);
    if (!configFile.is_open()) {
        cerr << "Error: Unable to open config file" << endl;
        return (false);
    }
    // Check if the file is empty
    if (configFile.peek() == ifstream::traits_type::eof()) {
        cerr << "Error: Config file is empty" << endl;
        return (false);
    }
	configFile.close();
    return (true);
}


int main(int argc, char* argv[])
{
	try
	{
		if (argc != 2)
			throw static_cast<string>("Error! Please enter: ./a.out [conf path]");
		if (!isValidConfigFile(argv[1]))
			throw static_cast<string>("Error Config");
		launchWebServer(argv);
	}
    catch (string error)
	{
		cerr << error << endl;
		return (1);
	}
    return 0;
}
