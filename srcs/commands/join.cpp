/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpipi <tpipi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 16:26:24 by tpipi             #+#    #+#             */
/*   Updated: 2025/07/14 18:47:54 by tpipi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"
#include "Channel.hpp"
#include "numeric_reply.hpp"
#include "command.hpp"

#include <sstream>

static void	removeUserFromEveryChannel(std::map<std::string, Channel> &channels, User &origin)
{
	std::string					originNickname = origin.getNickname();
	std::string					partMsg;

	for (std::map<std::string, Channel>::iterator chanIt = channels.begin(); chanIt != channels.end(); ++chanIt) {
		partMsg = ":"+origin.getFullName()+" PART "+chanIt->second.getName()+"\r\n";

		if (chanIt->second.isUserConnected(originNickname)) {
			chanIt->second.sendToEveryone(partMsg);
			chanIt->second.removeUser(originNickname);
		}
	}
	deleteEmptyChannel(channels);
}

int executeJoin(User &origin, std::map<std::string, Channel> &channels, std::string cmdline)
{
	Channel 	*chan;
	std::string	chanName;
	std::string	originNick = origin.getNickname();
	std::string errMsg = ERR_NEEDMOREPARAM(originNick, "JOIN");

	std::vector<std::string> keysParam;
	std::vector<std::string> channelsParam;
	std::vector<std::string> params = getVector(cmdline, ' ');

	if (params.size() == 1)
		send(origin.getSocket(), errMsg.c_str(), errMsg.size(), 0);
	else if (params.size() > 1)
	{
		channelsParam = getVector(params[1], ',');
		if (params.size() > 2)
			keysParam = getVector(params[2], ',');
		
		if (channelsParam[0] == "0" && channelsParam.size() == 1)
			removeUserFromEveryChannel(channels, origin);
		else
		{
			for (size_t i = 0; i < channelsParam.size(); i++) {
				chanName = channelsParam[i];
				chan = getChannelPtr(channels, chanName);

				if (chan != NULL)
				{
					if (!chan->isUserConnected(originNick)) {
						if (chan->isChannelProtected() && (params.size() <= 2 || (keysParam.size() <= i || chan->getKey() != keysParam[i]))) {
							errMsg = ERR_BADCHANNELKEY(originNick, chanName);
							send(origin.getSocket(), errMsg.c_str(), errMsg.size(), 0);
						}
						else if (chan->onLimiteMode() && chan->getChannelSize() >= chan->getUserLimit()) {
							errMsg = ERR_CHANNELISFULL(originNick, chanName);
							send(origin.getSocket(), errMsg.c_str(), errMsg.size(), 0);
						}
						else if (chan->onInviteMode() && !origin.isInvitedTo(chanName)) {
							errMsg = ERR_INVITEONLYCHAN(originNick, chanName);
							send(origin.getSocket(), errMsg.c_str(), errMsg.size(), 0);
						}
						else {
							chan->addUser(origin, false);
							executeNames(origin, channels, "NAMES "+chanName, NULL);
						}
					}
				}
				else
				{
					try
					{
						Channel channel(chanName);
						channel.addUser(origin, true);
						channels.insert(std::pair<std::string, Channel>(channel.getName(), channel));
						executeNames(origin, channels, "NAMES "+chanName, NULL);
					}
					catch(const std::exception& e)
					{
						errMsg = ERR_NOSUCHCHANNEL(originNick, chanName);
						send(origin.getSocket(), errMsg.c_str(), errMsg.size(), 0);
					}
				}
			}
		}
	}
	return 0; 
}