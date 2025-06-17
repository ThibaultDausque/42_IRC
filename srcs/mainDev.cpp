/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mainDev.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpipi <tpipi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 16:23:42 by tpipi             #+#    #+#             */
/*   Updated: 2025/06/17 14:47:11 by tpipi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Command.hpp"
#include "Client.hpp"
#include "Channel.hpp"

#include <unistd.h>
#include <limits.h>

int	main(int ac, char **av)
{
	(void)av;
	std::string line;

	if (ac != 3)
	{
		std::cout << "* two arguments needed *" << std::endl;
		return 0;
	}
	std::getline(std::cin, line);

	std::string cmd = line.substr(0, line.find(" "));
	// separer les \n en plusieurs lignes (vector<std::string> ???)
	// transformer les whitespaces en espace normal (verifie si c'est Hexchat ou pas)
	// chercher si les messages de base doivent etre parse d'une certaine maniere
	if (isCmdValid(cmd))
		std::cout << ">> La commande " << cmd << " est bien valide !" << std::endl;
	else
		std::cout << ">> La commande " << cmd << " n'est pas valide !" << std::endl;

	try
	{
		char hostname[HOST_NAME_MAX];
		gethostname(hostname, HOST_NAME_MAX);
		Client client(av[1], av[2], hostname, "realname", 1);
		std::cout << client << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	return 0;
}
