/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   names.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpipi <tpipi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 17:28:58 by tpipi             #+#    #+#             */
/*   Updated: 2025/06/27 22:26:44 by tpipi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "NumericReply.hpp"
#include "User.hpp"
#include "Channel.hpp"
#include "Command.hpp"

int executeNames(User origin, std::map<std::string, Channel> &channels, std::string cmdline)
	{
	std::string					chanName;
	std::string					originNick = origin.getNickname();
	std::string					rplEndOfNames;
	std::string					rplNamReply;
	std::map<User, bool>		userList;
	std::vector<std::string>    params = getVector(cmdline, ' ');

	if (params.size() > 1) {
		std::vector<std::string> channelsParam = getVector(params[1], ',');

		for (size_t i = 0; i < channelsParam.size(); i++) {
			chanName = channelsParam[i];
			rplEndOfNames =  RPL_ENDOFNAMES(originNick, chanName);

			if (doesChannelExist(channels, chanName)) {
				rplNamReply = RPL_NAMREPLY(originNick, chanName);
				userList = channels.at(chanName).getUsers();

				for (std::map<User, bool>::iterator it = userList.begin(); it != userList.end(); ++it) {
					User tmp = it->first;
					if (it->second)
						rplNamReply.push_back('@');
					rplNamReply.append(tmp.getFullName()+" ");
				}
				rplNamReply.append("\r\n");
				std::cout << rplNamReply << std::endl;
				//send(origin.getSocket(), rplNamReply.c_str(), rplNamReply.size(), 0);
			}
			std::cout << rplEndOfNames << std::endl;
			//send(origin.getSocket(), rplEndOfNames.c_str(), rplEndOfNames.size(), 0);
		}
	}
	return 0;
}
