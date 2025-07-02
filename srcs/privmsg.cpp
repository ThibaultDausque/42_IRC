/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpipi <tpipi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 13:53:41 by tpipi             #+#    #+#             */
/*   Updated: 2025/07/02 16:52:43 by tpipi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "Channel.hpp"
#include "NumericReply.hpp"

static bool	isReceiverAChannel(std::string receiver)
{
	if (receiver.size() > 0 && (receiver[0] == '&' || receiver[0] == '#'))
		return (true);
	return (false);
}

int executePrivmsg(User &origin, std::map<std::string, Channel> &channels, std::string cmdline, std::vector<User*> users)
{
	std::map<User*, bool>		userList;
	std::vector<std::string>	params = getVector(cmdline, ' ');
	std::vector<std::string>	receiverParam;
	std::string					text;
	std::string					privmsgMsg = ":"+origin.getFullName()+" PRIVMSG ";
	std::string					sentMsg;
	std::string					errNoRecipient = ERR_NORECIPIENT(origin.getNickname(), "PRIVMSG");
	std::string					errNoTextToSend = ERR_NOTEXTTOSEND(origin.getNickname());
	std::string					errNoSuchNick;
	Channel						*chan;
	User						*user;
	
	if (params.size() == 1)
		std::cout << errNoRecipient << std::endl;
		//send(origin.getSocket(), errNoRecipient.c_str(), errNoRecipient.size(), 0);
	else if (params.size() == 2)
		std::cout << errNoTextToSend << std::endl;
		//send(origin.getSocket(), errNoTextToSend.c_str(), errNoTextToSend.size(), 0);
	else
	{
		receiverParam = getVector(params[1], ',');

		if (params[2].size() > 0 && params[2][0] != ':')
			text.append(":"+params[2]+"\r\n");
		else
		{
			for (size_t i = 2; i < params.size(); i++) {
				text.append(params[i]);
				if (i + 1 == params.size())
					break ;
				text.append(" ");
			}
			text.append("\r\n");
		}
		
		for (std::vector<std::string>::iterator receiverIt = receiverParam.begin(); receiverIt != receiverParam.end(); ++receiverIt) {
			errNoSuchNick = ERR_NOSUCHNICK(origin.getNickname(), *receiverIt);
			sentMsg = privmsgMsg+(*receiverIt)+" "+text;
			
			if (!doesChannelExist(channels, *receiverIt) && !doesClientExist(users, *receiverIt))
				std::cout << errNoSuchNick << std::endl;
				//send(origin.getSocket(), errNoSuchNick.c_str(), errNoSuchNick.size(), 0);
			else if (isReceiverAChannel(*receiverIt)) {
				chan = getChannelPtr(channels, *receiverIt);
				userList = chan->getUsers();
				
				for (std::map<User*, bool>::iterator userIt = userList.begin(); userIt != userList.end(); ++userIt) {
					user = userIt->first;
					if (user->getNickname() != origin.getNickname())
						std::cout << sentMsg << std::endl;
						//send(user->getSocket(), sentMsg.c_str(), sentMsg.size(), 0);
				}
			}
			else if (!isReceiverAChannel(*receiverIt)) {
				user = getUserPtr(users, *receiverIt);
				std::cout << sentMsg << std::endl;
				//send(user->getSocket(), sentMsg.c_str(), sentMsg.size(), 0);
			}
		}
	}
	return (0);
}
