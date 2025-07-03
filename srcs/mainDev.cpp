/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mainDev.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpipi <tpipi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 16:23:42 by tpipi             #+#    #+#             */
/*   Updated: 2025/07/04 00:52:08 by tpipi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <unistd.h>
#include <limits.h>

#include "Command.hpp"
#include "User.hpp"
#include "Channel.hpp"

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
		// CREATION DE MES DEUX MAP POUR TOUS LES CHANNELS ET USERS PRESENT DANS LE SERVEUR
		User user(1);
		User zaloufi("Zaloufi", "ADOMINUSREX", CLIENT_HOSTNAME, "realname", 0);
		std::map<std::string, Channel>	channels;
		std::vector<User*> users;
		users.push_back(&user);
		users.push_back(&zaloufi);

		executeJoin(zaloufi, channels, "JOIN #test");
		Channel *chan = getChannelPtr(channels, "#test");
		chan->changeMode("i");

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
			else if (isCmdValid(cmd) && cmd == "KICK")
				executeKick(user, channels, line);
			else if (isCmdValid(cmd) && cmd == "PRIVMSG")
				executePrivmsg(user, channels, line, users);
			else if (isCmdValid(cmd) && cmd == "INVITE")
				executeInvite(user, channels, line, users);
			else if (isCmdValid(cmd) && cmd == "PART")
				executePart(user, channels, line);
			else if (isCmdValid(cmd) && cmd == "TOPIC")
				executeTopic(user, channels, line);
			else if (cmd == "TRYINVITE")
				executeInvite(zaloufi, channels, "INVITE Zetune #test", users);
			else if (cmd == "MODETOPIC")
				chan->changeMode("t");
			else
				std::cout << ERR_UNKNOWNCOMMAND(user.getNickname(), cmd) << std::endl;
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	return 0;
}
