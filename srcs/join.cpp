/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpipi <tpipi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 16:26:24 by tpipi             #+#    #+#             */
/*   Updated: 2025/06/25 01:22:20 by tpipi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"
#include "Channel.hpp"
#include "NumericReply.hpp"

#include <sstream>

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

	while (std::getline(stream, token, delimiter)) {
		tokens.push_back(token);
	}
	return tokens;
}

static void	removeUserFromEveryChannel(std::map<std::string, Channel> &channels, User origin)
{
	std::string				originNickname = origin.getNickname();
	std::string				originFullname = origin.getFullName();
	std::string				partMsg;
	std::map<User, bool>	userList;

	for (std::map<std::string, Channel>::iterator chanIt = channels.begin(); chanIt != channels.end(); chanIt++) {
		userList = chanIt->second.getUsers();
		partMsg = ":"+originFullname+" PART "+chanIt->second.getName()+"\r\n";

		if (chanIt->second.isUserConnected(originNickname)) {
			for (std::map<User, bool>::iterator userIt = userList.begin(); userIt != userList.end(); userIt++) {
				User tmp = userIt->first;
				std::cout << partMsg << std::endl;
				//send(tmp.getSocket(), partMsg.c_str(), partMsg.size(), 0);
				chanIt->second.removeUser(originNickname);
			}
		}
	}
}

int executeJoin(User origin, std::map<std::string, Channel> &channels, std::string cmdline)
{
	std::string	chanName;
	std::string errNeedMoreParam = ERR_NEEDMOREPARAM(origin.getNickname(), "JOIN");

	std::vector<std::string> keysParam;
	std::vector<std::string> channelsParam;
	std::vector<std::string> params = getVector(cmdline, ' ');

	if (params.size() == 1)
		std::cout << "\n" << errNeedMoreParam << "\n" << std::endl;
		//send(origin.getSocket(), errNeedMoreParams.c_str(), errNeedMoreParams.size(), 0);
	else if (params.size() > 1)
	{
		channelsParam = getVector(params[1], ',');
		if (params.size() > 2)
			keysParam = getVector(params[2], ',');
		
		if (channelsParam[0] == "0" && channelsParam.size() == 1)
			removeUserFromEveryChannel(channels, origin);
		for (size_t i = 0; i < channelsParam.size(); i++) {
			chanName = channelsParam[i];
			std::string errNoSuchChannel = ERR_NOSUCHCHANNEL(origin.getNickname(), chanName);
			std::string errBadChannelKey = ERR_BADCHANNELKEY(origin.getNickname(), chanName);
			std::string errChannelIsFull = ERR_CHANNELISFULL(origin.getNickname(), chanName);
			std::string errInviteOnlyChan = ERR_INVITEONLYCHAN(origin.getNickname(), chanName);
			
			if (doesChannelExist(channels, chanName))
			{
				Channel *chan = &channels.at(chanName);

				if ((*chan).isChannelProtected() && (params.size() <= 2 || (keysParam.size() <= i || (*chan).getKey() != keysParam[i])))
					std::cout << "\n" << errBadChannelKey << "\n" << std::endl;
					//send(origin.getSocket(), errBadChannelKey.c_str(), errBadChannelKey.size(), 0);
				else if ((*chan).getChannelSize() >= (*chan).getUserLimit())
					std::cout << "\n" << errChannelIsFull << "\n" << std::endl;
					//send(origin.getSocket(), errChannelIsFull.c_str(), errChannelIsFull.size(), 0);
				else if ((*chan).onInviteMode() && !origin.isInvitedTo(chanName))
					std::cout << "\n" << errInviteOnlyChan << "\n" << std::endl;
					//send(origin.getSocket(), errInviteOnlyChan.c_str(), errInviteOnlyChan.size(), 0);
				else
					(*chan).addUser(origin, false);
			}
			else
			{
				try
				{
					Channel channel(chanName);
					channel.addUser(origin, true);
					channels.insert(std::pair<std::string, Channel>(channel.getName(), channel));
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