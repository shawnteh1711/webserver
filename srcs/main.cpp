/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leng-chu <-chu@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 17:53:17 by leng-chu          #+#    #+#             */
/*   Updated: 2023/03/24 15:50:27 by leng-chu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "server.hpp"
#include "parse8.hpp"

//int	main()
//{
//	Server	server = Server("0.0.0.0", 8080);
//	signal(SIGINT, Server::sig_handler);
//	signal(SIGTSTP, Server::sig_handler);
//	server.startListen();
//	return (0);
//}
//
void	alecprintf(vector<Server_Detail> *d_servers)
{
	vector<Server_Detail>::iterator it = d_servers->begin();
	vector<Server_Detail>::iterator ite = d_servers->end();
	vector<string>::iterator sit;
	vector<string>::iterator site;
//	vector<Direcrive>::iterator dit;
//	vector<Directive>::iterator dite;
	int i = 1;
	while (it != ite)
	{
		cout << "====== Server " << i++ << " =======" << endl;
		cout << "serverName: " << it->serverName << endl;
		cout << "port: " << it->port << endl;
		cout << "clientMaxBodySize: " << it->clientMaxBodySize << endl;
		//cout << "errorPage: " << it->errorPage << endl;
		sit = it->errorPage.begin();
		site = it->errorPage.end();
		cout << "errorPage: " << it->errorPage[0] << endl;
		// errorPage vector got problem.
		while (sit != site)
		{
			cout << "errorPage: " << *sit << endl;
			sit++;
		}
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
//			bt = dit->block.begin();
//			bte = dit->block.end();
//			while (bt != bte)
//			{
//				cout << "block directive: " << bt->directive << endl;
//				cout << "block line: " << bt->line << endl;
//				bt++;
//			}
		}
		it++;
	}
}

void	get_ip(void)
{
	const char* server_name = "alec42.com";
    struct addrinfo hints, *res;

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

	memset(&hints, 0, sizeof hints);

    if (getaddrinfo(server_name, NULL, &hints, &res) != 0)
    {
        std::cerr << "Failed to get address info for " << server_name << std::endl;
      //  return 1;
    }

    const sockaddr_in* addr = reinterpret_cast<const sockaddr_in*>(res->ai_addr);
    char ip_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &addr->sin_addr, ip_str, INET_ADDRSTRLEN);
    std::cout << "The IP address of " << server_name << " is " << ip_str << std::endl;

    freeaddrinfo(res);

 //   return 0;
}

int	get_ip2(void)
{
	const char* hostname = "alec42.com";
    const char* port = "8080";

    struct addrinfo hints, *res, *p;
	memset(&hints, 0, sizeof hints);

    // Set up hints for getaddrinfo()
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    // Call getaddrinfo() to get a list of address structures
    int status = getaddrinfo(hostname, port, &hints, &res);
    if (status != 0) {
        std::cerr << "getaddrinfo error: " << gai_strerror(status) << '\n';
        return 1;
    }

    // Loop through the list of address structures and try to connect
    int sockfd = -1;
    for (p = res; p != NULL; p = p->ai_next) {
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sockfd == -1) {
            continue;
        }

        status = connect(sockfd, p->ai_addr, p->ai_addrlen);
        if (status == -1) {
            close(sockfd);
            sockfd = -1;
            continue;
        }

        break;  // connection successful
    }

    // Clean up the address structures
    freeaddrinfo(res);

    // Check if we were able to connect
    if (sockfd == -1) {
        std::cerr << "Unable to connect to " << hostname << ":" << port << '\n';
        return 1;
    }

    std::cout << "Connected to " << hostname << ":" << port << '\n';

    // Do something with the socket...

    close(sockfd);
    return 0;
}

void	test(char **argv)
{
    string filename = argv[1];

    Config      			config;
    vector<Directive*>		mydirective;
    vector<Server_Detail>	d_servers;
	
	config = config.parseConfigFile(filename);
	config.print(config);
	d_servers = config.setServer(config); 
	config.printServer(d_servers);
//	alecprintf(&d_servers);

//	get_ip();
//	get_ip2();

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
