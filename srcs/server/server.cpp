/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: steh <steh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/11 17:43:00 by steh              #+#    #+#             */
/*   Updated: 2023/03/11 18:08:35 by steh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

server::server()
{
    cout << "server default constructor" << endl;
}

server::server(ifstream& config_file)
{
    (void)config_file;
    cout << "server constructor" << endl;
}

void    server::parse(ifstream& config_file)
{
    (void)config_file;
    cout << "server receive config" << endl;
}
