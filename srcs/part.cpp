/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpipi <tpipi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 19:43:16 by tpipi             #+#    #+#             */
/*   Updated: 2025/07/02 19:59:33 by tpipi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "NumericReply.hpp"
#include "Command.hpp"

int executePart(User &user, std::map<std::string, Channel> &channels, std::string cmdline)
{
	std::vector<std::string>	params = getVector(cmdline, ' ');
	std::string					errMsg = ERR_NEEDMOREPARAM(user.getNickname(), "PART");
	std::string					partMsg = user.getFullName()+" PART ";
	Channel						*chan;

	if (params.size() < 2)
		std::cout << errMsg << std::endl;
		//send(user.getSocket(), errMsg.c_str(), errMsg.size(), 0);
	else {
		std::vector<std::string>	channelParam = getVector(params[1], ',');
		
		for (std::vector<std::string>::iterator it = channelParam.begin(); it != channelParam.end(); ++it) {
			chan = getChannelPtr(channels, *it);
			
			if (chan == NULL) {
				errMsg = ERR_NOSUCHCHANNEL(user.getNickname(), *it);
				std::cout << errMsg << std::endl;
				//send(user.getSocket(), errMsg.c_str(), errMsg.size(), 0);
			}
			else if (!chan->isUserConnected(user.getNickname())) {
				errMsg = ERR_NOTONCHANNEL(user.getNickname(), *it);
				std::cout << errMsg << std::endl;
				//send(user.getSocket(), errMsg.c_str(), errMsg.size(), 0);
			}
			else {
				chan->sendToEveryone(partMsg+chan->getName()+"\r\n");
				chan->removeUser(user.getNickname());
			}
		}
		deleteEmptyChannel(channels);
	}
	return (0);
}
