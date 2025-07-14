/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpipi <tpipi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 22:57:53 by tpipi             #+#    #+#             */
/*   Updated: 2025/07/14 18:47:54 by tpipi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "numeric_reply.hpp"
#include "Channel.hpp"
#include "command.hpp"

int executeTopic(User &user, std::map<std::string, Channel> &channels, std::string cmdline)
{
	std::string					topic;
	std::string					errMsg = ERR_NEEDMOREPARAM(user.getNickname(), "TOPIC");
	std::vector<std::string>	params = getVector(cmdline, ' ');
	Channel						*chan;
	
	if (params.size() < 2)
		send(user.getSocket(), errMsg.c_str(), errMsg.size(), 0);
	else {
		chan = getChannelPtr(channels, params[1]);
		if (params.size() > 2) {
			if (params[2][0] == ':') {
				for (size_t i = 2; i < params.size(); i++) {
					topic.append(params[i]);
					if (i + 1 == params.size())
						break ;
					topic.append(" ");
				}
			}
			else
				topic = params[2];
		}
		else
			topic = "";

		if (chan == NULL) {
			errMsg = ERR_NOSUCHCHANNEL(user.getNickname(), params[1]);
			send(user.getSocket(), errMsg.c_str(), errMsg.size(), 0);
		}
		else if (!chan->isUserConnected(user.getNickname())) {
			errMsg = ERR_NOTONCHANNEL(user.getNickname(), params[1]);
			send(user.getSocket(), errMsg.c_str(), errMsg.size(), 0);
		}
		else if (!topic.empty() && !chan->isUserOperator(user.getNickname()) && chan->onTopicRestrictedMode()) {
			errMsg = ERR_CHANOPRIVSNEEDED(user.getNickname(), params[1]);
			send(user.getSocket(), errMsg.c_str(), errMsg.size(), 0);
		}
		else if (topic.empty()) {
			if (chan->doesChannelHaveATopic()) {
				std::string	rplTopic = RPL_TOPIC(user.getNickname(), chan->getName(), chan->getTopic());
				std::string	rplTopicWhoTime = RPL_TOPICWHOTIME(user.getNickname(), chan->getName(), chan->getLastUserToChangeTopic(), chan->convertUNIXTimeToString(chan->getLastTimeTopicChange()));
				send(user.getSocket(), rplTopic.c_str(), rplTopic.size(), 0);
				send(user.getSocket(), rplTopicWhoTime.c_str(), rplTopicWhoTime.size(), 0);
			}
			else {
				std::string	rplNoTopic = RPL_NOTOPIC(user.getNickname(), chan->getName());
				send(user.getSocket(), rplNoTopic.c_str(), rplNoTopic.size(), 0);
			}
		}
		else {
			topic.erase(0, 1); //delete ':'
			chan->changeTopic(topic, user);
			chan->sendToEveryone(":"+user.getFullName()+" TOPIC "+chan->getName()+" :"+topic+"\r\n");
		}
	}
	return (0);
}
