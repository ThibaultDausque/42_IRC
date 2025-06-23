/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpipi <tpipi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 16:26:24 by tpipi             #+#    #+#             */
/*   Updated: 2025/06/23 12:53:59 by tpipi            ###   ########.fr       */
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

#define ERR_NEEDMOREPARAM_NB ":42.42_irc.fr 461 "
#define ERR_NEEDMOREPARAM " JOIN :Not enough parameters\r\n"
#define ERR_NOSUCHCHANNEL_NB ":42.42_irc.fr 403 "
#define ERR_NOSUCHCHANNEL " :No such channel\r\n"
#define ERR_BADCHANNELKEY_NB ":42.42_irc.fr 475 "
#define ERR_BADCHANNELKEY " :Cannot join channel (+k)\r\n"

static bool	doesChannelExist(std::map<std::string, Channel> &channels, std::string chanName)
{
	try
	{
		channels.at(chanName);
		return (true);
	}
	catch(const std::exception& e)
	{
		return (false);
	}
}

static std::vector<std::string>	getVector(std::string strToSplit, char delimiter)
{
	std::istringstream stream(strToSplit);
	std::vector<std::string> tokens;
	std::string token;
	std::string	chanName;

	while (std::getline(stream, token, delimiter)) {
		tokens.push_back(token);
	}
	return tokens;
}

int executeJoin(User origin, std::map<std::string, Channel> &channels, std::string cmdline)
{
	std::string	fullname = ":"+origin.getNickname()+"!"+origin.getUsername()+"@"+origin.getHostname();
	std::string errNeedMoreParams = ERR_NEEDMOREPARAM_NB+origin.getNickname()+ERR_NEEDMOREPARAM;
	std::string	chanName;
	
	std::vector<std::string> params = getVector(cmdline, ' ');
	std::vector<std::string> keysParam;
	std::vector<std::string> channelsParam;

	if (params.size() == 1)
		std::cout << "\n" << errNeedMoreParams << "\n" << std::endl;
		//send(origin.getSocket(), errNeedMoreParams.c_str(), errNeedMoreParams.size(), 0);
	else if (params.size() > 1)
	{
		channelsParam = getVector(params[1], ',');
		if (params.size() > 2)
			keysParam = getVector(params[2], ',');
		
		for (size_t i = 0; i < channelsParam.size(); i++) {
			chanName = channelsParam[i];
			std::string	joinMsg = fullname+" JOIN "+chanName+"\r\n";
			std::string errNoSuchChannel = ERR_NOSUCHCHANNEL_NB+origin.getNickname()+" "+chanName+ERR_NOSUCHCHANNEL;
			std::string errBadChannelKey = ERR_BADCHANNELKEY_NB+origin.getNickname()+" "+chanName+ERR_BADCHANNELKEY;
			
			if (doesChannelExist(channels, chanName))
			{
				Channel *chan = &channels.at(chanName);

				if ((*chan).isChannelProtected())
				{
					if (params.size() > 2) {
						if (keysParam.size() <= i || (*chan).getKey() != keysParam[i])
							std::cout << "\n" << errBadChannelKey << "\n" << std::endl;
							//send(origin.getSocket(), errBadChannelKey.c_str(), errBadChannelKey.size(), 0);
						else
						{
							(*chan).addUser(origin, false);
							//send(origin.getSocket(), joinMsg.c_str(), joinMsg.size(), 0);
							std::cout << "\n" << joinMsg << "\n" << std::endl;
						}
					}
					else
						std::cout << "\n" << errBadChannelKey << "\n" << std::endl;
						//send(origin.getSocket(), errBadChannelKey.c_str(), errBadChannelKey.size(), 0);
				}
				else
				{
					(*chan).addUser(origin, false);
					std::cout << "\n" << joinMsg << "\n" << std::endl;
					//send(origin.getSocket(), joinMsg.c_str(), joinMsg.size(), 0);
				}
			}
			else
			{
				try
				{
					Channel channel(chanName, "", "", "");
					channel.addUser(origin, true);
					channels.insert(std::pair<std::string, Channel>(channel.getName(), channel));
					//send(origin.getSocket(), joinMsg.c_str(), joinMsg.size(), 0);
					std::cout << "\n" << joinMsg << "\n" << std::endl;
				}
				catch(const std::exception& e)
				{
					//send(origin.getSocket(), errNoSuchChannel.c_str(), errNoSuchChannel.size(), 0);
					std::cout << "\n" << errNoSuchChannel << "\n" << std::endl;
				}
			}
		}
	}
	return 0; 
}