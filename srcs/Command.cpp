/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpipi <tpipi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 16:27:00 by tpipi             #+#    #+#             */
/*   Updated: 2025/06/14 16:26:35 by tpipi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

/*-------------------------
CONSTRUCTORS AND DESTRUCTOR
---------------------------*/

Command::Command(std::string cmd, std::string pre, std::string line) : c_cmd(cmd), c_prefix(pre)
{
	args.push_back(line);
}

Command::~Command() {}

/*-----
GETTERS
-------*/

std::string	Command::getCmd(void) const
{
	return (this->c_cmd);
}

std::string	Command::getPrefix(void) const
{
	return (this->c_prefix);
}

/*--------------
FUNCTIONS MEMBER
----------------*/

/*-------
FUNCTIONS
---------*/

bool	isCmdValid(std::string cmd)
{
	std::string cmds[15] = {
		"USER",
		"NICK",
		"PASS",
		"QUIT",
		"PRIVMSG",
		"PING",
		"PONG",
		"JOIN",
		"PART",
		"MODE",
		"TOPIC",
		"INVITE",
		"KICK",
		"NAMES",
		"LIST"
	};

	for (int i = 0; i < 15; i++) {
		if (cmds[i] == cmd)
			return true;
	}
	return false;
}