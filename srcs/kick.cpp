/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpipi <tpipi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 09:57:22 by tpipi             #+#    #+#             */
/*   Updated: 2025/07/02 20:16:29 by tpipi            ###   ########.fr       */
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
	std::string					comment;
	std::string					originNick = origin.getNickname();
	std::string					errMsg = ERR_NEEDMOREPARAM(originNick, "KICK");
	Channel						*chan;

	if (params.size() < 3)
		std::cout << errMsg << std::endl;
		//send(origin.getSocket(), errMsg.c_str(), errMsg.size(), 0);
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
			chan = getChannelPtr(channels, *chanIt);
			
			if (chan == NULL) {
				errMsg = ERR_NOSUCHCHANNEL(originNick, *chanIt);
				std::cout << errMsg << std::endl;
				//send(origin.getSocket(), errMsg.c_str(), errMsg.size(), 0);
			}
			else if (!(*chan).isUserConnected(originNick)) {
				errMsg = ERR_NOTONCHANNEL(originNick, *chanIt);
				std::cout << errMsg << std::endl;
				//send(origin.getSocket(), errMsg.c_str(), errMsg.size(), 0);
			}
			else if (!(*chan).isUserOperator(originNick)) {
				errMsg = ERR_CHANOPRIVSNEEDED(originNick, *chanIt);
				std::cout << errMsg << std::endl;
				//send(origin.getSocket(), errMsg.c_str(), errMsg.size(), 0);
			}
			else
			{
				for (std::vector<std::string>::iterator clientIt = clientParam.begin(); clientIt != clientParam.end(); ++clientIt) {
					errMsg = ERR_USERNOTINCHANNEL(originNick, *clientIt, *chanIt);
					
					if (!(*chan).isUserConnected(*clientIt))
						std::cout << errMsg << std::endl;
						//send(origin.getSocket(), errMsg.c_str(), errMsg.size(), 0);
					else
						(*chan).kickUser(*clientIt, origin.getFullName(), comment);
				}
			}
		}
		deleteEmptyChannel(channels);
	}
	return (0);
}
