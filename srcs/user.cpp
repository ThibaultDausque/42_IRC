/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpipi <tpipi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 16:26:42 by tpipi             #+#    #+#             */
/*   Updated: 2025/07/01 17:45:38 by tpipi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "Channel.hpp"
#include "NumericReply.hpp"

int	executeUser(User &user, std::string cmdline)
{
	std::string					userNick;
	std::string					username;
	std::string					realname;
	std::vector<std::string>	params = getVector(cmdline, ' ');

	if (user.isNicknameRegistered())
		userNick = user.getNickname();
	else
		userNick = "*";
	std::string errNeedMoreParam = ERR_NEEDMOREPARAM(userNick, "USER");
	std::string errAlreadyRegistred = ERR_ALREADYREGISTRED(userNick);
	
	if (user.isUsernameRegistered())
		std::cout << errAlreadyRegistred << std::endl;
		//send(user->getSocket(), errAlreadyRegistred.c_str(), errAlreadyRegistred.size(), 0);
	else if (params.size() < 5)
		std::cout << errNeedMoreParam << std::endl;
		//send(user->getSocket(), errNeedMoreParam.c_str(), errNeedMoreParam.size(), 0);
	else
	{
		if (userNick == "*")
			userNick = "unknown";
		
		if (params[1].size() < 1 || hasNonAlphanumCharacter(params[1]))
			user.setUsername(userNick);
		else
		{
			username.append("~");
			if (params[1].length() > 9)
				params[1] = params[1].substr(0, 9);
			username.append(params[1]);
			user.setUsername(username);
		}

		user.setHostname(CLIENT_HOSTNAME);

		if (params[4].size() < 2 || params[4][0] != ':')
			user.setRealname(params[4]);
		else
		{
			params[4].erase(0, 1); // remove the ':'
			for (size_t i = 4; i < params.size(); i++) {
				realname.append(params[i]);
				if (i + 1 == params.size())
					break ;
				realname.append(" ");
			}
			user.setRealname(realname);
		}
		user.isUserRegistered();
	}
	return (0);
}
