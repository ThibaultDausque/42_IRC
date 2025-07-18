/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpipi <tpipi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 09:57:22 by tpipi             #+#    #+#             */
/*   Updated: 2025/07/14 18:47:54 by tpipi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.hpp"
#include "Channel.hpp"
#include "numeric_reply.hpp"

int executeKick(User &origin, std::map<std::string, Channel> &channels, std::string cmdline)
{
	std::vector<std::string>	channelParam;
	std::vector<std::string>	clientParam;
	std::vector<std::string>	params = getVector(cmdline, ' ');
	std::string					reason;
	std::string					originNick = origin.getNickname();
	std::string					errMsg = ERR_NEEDMOREPARAM(originNick, "KICK");
	Channel						*chan;

	if (params.size() < 3)
		send(origin.getSocket(), errMsg.c_str(), errMsg.size(), 0);
	else
	{
		channelParam = getVector(params[1], ',');
		clientParam = getVector(params[2], ',');
		createReason(params, &reason, 3);
		
		for (std::vector<std::string>::iterator chanIt = channelParam.begin(); chanIt != channelParam.end(); ++chanIt) {
			chan = getChannelPtr(channels, *chanIt);
			
			if (chan == NULL) {
				errMsg = ERR_NOSUCHCHANNEL(originNick, *chanIt);
				send(origin.getSocket(), errMsg.c_str(), errMsg.size(), 0);
			}
			else if (!chan->isUserConnected(originNick)) {
				errMsg = ERR_NOTONCHANNEL(originNick, *chanIt);
				send(origin.getSocket(), errMsg.c_str(), errMsg.size(), 0);
			}
			else if (!chan->isUserOperator(originNick)) {
				errMsg = ERR_CHANOPRIVSNEEDED(originNick, *chanIt);
				send(origin.getSocket(), errMsg.c_str(), errMsg.size(), 0);
			}
			else
			{
				for (std::vector<std::string>::iterator clientIt = clientParam.begin(); clientIt != clientParam.end(); ++clientIt) {
					errMsg = ERR_USERNOTINCHANNEL(originNick, *clientIt, *chanIt);
					
					if (!chan->isUserConnected(*clientIt))
						send(origin.getSocket(), errMsg.c_str(), errMsg.size(), 0);
					else
						chan->kickUser(*clientIt, origin.getFullName(), reason);
				}
			}
		}
		deleteEmptyChannel(channels);
	}
	return (0);
}
