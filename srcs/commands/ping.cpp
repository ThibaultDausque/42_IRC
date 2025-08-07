/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpipi <tpipi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 03:46:33 by tpipi             #+#    #+#             */
/*   Updated: 2025/08/07 18:23:45 by tpipi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.hpp"
#include "Channel.hpp"
#include "numeric_reply.hpp"

int executePing(User &user, std::string cmdline)
{
	std::vector<std::string>    params = getVector(cmdline, ' ');
	std::string					errMsg = ERR_NEEDMOREPARAM(user.getNickname(), "PING");

	if (params.size() < 2)
		send(user.getSocket(), errMsg.c_str(), errMsg.size(), 0);
	else {
		std::string	token = params[1];
		std::string	pingMsg = ":"SERVER_HOSTNAME" PONG "SERVER_HOSTNAME" :"+token+"\r\n";
		send(user.getSocket(), pingMsg.c_str(), pingMsg.size(), 0);
	}
	return (0);
}
