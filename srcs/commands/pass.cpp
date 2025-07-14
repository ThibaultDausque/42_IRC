/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpipi <tpipi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 22:20:54 by tpipi             #+#    #+#             */
/*   Updated: 2025/07/14 18:47:54 by tpipi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"
#include "command.hpp"
#include "numeric_reply.hpp"

int	executePass(User &user, std::string cmdline, std::string password)
{
	std::string					errMsg;
	std::string					nick;
	std::vector<std::string>	params = getVector(cmdline, ' ');

	if (!user.isNicknameRegistered())
		nick = "*";
	else
		nick = user.getNickname();
		
	if (params.size() < 2) {
		errMsg = ERR_NEEDMOREPARAM(nick, "PASS");
		send(user.getSocket(), errMsg.c_str(), errMsg.size(), 0);
	}
	else {
		if (user.isPwdEntered()) {
			errMsg = ERR_ALREADYREGISTRED(nick);
			send(user.getSocket(), errMsg.c_str(), errMsg.size(), 0);
		}
		else if (params[1] != password) {
			errMsg = ERR_PASSWDMISMATCH(nick);
			send(user.getSocket(), errMsg.c_str(), errMsg.size(), 0);
		}
		else
			user.setPassword(true);
	}
	return (0);
}
