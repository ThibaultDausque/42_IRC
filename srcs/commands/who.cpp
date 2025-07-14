/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   who.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpipi <tpipi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 04:31:44 by tpipi             #+#    #+#             */
/*   Updated: 2025/07/14 18:47:54 by tpipi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "numeric_reply.hpp"
#include "command.hpp"

int executeWho(User &origin, std::map<std::string, Channel> &channels, std::string cmdline, std::vector<User> &users)
{
	std::string 				rplWhoReply;
	std::string 				flag = "H";
	std::string					errMsg = ERR_NEEDMOREPARAM(origin.getNickname(), "WHO");
	std::vector<std::string>	params = getVector(cmdline, ' ');
	Channel						*chan;
	User						*user;
	
	if (params.size() < 2)
		send(origin.getSocket(), errMsg.c_str(), errMsg.size(), 0);
	else {
		std::string rplEndOfWho = RPL_ENDOFWHO(origin.getNickname(), params[1]);
		chan = getChannelPtr(channels, params[1]);
		user = getUserPtr(users, params[1]);

		if (isReceiverAChannel(params[1]) && chan != NULL) {
			std::map<User*, bool> userList = chan->getUsers();
			for (std::map<User*, bool>::iterator it = userList.begin(); it != userList.end(); ++it) {
				user = it->first;
				flag = "H";
				if (chan->isUserOperator(user->getNickname()))
					flag = "H@";

				rplWhoReply = RPL_WHOREPLY(origin.getNickname(), params[1], user->getUsername(), user->getHostname(), SERVER_HOSTNAME, user->getNickname(), flag, user->getRealname());
				send(origin.getSocket(), rplWhoReply.c_str(), rplWhoReply.size(), 0);
			}
		}
		else if (!isReceiverAChannel(params[1]) && user != NULL) {
			std::string channel = "*";
			
			for (std::map<std::string, Channel>::iterator it = channels.begin(); it != channels.end(); ++it) {
				if (it->second.isUserConnected(params[1])) {
					channel = it->second.getName();
					if (it->second.isUserOperator(params[1]))
						flag = "H@";
					break ;
				}
			}
			rplWhoReply = RPL_WHOREPLY(origin.getNickname(), channel, user->getUsername(), user->getHostname(), SERVER_HOSTNAME, user->getNickname(), flag, user->getRealname());
			send(origin.getSocket(), rplWhoReply.c_str(), rplWhoReply.size(), 0);
		}
		send(origin.getSocket(), rplEndOfWho.c_str(), rplEndOfWho.size(), 0);
	}
	return (0);
}
