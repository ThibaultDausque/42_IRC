/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpipi <tpipi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 13:53:41 by tpipi             #+#    #+#             */
/*   Updated: 2025/07/11 15:35:39 by tpipi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "Channel.hpp"
#include "NumericReply.hpp"

int executePrivmsg(User &origin, std::map<std::string, Channel> &channels, std::string cmdline, std::vector<User> users)
{
	std::map<User*, bool>		userList;
	std::vector<std::string>	params = getVector(cmdline, ' ');
	std::vector<std::string>	receiverParam;
	std::string					text;
	std::string					sentMsg;
	std::string					errMsg;
	Channel						*chan;
	User						*user;
	
	if (params.size() == 1) {
		errMsg = ERR_NORECIPIENT(origin.getNickname(), "PRIVMSG");
		send(origin.getSocket(), errMsg.c_str(), errMsg.size(), 0);
	}
	else if (params.size() == 2) {
		errMsg = ERR_NOTEXTTOSEND(origin.getNickname());
		send(origin.getSocket(), errMsg.c_str(), errMsg.size(), 0);
	}
	else
	{
		receiverParam = getVector(params[1], ',');
		createReason(params, &text, 2);
		
		for (std::vector<std::string>::iterator receiverIt = receiverParam.begin(); receiverIt != receiverParam.end(); ++receiverIt) {
			errMsg = ERR_NOSUCHNICK(origin.getNickname(), *receiverIt);
			sentMsg = ":"+origin.getFullName()+" PRIVMSG "+(*receiverIt)+" "+text;
			
			if (!doesChannelExist(channels, *receiverIt) && !doesClientExist(users, *receiverIt))
				send(origin.getSocket(), errMsg.c_str(), errMsg.size(), 0);
			else if (isReceiverAChannel(*receiverIt)) {
				chan = getChannelPtr(channels, *receiverIt);

				if (!chan->isUserConnected(user->getNickname())) {
					errMsg = ERR_CANNOTSENDTOCHAN(origin.getNickname(), *receiverIt);
					send(origin.getSocket(), errMsg.c_str(), errMsg.size(), 0);
				}
				else {
					userList = chan->getUsers();
					
					for (std::map<User*, bool>::iterator userIt = userList.begin(); userIt != userList.end(); ++userIt) {
						user = userIt->first;
						if (user->getNickname() != origin.getNickname())
							send(user->getSocket(), sentMsg.c_str(), sentMsg.size(), 0);
					}
				}
			}
			else if (!isReceiverAChannel(*receiverIt)) {
				user = getUserPtr(users, *receiverIt);
				send(user->getSocket(), sentMsg.c_str(), sentMsg.size(), 0);
			}
		}
	}
	return (0);
}
