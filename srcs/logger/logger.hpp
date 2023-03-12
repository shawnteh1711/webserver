/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: steh <steh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/12 09:54:54 by steh              #+#    #+#             */
/*   Updated: 2023/03/12 10:27:27 by steh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef logger_HPP
# define logger_HPP
# include "webserv.hpp"

class logger
{
    public:
        logger(void);
        logger(const logger& other);
        logger&	operator=(const logger& other);
        ~logger(void);

    private:
        
};

#endif
