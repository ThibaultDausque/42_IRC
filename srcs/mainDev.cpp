/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mainDev.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpipi <tpipi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 16:23:42 by tpipi             #+#    #+#             */
/*   Updated: 2025/06/20 15:47:03 by tpipi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Command.hpp"
#include "User.hpp"
#include "Channel.hpp"

#include <unistd.h>
#include <limits.h>

int executeJoin(User origin, std::map<std::string, Channel> &channels, std::string cmdline);

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
		User user(av[1], "AdominusRexL9", hostname, "realname", 1);
		std::cout << user << std::endl;

		User tpipi("tpipi", "tpipi", hostname, "realname", 1);
		User tdausque("tdausque", "tdausque", hostname, "realname", 1);

		Channel	channel(av[2], "", "", "");
		channel.addUser(tpipi, true);
		channel.addUser(tdausque, false);

		std::map<std::string, Channel>	channels;
		std::vector<User> users;

		channels.insert(std::pair<std::string, Channel>(channel.getName(), channel));
		users.push_back(tpipi);
		users.push_back(tdausque);
		executeJoin(tpipi, channels, line);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	return 0;
}
