/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpipi <tpipi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 16:21:20 by tpipi             #+#    #+#             */
/*   Updated: 2025/07/14 17:19:23 by tpipi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP

# include <string>
# include <vector>
# include <map>
# include <set>
# include <limits.h>
# include <errno.h>

# include "Channel.hpp"
# include "User.hpp"
# include "Server.hpp"

class Server;

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

bool						doesChannelExist(std::map<std::string, Channel> &channels, std::string chanName);
Channel						*getChannelPtr(std::map<std::string, Channel> &channels, std::string chanName);
bool						doesClientExist(std::vector<User> &clients, std::string clientName);
User						*getUserPtr(std::vector<User> &clients, std::string clientName);
User						&getUserRfr(std::vector<User> &clients, int fd);
std::vector<std::string>	getVector(std::string strToSplit, char delimiter);
bool						userConnectedOnAnyChannel(std::map<std::string, Channel> &channels, User &user);
void						deleteEmptyChannel(std::map<std::string, Channel> &channels);
void						createReason(std::vector<std::string> reasonVector, std::string *reason, size_t index);
bool						isReceiverAChannel(std::string receiver);

int executeJoin(User &origin, std::map<std::string, Channel> &channels, std::string cmdline);
int executeNames(User &origin, std::map<std::string, Channel> &channels, std::string cmdline, std::vector<User> *users);
int executeNick(User &user, std::map<std::string, Channel> &channels, std::string cmdline, std::vector<User> &users);
int	executeUser(User &user, std::string cmdline);
int executeKick(User &origin, std::map<std::string, Channel> &channels, std::string cmdline);
int executePrivmsg(User &origin, std::map<std::string, Channel> &channels, std::string cmdline, std::vector<User> &users);
int executeInvite(User &origin, std::map<std::string, Channel> &channels, std::string cmdline, std::vector<User> &users);
int executePart(User &user, std::map<std::string, Channel> &channels, std::string cmdline);
int executeTopic(User &user, std::map<std::string, Channel> &channels, std::string cmdline);
int executePing(User &user, std::string cmdline);
int executeWho(User &origin, std::map<std::string, Channel> &channels, std::string cmdline, std::vector<User> &users);
int	executePass(User &user, std::string cmdline, std::string password);
int executeMode(User &user, std::map<std::string, Channel> &channels, std::string cmdline, std::vector<User> &users);

#endif