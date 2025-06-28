/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpipi <tpipi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 16:26:42 by tpipi             #+#    #+#             */
/*   Updated: 2025/06/28 16:50:22 by tpipi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "Channel.hpp"
#include "NumericReply.hpp"

int	executeUser(User &user, std::string cmdline)
{
	std::string					originNick;
	std::vector<std::string>	params = getVector(cmdline, ' ');

	if (user.getNickname() != "")
		originNick = user.getNickname();
	else
		originNick = "*";
	std::string errNeedMoreParam = ERR_NEEDMOREPARAM(originNick, "USER");
	std::string errAlreadyRegistred = ERR_ALREADYREGISTRED(originNick);
	
	if (params.size() < 5)
		std::cout << errNeedMoreParam << std::endl;
		//send(user->getSocket(), errNeedMoreParam.c_str(), errNeedMoreParam.size(), 0);
	else if (user.getUsername() != "")
		std::cout << errNeedMoreParam << std::endl;
		//send(user->getSocket(), errNeedMoreParam.c_str(), errNeedMoreParam.size(), 0);
	else
	{
		std::cout << std::endl;
	}

}