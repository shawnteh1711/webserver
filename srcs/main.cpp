/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: steh <steh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 17:53:17 by leng-chu          #+#    #+#             */
/*   Updated: 2023/04/13 16:50:45 by leng-chu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

//int	main()
//{
//	Server	server = Server("0.0.0.0", 8080);
//	signal(SIGTSTP, Server::sig_handler);
//	server.startListen();
//	return (0);
//}
//
int	alecprintf(vector<Server_Detail> *d_servers)
{
	int max_server = 0;
	vector<Server_Detail>::iterator it = d_servers->begin();
	vector<Server_Detail>::iterator ite = d_servers->end();
	vector<string>::iterator sit;
	vector<string>::iterator site;
	int i = 1;
	while (it != ite)
	{
		cout << "====== Server " << i++ << " =======" << endl;
		cout << "serverName: " << it->serverName << endl;
		cout << "serverName ID: " << it->id << endl;
		max_server = max_server < it->id ? it->id : max_server;
		cout << "port: " << it->port << endl;
		cout << "clientMaxBodySize: " << it->clientMaxBodySize << endl;
		sit = it->errorPage.begin();
		site = it->errorPage.end();
		while (sit != site)
		{
			cout << "errorPage: " << *sit << endl;
			sit++;
		}
		cout << "root: " << it->root << endl;
		cout << "index: " << it->index << endl;
		// cout << "autoIndex: " << it->autoIndex << endl;
		cout << "redirection: " << it->redirection << endl;

		vector<Directive>::iterator dit = it->locations.begin();
		vector<Directive>::iterator dite = it->locations.end();
		vector<Directive>::iterator bt, bte, bbt, bbte;

		// vector<Directive> locations is start
		string mainkey;
		while (dit != dite)
		{
			cout << "directive: " << dit->directive << endl;
			sit = dit->args.begin();
		//	site = dit->args.end();
		//	while (sit != site)
		//	{
				cout << "LOCATION ARG PATH: " << *sit << endl;
		//		it->mylocations[*sit] = multimap<string, string>();
				it->mylocations.insert(make_pair(*sit, multimap<string, string>()));
				mainkey = *sit;
		//		sit++;
		//	}
			bt = dit->block.begin();
			bte = dit->block.end();
			multimap<string, string> tmp;
			while ( (bt != bte))
 			{
				// here keep looping;
				cout << "bt->directive: " << bt->directive << endl;
				sit = bt->args.begin();
	//			site = bt->args.end();
	//			while (sit != site)
					cout << "bt->arg: " << *sit << endl;
	//				cout << "bt->arg: " << *sit++ << endl;
		//		tmp[bt->directive] = *sit;
				tmp.insert(make_pair(bt->directive, *sit));
				bbt = bt->block.begin();
				bbte = bt->block.end();
				while (bbt != bbte)
				{
					cout << "bt2->directive: " << bbt->directive << endl;
					sit = bbt->args.begin();
	//				site = bbt->args.end();
	//				while (sit != site)
						cout << "bt2->arg: " << *sit << endl;
	//					cout << "bt2->arg: " << *sit++ << endl;
		//			tmp[bbt->directive] = *sit;
					tmp.insert(make_pair(bbt->directive, *sit));
					bbt++;
				}
				//it->mylocations[mainkey] = tmp; 
				it->mylocations.find(mainkey)->second = tmp;
				bt++;
			}
			dit++;
		}
		it++;
	}
	return (max_server);
}

void	alecprintlocation(vector<Server_Detail> & d_servers)
{
	int i = 1;
	vector<Server_Detail>::iterator s = d_servers.begin();
	vector<Server_Detail>::iterator se = d_servers.end();
	multimap<string, multimap<string, string> >::iterator it, ite;
	multimap<string, string>::iterator tw, twe;
	while (s != se)
	{
		cout << "====SERVER " << i++ << "====" << endl;
		it = s->mylocations.begin();
		ite = s->mylocations.end();
		while (it != ite)
		{
			cout << "Location Path: " << it->first << endl;
			tw = it->second.begin();
			twe = it->second.end();
			while (tw != twe)
			{
				cout << tw->first << ": " << tw->second << endl;
				tw++;
			}
			it++;
		}
		s++;
	}
}

void	*startListen(void *arg)
{
	Server	*server = static_cast<Server*>(arg);
	server->startListen();
	return (NULL);
}

void	setup(vector<Server_Detail> & d_servers, char **argv)
{
    string filename = argv[1];
	
	Config      			config;
	vector<Directive*>		mydirective;
	config = config.parseConfigFile(filename);
//	config.print(config);
	d_servers = config.setServer(config); 
//	config.printServer(d_servers);
	// alecprintlocation(d_servers);

}

void	testing_limit_except(vector<Server_Detail> & d_servers)
{
	string	disable_path;
	string	enable_method;

	for (vector<Server_Detail>::iterator it = d_servers.begin(); it != d_servers.end(); ++it)
	{
		cout << RED << "Server: " << it->id << endl;
		for (map<string, multimap<string, string> >::iterator mit = it->mylocations.begin(); mit != it->mylocations.end(); ++mit)
		{
			// cout << RED << "Location: " << mit->first << endl;
			for (multimap<string, string>::iterator sit = mit->second.begin(); sit != mit->second.end(); ++sit)
			{
				// cout << RED << "Directive: " << sit->first << ", Argument: " << sit->second << RESET<< endl;
				if (sit->first == "limit_except")
				{
					cout << RED << "Location: " << mit->first << endl;
					disable_path = mit->first;
					cout << RED << "Directive:|" << sit->first << "|, Argument: " << sit->second << RESET<< endl;
					enable_method = sit->second;
				}
			}
		}
	}
	string	request_path;
	string  request_method;

	request_path = "/example";
	request_method = "POST";
	if (request_path == disable_path)
	{
		cout << "enable_method: " << enable_method << endl;
		if (enable_method.find(request_method) != string::npos)
			cout << request_method << " method is able" << endl;
		else
			cout << "HTTP/1.1 405 Method Not Allowed\r\n\r\n" << endl;
	}

	cout << RESET << endl;
}

void	printServerDetail(vector<Server_Detail>	d_servers)
{
	vector<Server_Detail>::iterator it, ite;
	map<string, string>::iterator	it2;
	vector<string>::iterator 		it3;
	map<int, string>::iterator		it4;

	//  for (it2 = d_servers[0].urlMethod.begin(); it2 != d_servers[0].urlMethod.end(); ++it2)
	//  {
	//  	cout << RED << "it2->first: " << it2->first << endl;
	//  	cout << RED << "it2->second: " << it2->second << endl;
	//  }
	it = d_servers.begin();
	ite = d_servers.end();
	while (it != ite)
	{

		cout << "SERVER: " << it->id << endl;
		cout << "serverName: " << it->serverName << endl;
		cout << "port: " << it->port << endl;
		cout << "clientMaxBodySize: " << it->clientMaxBodySize << endl;
		for (it3 = it->errorPage.begin(); it3 != it->errorPage.end(); it3++)
		{
			cout << "errorPage: " << *it3 << endl;
		}
		cout << "root: " << it->root << endl;
		cout << "index: " << it->index << endl;
		cout << "redirection: " << it->redirection << endl;
		for (it3 = it->urlIndexOn.begin();it3 != it->urlIndexOn.end();it3++)
			cout << RED << "urlIndexOn: "  << *it3 << RESET << endl;
		for (it2 = it->urlLimitExcept.begin();it2 != it->urlLimitExcept.end();it2++)
			cout << GREEN << "urlLimitExcept: " << it2->first << " " << it2->second << RESET << endl;
		for (it2 = it->urlRoot.begin();it2 != it->urlRoot.end();it2++)
			cout << BLUE << "urlRoot: " << it2->first << " " << it2->second << RESET  << endl;
		for (it2 = it->urlCgi.begin();it2 != it->urlCgi.end();it2++)
			cout << BLUE << "urlCgi: " << it2->first << " " << it2->second << RESET  << endl;
		for (it3 = it->urlLocation.begin(); it3 != it->urlLocation.end(); it3++)
			cout << YELLOW << "urlLocation: " << *it3 << RESET << endl;
		for	(it4 = it->errorPageMap.begin(); it4 != it->errorPageMap.end(); it4++)
			cout << YELLOW << "errorPageMap: " << it4->first << " " << it4->second << endl;
		it++;
	}
	
}

void	test(char **argv)
{
	vector<Server_Detail>	d_servers;
//	vector<Server*>		servers;
	setup(d_servers, argv);
	// max_servers = alecprintf(&d_servers);
	signal(SIGINT, Server::sig_handler);
	signal(SIGTSTP, Server::sig_handler);
	Server	webserver(d_servers);
	// testing_limit_except(d_servers); // this one ?
	//  d_servers[0].search("fastcgi_pass");
	//printServerDetail(d_servers);
	webserver.startListen();
}

bool isValidConfigFile(const char* path)
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

    // Additional checks to ensure that the file is in the correct format can be performed here
    return (true);
}


int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        cerr << "error" << endl;
        cerr << "Please enter: ./a.out [conf path]" << endl;
        return (1);
    }
	if (!isValidConfigFile(argv[1]))
		return (1);
	test(argv);
	// system("leaks webserv");
    return 0;
}
