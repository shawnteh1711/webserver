/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leng-chu <-chu@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 17:53:17 by leng-chu          #+#    #+#             */
/*   Updated: 2023/03/23 17:36:46 by leng-chu         ###   ########.fr       */
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

void	test(char **argv)
{
    string filename = argv[1];

    Config      			config;
    vector<Directive*>		mydirective;
    vector<Server_Detail>	d_servers;
	
	config = config.parseConfigFile(filename);
//	config.print(config);
	d_servers = config.setServer(config); 
//	config.printServer(d_servers);
	vector<Server_Detail>::iterator it = d_servers.begin();
	vector<Server_Detail>::iterator ite = d_servers.end();
	int i = 1;
	while (it != ite)
	{
		cout << "====== Server " << i++ << " =======" << endl;
		cout << "serverName: " << it->serverName << endl;
		cout << "port: " << it->port << endl;
		cout << "clientMaxBodySize: " << it->clientMaxBodySize << endl;
		cout << "errorPage: " << it->errorPage << endl;
		cout << "root: " << it->root << endl;
		cout << "index: " << it->index << endl;

		vector<Directive>::iterator dit = it->locations.begin();
		vector<Directive>::iterator dite = it->locations.end();
		vector<string>::iterator sit;
		vector<string>::iterator site;
		vector<Directive>::iterator bt;
		vector<Directive>::iterator bte;
		while (dit != dite)
		{
			cout << "directive: " << dit->directive << endl;
			sit = dit->args.begin();
			site = dit->args.end();
			while (sit != site)
				cout << "arg: " << *sit++ << endl;
			dit++;
			cout << "line: " << dit->line << endl;
			bt = dit->block.begin();
			bte = dit->block.end();
		//	while (bt != bte)
		//	{
		//		cout << "block directive: " << bt->directive << endl;
		//		cout << "block line: " << bt->line << endl;
		//		bt++;
		//	}
		}
		it++;
	}

	// alec
	//Server server = Server(d_servers[0].serverName, stoi(d_servers[0].port));
//	cout << "serverName: " << d_servers[0].serverName << endl;
//	int pos = d_servers[0].serverName.find(" ");
//	string newlol = d_servers[0].serverName.substr(0, pos);
//	cout << "newlol: " << newlol << endl;
//	Server	server = Server("128.100.125.127", stoi(d_servers[0].port));
//	Server	server = Server("127.0.0.1", 8080);
//	signal(SIGINT, Server::sig_handler);
//	signal(SIGTSTP, Server::sig_handler);
//	server.startListen();
}

int main(int argc, char* argv[])
{
	// shawn
    if (argc != 2)
    {
        cerr << "error" << endl;
        cerr << "Please enter: ./a.out [conf path]" << endl;
        return (1);
    }
	test(argv);
//	system("leaks webserv");
    return 0;
}
