/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: steh <steh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/12 10:20:49 by steh              #+#    #+#             */
/*   Updated: 2023/03/12 10:33:53 by steh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

logger::logger(void)
{

}

logger::logger(const logger& other)
{
    if (this != &other)
        ;
}

logger&	logger::operator=(const logger& other)
{
    if (this != &other)
        ;
    return (*this);
}

logger::~logger(void)
{

}
