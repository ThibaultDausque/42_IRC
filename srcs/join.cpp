/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpipi <tpipi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 16:26:24 by tpipi             #+#    #+#             */
/*   Updated: 2025/06/20 15:46:07 by tpipi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"
#include "Channel.hpp"

#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>

int executeJoin(User origin, std::map<std::string, Channel> &channels, std::string cmdline)
{
	// 42 irc a changer avec le nom du serveur
	std::string errNeedMoreParams = ":42_irc 461 "+origin.getNickname()+" JOIN :Not enough parameters\r\n";
	
	std::istringstream stream(cmdline);
    std::vector<std::string> tokens;
    std::string token;

    while (std::getline(stream, token, ' ')) {
        tokens.push_back(token);
    }

	if (tokens.size() == 1)
		send(origin.getSocket(), errNeedMoreParams.c_str(), errNeedMoreParams.size(), 0);
	else if (tokens.size() == 2)
	{
		try
		{
			Channel &chan = channels.at(tokens[1]);
			chan.addUser(origin, false);
		}
		catch(const std::exception& e)
		{
			Channel channel(tokens[1], "", "", "");
			channel.addUser(origin, true);
			channels.insert(std::pair<std::string, Channel>(tokens[1], channel));
		}
		std::string	joinMsg = ":"+origin.getNickname()+"!"+origin.getUsername()+"@"+origin.getHostname()+" JOIN "+tokens[1]+"\r\n";
		send(origin.getSocket(), joinMsg.c_str(), joinMsg.size(), 0);
	}
	return 0; 
}