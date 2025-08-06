/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   names.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpipi <tpipi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 17:28:58 by tpipi             #+#    #+#             */
/*   Updated: 2025/08/06 04:42:55 by tpipi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "numeric_reply.hpp"
#include "User.hpp"
#include "Channel.hpp"
#include "command.hpp"

void	printUsersInChannel(User &origin, std::map<std::string, Channel> &channels, std::string chanName)
{
	std::string				rplEndOfNames;
	std::string				rplNamReply;
	std::string				originNick = origin.getNickname();
	std::map<User*, bool>	userList;

	rplEndOfNames =  RPL_ENDOFNAMES(originNick, chanName);
	if (doesChannelExist(channels, chanName)) {
		rplNamReply = RPL_NAMREPLY(originNick, chanName);
		userList = channels.at(chanName).getUsers();

		for (std::map<User*, bool>::iterator it = userList.begin(); it != userList.end(); ++it) {
			User tmp = (*it->first);
			if (it->second)
				rplNamReply.push_back('@');
			rplNamReply.append(tmp.getNickname()+" ");
		}
		rplNamReply.append("\r\n");
		send(origin.getSocket(), rplNamReply.c_str(), rplNamReply.size(), 0);
	}
	send(origin.getSocket(), rplEndOfNames.c_str(), rplEndOfNames.size(), 0);
}

int executeNames(User &origin, std::map<std::string, Channel> &channels, std::string cmdline, std::list<User> *users)
{
	std::string					chanName;
	std::string					rplEndOfNames;
	std::string					rplNamReply;
	std::string					originNick = origin.getNickname();
	std::vector<std::string>    params = getVector(cmdline, ' ');
	bool						isAUserAlone = false;

	if (params.size() > 1) {
		std::vector<std::string> channelsParam = getVector(params[1], ',');

		for (size_t i = 0; i < channelsParam.size(); i++)
			printUsersInChannel(origin, channels, channelsParam[i]);
	}
	else
	{
		for (std::map<std::string, Channel>::iterator it = channels.begin(); it != channels.end(); it++)
			printUsersInChannel(origin, channels, it->first);
		
		
		rplEndOfNames =  RPL_ENDOFNAMES(originNick, "*");
		rplNamReply = RPL_NAMREPLY(originNick, "*");
		for (std::list<User>::iterator userIt = users->begin(); userIt != users->end(); userIt++) {
			if (!userConnectedOnAnyChannel(channels, *userIt)) {
				isAUserAlone = true;
				rplNamReply.append(userIt->getNickname());
				rplNamReply.append(" ");
			}
		}
		if (isAUserAlone) {
			rplNamReply.erase(rplNamReply.end() - 1);
			rplNamReply.append("\r\n");
			send(origin.getSocket(), rplNamReply.c_str(), rplNamReply.size(), 0);
			send(origin.getSocket(), rplEndOfNames.c_str(), rplEndOfNames.size(), 0);
		}
	}
	return 0;
}
