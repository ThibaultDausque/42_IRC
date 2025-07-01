/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpipi <tpipi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 13:58:44 by tpipi             #+#    #+#             */
/*   Updated: 2025/07/01 12:24:58 by tpipi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "NumericReply.hpp"
#include "Command.hpp"
#include "User.hpp"
#include <set>

int executeNick(User &user, std::map<std::string, Channel> &channels, std::string cmdline, std::vector<User*> &users)
{
	std::string 				oldNick = user.getNickname();
	std::string 				errNoNicknameGiven = ERR_NONICKNAMEGIVEN(oldNick);
	std::vector<std::string>	params = getVector(cmdline, ' ');
	std::map<User*, bool> 		userList;
	std::set<int>				usersSharingChannelFD;

	if (oldNick == "")
		oldNick = "*";
	if (params.size() == 1) {
		std::cout << errNoNicknameGiven << std::endl;
		//send(user.getSocket(), errNoNicknameGiven.c_str(), errNoNicknameGiven.size(), 0);
		return (1);
	}
	else
	{
		std::string newNick = params[1];
		std::string errErroneusNickname = ERR_ERRONEUSNICKNAME(oldNick, newNick);
		std::string errNicknameInUse = ERR_NICKNAMEINUSE(oldNick, newNick);
		std::string	nickMsg = ":"+user.getFullName()+" NICK :"+newNick+"\r\n";

		if (hasInvalidChar(newNick)) {
			std::cout << errErroneusNickname << std::endl;
			//send(user.getSocket(), errErroneusNickname.c_str(), errErroneusNickname.size(), 0);
			return (1);
		}

		for (std::vector<User*>::iterator it = users.begin(); it != users.end(); ++it) {
			if ((*it)->getNickname() == newNick) {
				std::cout << errNicknameInUse << std::endl;
				//send(user.getSocket(), errNicknameInUse.c_str(), errNicknameInUse.size(), 0);
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
				std::cout << nickMsg << std::endl;
				//send(*fdIt, nickMsg.c_str(), nickMsg.size(), 0);
			std::cout << nickMsg << std::endl;
			//send(user.getSocket(), nickMsg.c_str(), nickMsg.size(), 0);
		}
		else
			user.isUserRegistered();
	}
	return (0);
}