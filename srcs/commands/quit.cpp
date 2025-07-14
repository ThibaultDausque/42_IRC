/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpipi <tpipi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 18:43:17 by tpipi             #+#    #+#             */
/*   Updated: 2025/07/14 19:59:14 by tpipi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "numeric_reply.hpp"
#include "command.hpp"

int executeQuit(User &user, std::map<std::string, Channel> &channels, std::string cmdline, std::vector<User> &users, std::vector<struct pollfd>	tab)
{
	int							fd = user.getSocket();
	std::map<User*, bool> 		userList;
	std::set<int>				usersSharingChannelFD;
	std::vector<std::string>	params = getVector(cmdline, ' ');
	std::string					reason;
	std::string					quitMsg = ":"+user.getFullName()+" QUIT ";

	createReason(params, &reason, 1);
	quitMsg.append(reason);

	for (std::map<std::string, Channel>::iterator chanIt = channels.begin(); chanIt != channels.end(); ++chanIt) {
		if (chanIt->second.isUserConnected(user.getNickname())) {
			chanIt->second.removeUser(user.getNickname());
			userList = chanIt->second.getUsers();
			
			for (std::map<User*, bool>::iterator userIt = userList.begin(); userIt != userList.end(); ++userIt) {
				usersSharingChannelFD.insert(userIt->first->getSocket());
			}
		}
	}
	for (std::set<int>::iterator fdIt = usersSharingChannelFD.begin(); fdIt != usersSharingChannelFD.end(); ++fdIt)
		send(*fdIt, quitMsg.c_str(), quitMsg.size(), 0);
	deleteEmptyChannel(channels);

	for (std::vector<User>::iterator userIt = users.begin(); userIt != users.end(); userIt++) {
		if (userIt->getNickname() == user.getNickname()) {
			users.erase(userIt);
			break ;
		}
	}
	for (std::vector<struct pollfd>::iterator fdIt = tab.begin(); fdIt != tab.end(); fdIt++) {
		if (fdIt->fd == fd) {
			tab.erase(fdIt);
			break ;
		}
	}
	close(fd);
	return (0);
}
