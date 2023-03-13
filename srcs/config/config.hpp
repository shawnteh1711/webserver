/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: steh <steh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/11 18:27:16 by steh              #+#    #+#             */
/*   Updated: 2023/03/13 20:21:02 by steh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

#include "webserv.hpp"


// 1. Define a struct or an enum to store the valid directives for the configuration file.
// 2. Read the configuration file line by line.
//  For each line, check if the line is empty or a comment (denoted by #).
//  If the line is not empty or a comment, split the line into a directive and its value.
// 3. Check if the directive is a valid directive by comparing it to the valid directives stored in the struct or enum.
//  If the directive is not valid, throw an error.
// 4. If the directive is valid, parse its value and store it in a variable.
// 5. Repeat steps 4-7 for all the valid directives in the configuration file.
// 6. Check if all the required directives are present in the configuration file.
// 7. If any of the required directives are missing, throw an error.
// 8. If all the required directives are present, the configuration file is valid.
// 9. Use the parsed values to configure the WebSocket.
struct RequiredDirectives
{
    std::set<std::string> directives;

    RequiredDirectives()
    {
        directives.insert("user");
        directives.insert("worker_processes");
        directives.insert("error_log");
        directives.insert("pid");
        directives.insert("worker_rlimit_nofile");
    }

    bool contains(const string& directive)
    {
        return (directives.count(directive) > 0);
    }
};

class config
{
    public:
        config();
        config(ifstream& config_file);
        config(const config& other);
        ~config();
        config& operator=(const config& other);
        bool is_valid();

    private:
        ifstream*           _config_file;
        RequiredDirectives  _req_directives;
};

#endif
