/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpipi <tpipi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 13:58:44 by tpipi             #+#    #+#             */
/*   Updated: 2025/08/06 04:34:53 by tpipi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "numeric_reply.hpp"
#include "command.hpp"
#include "User.hpp"

int executeNick(User &user, std::map<std::string, Channel> &channels, std::string cmdline, std::list<User> &users)
{
	std::string 				oldNick = user.getNickname();
	std::string 				errMsg;
	std::vector<std::string>	params = getVector(cmdline, ' ');
	std::map<User*, bool> 		userList;
	std::set<int>				usersSharingChannelFD;

	if (oldNick == "")
		oldNick = "*";
	if (params.size() == 1) {
		errMsg = ERR_NONICKNAMEGIVEN(oldNick);
		send(user.getSocket(), errMsg.c_str(), errMsg.size(), 0);
		return (1);
	}
	else
	{
		std::string newNick = params[1];
		std::string	nickMsg = ":"+user.getFullName()+" NICK :"+newNick+"\r\n";

		if (hasInvalidChar(newNick)) {
			errMsg = ERR_ERRONEUSNICKNAME(oldNick, newNick);
			send(user.getSocket(), errMsg.c_str(), errMsg.size(), 0);
			return (1);
		}

		for (std::list<User>::iterator it = users.begin(); it != users.end(); ++it) {
			if (it->getNickname() == newNick) {
				errMsg = ERR_NICKNAMEINUSE(oldNick, newNick);
				send(user.getSocket(), errMsg.c_str(), errMsg.size(), 0);
				return (1);
			}
		}

		user.setNickname(newNick);
		if (oldNick != "*" && user.isUsernameRegistered()) {
			for (std::map<std::string, Channel>::iterator chanIt = channels.begin(); chanIt != channels.end(); ++chanIt) {
				if (chanIt->second.isUserConnected(newNick)) {
					userList = chanIt->second.getUsers();
					
					for (std::map<User*, bool>::iterator userIt = userList.begin(); userIt != userList.end(); ++userIt) {
						if ((*userIt->first).getNickname() != user.getNickname())
							usersSharingChannelFD.insert((*userIt->first).getSocket());
					}
				}
			}
			for (std::set<int>::iterator fdIt = usersSharingChannelFD.begin(); fdIt != usersSharingChannelFD.end(); ++fdIt)
				send(*fdIt, nickMsg.c_str(), nickMsg.size(), 0);
			send(user.getSocket(), nickMsg.c_str(), nickMsg.size(), 0);
		}
		else
			user.isUserRegistered();
	}
	return (0);
}