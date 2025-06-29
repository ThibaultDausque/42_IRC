/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpipi <tpipi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 16:21:20 by tpipi             #+#    #+#             */
/*   Updated: 2025/06/28 11:30:22 by tpipi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP

# include <string>
# include <vector>
# include <map>

# include "Channel.hpp"
# include "User.hpp"

class Command
{
	public :
		Command(std::string cmd, std::string pre, std::string line);

		std::string	getCmd(void) const;
		std::string	getPrefix(void) const;

		~Command(void);
	private :
		const std::string			c_cmd;
		const std::string			c_prefix;
		std::vector<std::string>	args;
};

bool						isCmdValid(std::string cmd);
bool						doesChannelExist(std::map<std::string, Channel> &channels, std::string chanName);
std::vector<std::string>	getVector(std::string strToSplit, char delimiter);
bool						userConnectedOnAnyChannel(std::map<std::string, Channel> &channels, User &user);

#endif