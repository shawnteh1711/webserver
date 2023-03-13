/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: steh <steh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/11 18:28:31 by steh              #+#    #+#             */
/*   Updated: 2023/03/13 20:20:41 by steh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

config::config() : _config_file(nullptr)
{
    return ;
}

config::config(ifstream& config_file) : _config_file(&config_file)
{
    is_valid();
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

bool config::is_valid()
{
    string                  line;
    size_t                  pos;
    string                  directive;
    set<string>             present_directives;
    set<string>::iterator   it;
    bool                    inside_context;

    inside_context = false;
    while (getline(*_config_file, line))
    {
        if (line.empty() || line[0] == '#')
            continue ;
         if (line[0] == '{') {
            inside_context = true;
            continue;
        }
        if (line[0] == '}') {
            inside_context = false;
            continue;
        }
        pos = line.find(' ');
        if (pos == string::npos)
        {
            cerr << "Error: invalid configuration file" << endl;
            return (false);
        }
        if (!inside_context) {
            pos = line.find(' ');
            if (pos == string::npos)
            {
                cerr << "Error: invalid configuration file" << endl;
                return (false);
            }
            directive = line.substr(0, pos);
            if (!_req_directives.contains(directive))
            {
                cerr << "Error: invalid directive: " << directive << endl;
                return (false);
            }
            present_directives.insert(directive);
        }
    }
    for (it = _req_directives.directives.begin(); it != _req_directives.directives.end(); ++it)
    {
        if (present_directives.find(*it) == present_directives.end())
        {
            cerr << "Error: missing required directive: " << *it << endl;
            return (false);
        }
    }
    return (true);
}
