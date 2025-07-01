/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   names.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpipi <tpipi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 17:28:58 by tpipi             #+#    #+#             */
/*   Updated: 2025/07/01 13:31:34 by tpipi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "NumericReply.hpp"
#include "User.hpp"
#include "Channel.hpp"
#include "Command.hpp"

void	printUsersInChannel(User &origin, std::map<std::string, Channel> &channels, std::string chanName)
{
	std::string				originNick = origin.getNickname();
	std::string				rplEndOfNames;
	std::string				rplNamReply;
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
		std::cout << rplNamReply << std::endl;
		//send(origin.getSocket(), rplNamReply.c_str(), rplNamReply.size(), 0);
	}
	std::cout << rplEndOfNames << std::endl;
	//send(origin.getSocket(), rplEndOfNames.c_str(), rplEndOfNames.size(), 0);
}

int executeNames(User &origin, std::map<std::string, Channel> &channels, std::string cmdline, std::vector<User*> *users)
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
		for (std::vector<User*>::iterator userIt = (*users).begin(); userIt != (*users).end(); userIt++) {
			if (!userConnectedOnAnyChannel(channels, *(*userIt))) {
				rplNamReply.append((*(*userIt)).getNickname());
				if (userIt + 1 == (*users).end())
					break ;
				rplNamReply.append(" ");
				isAUserAlone = true;
			}
		}
		rplNamReply.append("\r\n");
		if (isAUserAlone) {
			std::cout << rplNamReply << std::endl;
			//send(origin.getSocket(), rplNamReply.c_str(), rplNamReply.size(), 0);
			std::cout << rplEndOfNames << std::endl;
			//send(origin.getSocket(), rplEndOfNames.c_str(), rplEndOfNames.size(), 0);
		}
	}
	return 0;
}
