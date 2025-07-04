/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpipi <tpipi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 17:45:04 by tpipi             #+#    #+#             */
/*   Updated: 2025/07/05 00:24:06 by tpipi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "NumericReply.hpp"
#include "Command.hpp"

int executeInvite(User &origin, std::map<std::string, Channel> &channels, std::string cmdline, std::vector<User> users)
{
	std::vector<std::string>	params = getVector(cmdline, ' ');
	std::string					errMsg;
	std::string					rplInviting;
	std::string					inviteMsg = origin.getFullName()+" INVITE ";
	User						*user;
	Channel						*chan;

	if (params.size() < 3) {
		errMsg = ERR_NEEDMOREPARAM(origin.getNickname(), "INVITE");
		send(origin.getSocket(), errMsg.c_str(), errMsg.size(), 0);
	}
	else
	{
		user = getUserPtr(users, params[1]);
		chan = getChannelPtr(channels, params[2]);

		if (user == NULL) {
			errMsg = ERR_NOSUCHNICK(origin.getNickname(), params[1]);
			send(origin.getSocket(), errMsg.c_str(), errMsg.size(), 0);
		}
		else if (chan == NULL) {
			errMsg = ERR_NOSUCHCHANNEL(origin.getNickname(), params[2]);
			send(origin.getSocket(), errMsg.c_str(), errMsg.size(), 0);
		}
		else if (!chan->isUserConnected(origin.getNickname())) {
			errMsg = ERR_NOTONCHANNEL(origin.getNickname(), params[2]);
			send(origin.getSocket(), errMsg.c_str(), errMsg.size(), 0);
		}
		else if (chan->isUserConnected(params[1])) {
			errMsg = ERR_USERONCHANNEL(origin.getNickname(), params[1], params[2]);
			send(origin.getSocket(), errMsg.c_str(), errMsg.size(), 0);
		}
		else if (!chan->isUserOperator(origin.getNickname())) {
			errMsg = ERR_CHANOPRIVSNEEDED(origin.getNickname(), params[2]);
			send(origin.getSocket(), errMsg.c_str(), errMsg.size(), 0);
		}
		else {
			rplInviting = RPL_INVITING(origin.getNickname(), params[1], params[2]);
			inviteMsg.append(params[1]+" :"+params[2]+"\r\n");
			send(origin.getSocket(), rplInviting.c_str(), rplInviting.size(), 0);
			send(user->getSocket(), inviteMsg.c_str(), inviteMsg.size(), 0);
			user->addAnInvitation(chan->getName());
		}
	}
	return (0);
}
