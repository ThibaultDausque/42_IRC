/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   comman_utils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpipi <tpipi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 16:27:00 by tpipi             #+#    #+#             */
/*   Updated: 2025/08/03 00:05:07 by tpipi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.hpp"

bool	doesChannelExist(std::map<std::string, Channel> &channels, std::string chanName)
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

Channel	*getChannelPtr(std::map<std::string, Channel> &channels, std::string chanName)
{
	if (doesChannelExist(channels, chanName))
		return (&channels.at(chanName));
	return (NULL);
}

bool	doesClientExist(std::vector<User> &clients, std::string clientName)
{
	for (std::vector<User>::iterator it = clients.begin(); it != clients.end(); ++it) {
		if (it->getNickname() == clientName)
			return (true);
	}
	return (false);
}

User	*getUserPtr(std::vector<User> &clients, std::string clientName)
{
	for (std::vector<User>::iterator it = clients.begin(); it != clients.end(); ++it) {
		if (it->getNickname() == clientName)
			return (&(*it));
	}
	return (NULL);
}

User	&getUserRfr(std::vector<User> &clients, int fd)
{
	for (std::vector<User>::iterator it = clients.begin(); it != clients.end(); ++it) {
		if (it->getSocket() == fd)
			return (*it);
	}
	return (clients[0]);
}

std::vector<std::string>	getVector(std::string strToSplit, char delimiter)
{
	std::istringstream stream(strToSplit);
	std::vector<std::string> tokens;
	std::string token;

	while (std::getline(stream, token, delimiter)) {
		tokens.push_back(token);
	}
	return tokens;
}

bool	userConnectedOnAnyChannel(std::map<std::string, Channel> &channels, User &user)
{
	std::map<User*, bool>	userList;
	
	for (std::map<std::string, Channel>::iterator it = channels.begin(); it != channels.end(); it++) {
		userList = it->second.getUsers();
		for (std::map<User*, bool>::iterator userIt = userList.begin(); userIt != userList.end(); userIt++) {
			if ((*userIt->first).getNickname() == user.getNickname())
				return (true);
		}
	}
	return (false);
}

void	deleteEmptyChannel(std::map<std::string, Channel> &channels)
{
	std::vector<std::string>	channelToRemoveList;

	for (std::map<std::string, Channel>::iterator chanIt = channels.begin(); chanIt != channels.end(); ++chanIt) {
		if (chanIt->second.getChannelSize() == 0)
			channelToRemoveList.push_back(chanIt->first);
	}

	for (size_t i = 0; i < channelToRemoveList.size(); i++) {
		channels.erase(channelToRemoveList[i]);
	}
}

void	createReason(std::vector<std::string> reasonVector, std::string *reason, size_t index)
{
	if (reasonVector.size() > index) {
		if (reasonVector[index].size() > 0 && reasonVector[index][0] == ':') {
			for (size_t i = index; i < reasonVector.size(); i++) {
				(*reason).append(reasonVector[i]);
				if (i + 1 == reasonVector.size())
					break ;
				(*reason).append(" ");
			}
			(*reason).append("\r\n");
		}
		else
			(*reason).append(":"+reasonVector[index]+"\r\n");
	}
	else
		(*reason) = DEFAULT_REASON;
}

bool	isReceiverAChannel(std::string receiver)
{
	if (receiver.size() > 0 && (receiver[0] == '&' || receiver[0] == '#'))
		return (true);
	return (false);
}

size_t	doesTextHaveCRLF(const std::string text)
{
	return (text.find("\r\n"));
}
