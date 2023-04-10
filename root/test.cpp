/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leng-chu <-chu@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 14:49:00 by leng-chu          #+#    #+#             */
/*   Updated: 2023/04/06 15:00:14 by leng-chu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>

int	main(int i, char **argv)
{
	std::string filename = argv[1];
	std::ifstream file(filename.c_str());
	if (file.good())
		std::cout << "File " << filename << " exists! " << std::endl;
	else
		std::cout << "File " << filename << " does not exist." << std::endl;
	return (0);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leng-chu <-chu@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 14:49:00 by leng-chu          #+#    #+#             */
/*   Updated: 2023/04/06 15:00:14 by leng-chu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>

int	main(int i, char **argv)
{
	std::string filename = argv[1];
	std::ifstream file(filename.c_str());
	if (file.good())
		std::cout << "File " << filename << " exists! " << std::endl;
	else
		std::cout << "File " << filename << " does not exist." << std::endl;
	return (0);
}
