/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: steh <steh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/11 18:27:16 by steh              #+#    #+#             */
/*   Updated: 2023/03/14 12:14:43 by steh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

#include "webserv.hpp"

class config
{
    public:
        config();
        config(ifstream& config_file);
        config(const config& other);
        ~config();
        config& operator=(const config& other);

    private:
        ifstream*           _config_file;
};

#endif
