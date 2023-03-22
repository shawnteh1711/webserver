/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leng-chu <-chu@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 17:53:17 by leng-chu          #+#    #+#             */
/*   Updated: 2023/03/22 21:03:50 by leng-chu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "server.hpp"
#include "parse.hpp"

//int	main()
//{
//	Server	server = Server("0.0.0.0", 8080);
//	signal(SIGINT, Server::sig_handler);
//	signal(SIGTSTP, Server::sig_handler);
//	server.startListen();
//	return (0);
//}

int main(int argc, char* argv[])
{
	// shawn
    if (argc != 2)
    {
        cerr << "error" << endl;
        cerr << "Please enter: ./a.out [conf path]" << endl;
        return (1);
    }
    string filename = argv[1];

    Config      config;
    vector<Directive*>  mydirective;
    vector<Server_Detail>      d_servers;

    config = config.parseConfigFile(filename);
    // config.print(config);
    d_servers = config.setServer(config); 
    config.printServer(d_servers);
    cout << d_servers[0].port << endl;

	// alec
	//Server server = Server(d_servers[0].serverName, stoi(d_servers[0].port));
	cout << "serverName: " << d_servers[0].serverName << endl;
	int pos = d_servers[0].serverName.find(" ");
	string newlol = d_servers[0].serverName.substr(0, pos);
	cout << "newlol: " << newlol << endl;
//	Server	server = Server("128.100.125.127", stoi(d_servers[0].port));
	Server	server = Server("127.0.0.1", 8080);
	signal(SIGINT, Server::sig_handler);
	signal(SIGTSTP, Server::sig_handler);
	server.startListen();
    return 0;
}
