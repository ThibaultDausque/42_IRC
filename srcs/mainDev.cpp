/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mainDev.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpipi <tpipi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 16:23:42 by tpipi             #+#    #+#             */
/*   Updated: 2025/06/30 20:43:10 by tpipi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <unistd.h>
#include <limits.h>

#include "Command.hpp"
#include "User.hpp"
#include "Channel.hpp"

int executeJoin(User &origin, std::map<std::string, Channel> &channels, std::string cmdline);
int executeNames(User &origin, std::map<std::string, Channel> &channels, std::string cmdline, std::vector<User*> *users);
int executeNick(User &user, std::map<std::string, Channel> &channels, std::string cmdline, std::vector<User*> &users);
int	executeUser(User &user, std::string cmdline);

int	main(int ac, char **av)
{
	(void)av;
	std::string line;
	std::map<User, bool> userstmp;

	if (ac != 3)
	{
		std::cout << "* two arguments needed *" << std::endl;
		return 0;
	}

	try
	{
		// CREATION DE MON USER PERSONNALISE DANS LE PROMPT
		char hostname[HOST_NAME_MAX];
		gethostname(hostname, HOST_NAME_MAX);
		User user(1);

		// CREATION DE MES DEUX MAP POUR TOUS LES CHANNELS ET USERS PRESENT DANS LE SERVEUR
		std::map<std::string, Channel>	channels;
		std::vector<User*> users;
		users.push_back(&user);

		// BOUCLE DE COMMANDES
		while (true) {
			std::cout << ">> ";
			std::getline(std::cin, line);

			std::string cmd = line.substr(0, line.find(" "));
			// separer les \n en plusieurs lignes (vector<std::string> ???)
			// transformer les whitespaces en espace normal (verifie si c'est Hexchat ou pas)
			// chercher si les messages de base doivent etre parse d'une certaine maniere
			if (cmd == "QUIT")
				break ;
			else if (isCmdValid(cmd) && cmd == "USER")
				executeUser(user, line);
			else if (isCmdValid(cmd) && cmd == "NICK")
				executeNick(user, channels, line, users);
			else if (!user.isNicknameRegistered() || !user.isUsernameRegistered()) {
				std::string tmpNick;
				if (user.getNickname() == "")
					tmpNick = "*";
				else
					tmpNick = user.getNickname();
				std::cout << ERR_NOTREGISTERED(tmpNick) << std::endl;
			}
			else if (isCmdValid(cmd) && cmd == "JOIN")
				executeJoin(user, channels, line);
			else if (isCmdValid(cmd) && cmd == "NAMES")
				executeNames(user, channels, line, &users);
			else
				std::cout << ">> La commande " << cmd << " n'est pas valide !" << std::endl;
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	return 0;
}
