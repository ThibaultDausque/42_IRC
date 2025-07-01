/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpipi <tpipi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 09:57:22 by tpipi             #+#    #+#             */
/*   Updated: 2025/07/01 13:26:36 by tpipi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "Channel.hpp"
#include "NumericReply.hpp"

static void	deleteEmptyChannel(std::map<std::string, Channel> &channels)
{
	std::vector<std::string>	channelToRemoveList;

	for (std::map<std::string, Channel>::iterator chanIt = channels.begin(); chanIt != channels.end(); ++chanIt) {
		if (chanIt->second.getChannelSize() == 0)
			channelToRemoveList.push_back(chanIt->first);
	}

	for (size_t i = 0; i < channelToRemoveList.size(); i++) {
		channels.erase(channelToRemoveList[i]);
	}
}

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
			if (params[3].size() > 0 && params[3][0] == ':') {
				for (size_t i = 3; i < params.size(); i++) {
					comment.append(params[i]);
					if (i + 1 == params.size())
						break ;
					comment.append(" ");
				}
				comment.append("\r\n");
			}
			else
				comment.append(":"+params[3]+"\r\n");
		}
		else
			comment = BAN_REASON;
		
		for (std::vector<std::string>::iterator chanIt = channelParam.begin(); chanIt != channelParam.end(); ++chanIt) {
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
				for (std::vector<std::string>::iterator clientIt = clientParam.begin(); clientIt != clientParam.end(); ++clientIt) {
					clientNick = *clientIt;
					errUserNotInChannel = ERR_USERNOTINCHANNEL(originNick, clientNick, chanName);
					
					if (!(*chan).isUserConnected(*clientIt))
						std::cout << errUserNotInChannel << std::endl;
						//send(origin.getSocket(), errUserNotInChannel.c_str(), errUserNotInChannel.size(), 0);
					else
						(*chan).kickUser(clientNick, origin.getFullName(), comment);
				}
			}
		}
		deleteEmptyChannel(channels);
	}
	return (0);
}