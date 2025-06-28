/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mainDev.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpipi <tpipi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 16:23:42 by tpipi             #+#    #+#             */
/*   Updated: 2025/06/28 12:27:29 by tpipi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <unistd.h>
#include <limits.h>

#include "Command.hpp"
#include "User.hpp"
#include "Channel.hpp"

int executeJoin(User &origin, std::map<std::string, Channel> &channels, std::string cmdline);
int executeNames(User &origin, std::map<std::string, Channel> &channels, std::string cmdline, std::vector<User> *users);

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
		User user(av[1], "AdominusRexL9", hostname, "realname", 1);
		user.addAnInvitation(av[2]);

		// CREATION DE DEUX USERS ADDITIONNELS 
		User tpipi("tpipi", "tpipi", hostname, "realname", 1);
		User tdausque("tdausque", "tdausque", hostname, "realname", 1);

		// CREATION D'UN CHANNEL NOMMÉ GRACE AU PROMPT ET AJOUT DES DEUX USERS ET D'UN TOPIC
		Channel	channel(av[2], "", "i", "", -1);
		channel.addUser(tpipi, true);
		channel.addUser(tdausque, false);
		channel.changeTopic("Salut les ptits loups c'est Calistou", tpipi);

		// CREATION DE MES DEUX MAP POUR TOUS LES CHANNELS ET USERS PRESENT DANS LE SERVEUR
		std::map<std::string, Channel>	channels;
		std::vector<User> users;

		channels.insert(std::pair<std::string, Channel>(channel.getName(), channel));
		users.push_back(tpipi);
		users.push_back(tdausque);
		users.push_back(user);

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
			else if (isCmdValid(cmd) && cmd == "JOIN")
				executeJoin(user, channels, line);
			else if (isCmdValid(cmd) && cmd == "NAMES")
				executeNames(user, channels, line, &users);
			else
				std::cout << ">> La commande " << cmd << " n'est pas valide !" << std::endl;

			for (std::map<std::string, Channel>::iterator it = channels.begin(); it != channels.end(); ++it) {
				std::cout << it->first << " : " << std::endl;
				std::cout << "Nombre de personne dans le channel : " << it->second.getChannelSize() << std::endl;
				userstmp = it->second.getUsers();

				for (std::map<User, bool>::iterator ite = userstmp.begin(); ite != userstmp.end(); ++ite) {
					std::cout << "- " << ite->first.getNickname() << std::endl;
				}
				std::cout << std::endl;
			}


		}
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	return 0;
}
