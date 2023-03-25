/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leng-chu <-chu@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 17:53:17 by leng-chu          #+#    #+#             */
/*   Updated: 2023/03/25 19:14:49 by leng-chu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
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
		cout << "autoIndex: " << it->autoIndex << endl;
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

void	get_ip(void)
{
	const char* server_name = "example.com";
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
	const char* hostname = "127.0.0.1";
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

	cout << "loop start" << endl;
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
		cout << "connection successful" << endl;

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

# define PORT 80

void	get_ip3(void)
{
	int	BACKLOG = 10;
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
	const char* response = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 13\n\nHello world!\n";

    int response_len = strlen(response);

    // Create a socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options to allow reuse of the address and port
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    // Bind the socket to the specified port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, BACKLOG) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
	cout << "start accept loop" << endl;
    // Accept incoming connections and handle them
    while (true) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept failed");
            exit(EXIT_FAILURE);
        }

        // Read the HTTP request
        int n = read(new_socket, buffer, 1024);
		cout << "reading" << endl;
        if (n < 0) {
            perror("read failed");
            exit(EXIT_FAILURE);
        }

        // Send the HTTP response
		cout << "now sending msg!" << endl;
        n = write(new_socket, response, response_len);
        if (n < 0) {
            perror("write failed");
            exit(EXIT_FAILURE);
        }

        // Close the socket
        close(new_socket);
    }
	close(server_fd);
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
//	alecprintlocation(d_servers);

}

void	test(char **argv)
{
	vector<Server_Detail>	d_servers;
	int						max_servers;

	setup(d_servers, argv);
	max_servers = alecprintf(&d_servers);
	cout << "max_servers: " << max_servers << endl;
	vector<Server*>		servers;
	vector<pthread_t>	threads(max_servers);
	vector<Server_Detail>::iterator it, ite;
	it = d_servers.begin();
	ite = d_servers.end();
	signal(SIGINT, Server::sig_handler);
	signal(SIGTSTP, Server::sig_handler);
	//servers.push_back(Server("127.0.0.1", 8080));
	//servers[0].startListen();
	//Server	server("127.0.0.1", stoi(it->port));
	//server.startListen();
//	servers[0].startListen();
//	int i = 0;
//	while (it != ite)
//	{
//		cout << "it->port: " << it->port << endl;
//		servers.push_back(new Server("193.184.216.34", stoi(it->port)));
//		pthread_create(&threads[i], NULL, startListen, servers[i]);
//		i++;
//		it++;
//	}
	cout << "end of loop" << endl;
//	for (int s = 0; s < max_servers; s++)
//		pthread_join(threads[s], NULL);
//	pthread_create(&threads[0], NULL, startListen, &servers[0]);
//	pthread_create(&threads[1], NULL, startListen, &servers[1]);
//	pthread_join(threads[0], NULL);
//	pthread_join(threads[1], NULL);
//	servers[1]->startListen();

//	get_ip();
//	get_ip2();
//	get_ip3();

	// alec
	Server	server = Server("164.2.2.1", 8080);
//	Server	server2 = Server("127.0.0.1", 5000);
//	signal(SIGINT, Server::sig_handler);
//	signal(SIGTSTP, Server::sig_handler);

//	server.startListen();
//	pthread_t	thread1, thread2;
//	pthread_create(&thread1, NULL, startListen, &server);
//	pthread_create(&thread2, NULL, startListen, &server2);

//	pthread_join(thread1, NULL);
//	pthread_join(thread2, NULL);
	(void)argv;
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
