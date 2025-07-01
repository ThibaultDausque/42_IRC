/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpipi <tpipi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 09:57:22 by tpipi             #+#    #+#             */
/*   Updated: 2025/07/01 12:24:18 by tpipi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "Channel.hpp"
#include "NumericReply.hpp"

int executeKick(User &origin, std::map<std::string, Channel> &channels, std::string cmdline)
{
	std::vector<std::string>	channelParam;
	std::vector<std::string>	clientParam;
	std::vector<std::string>	params = getVector(cmdline, ' ');
	std::string					originNick = origin.getNickname();
	std::string					errNeedMoreParam = ERR_NEEDMOREPARAM(originNick, "KICK");
	std::string					errNoSuchChannel;
	std::string 				errNotOnchannel;
	std::string					errChanOPrivsNeeded;
	std::string					errUserNotInChannel;
	std::string					chanName;
	std::string					clientNick;
	std::string					comment;

	if (params.size() < 3)
		std::cout << errNeedMoreParam << std::endl;
		//send(origin.getSocket(), errNeedMoreParam.c_str(), errNeedMoreParam.size(), 0);
	else
	{
		channelParam = getVector(params[1], ',');
		clientParam = getVector(params[2], ',');
		if (params.size() >= 4) {
			for (int i = 3; i < params.size(); i++)
				comment.append(params[i]);
			comment.append("\r\n");
		}
		else
			comment = BAN_REASON;
		
		for (std::vector<std::string>::iterator chanIt = channelParam.end(); chanIt != channelParam.end(); ++chanIt) {
			chanName = *chanIt;
			errNoSuchChannel = ERR_NOSUCHCHANNEL(originNick, chanName);
			errNotOnchannel = ERR_NOTONCHANNEL(originNick, chanName);
			errChanOPrivsNeeded = ERR_CHANOPRIVSNEEDED(originNick, chanName);

			if (!doesChannelExist(channels, chanName)) {
				std::cout << errNoSuchChannel << std::endl;
				//send(origin.getSocket(), errNoSuchChannel.c_str(), errNoSuchChannel.size(), 0);
				continue ;
			}

			Channel *chan = &channels.at(chanName);
			if (!(*chan).isUserConnected(originNick))
				std::cout << errNotOnchannel << std::endl;
				//send(origin.getSocket(), errNotOnchannel.c_str(), errNotOnchannel.size(), 0);
			else if (!(*chan).isUserOperator(originNick))
				std::cout << errChanOPrivsNeeded << std::endl;
				//send(origin.getSocket(), errChanOPrivsNeeded.c_str(), errChanOPrivsNeeded.size(), 0);
			else
			{
				for (std::vector<std::string>::iterator clientIt = clientParam.end(); clientIt != clientParam.end(); ++clientIt) {
					clientNick = *clientIt;
					errUserNotInChannel = ERR_USERNOTINCHANNEL(originNick, clientNick, chanName);
					
					if (!(*chan).isUserConnected(*clientIt))
						std::cout << errNotOnchannel << std::endl;
						//send(origin.getSocket(), errNotOnchannel.c_str(), errNotOnchannel.size(), 0);
					else
						(*chan).removeUser(clientNick, origin.getFullName(), comment);
				}
			}
		}
	}
	return (0);
}