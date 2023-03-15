 /* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: steh <steh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/11 18:28:31 by steh              #+#    #+#             */
/*   Updated: 2023/03/14 12:13:10 by steh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

config::config() : _config_file(nullptr)
{
    return ;
}

config::config(ifstream& config_file) : _config_file(&config_file)
{
    cout << "config file constructor" << endl;
    return ;
}

config::config(const config& other)
{
    if (this != &other)
        this->_config_file = other._config_file;
}

config::~config()
{
    return ;
}

config& config::operator=(const config& other)
{
    if (this != &other)
        this->_config_file = other._config_file;
    return (*this);
}
