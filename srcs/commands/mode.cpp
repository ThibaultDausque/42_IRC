/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpipi <tpipi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 17:14:46 by tpipi             #+#    #+#             */
/*   Updated: 2025/07/11 23:28:25 by tpipi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "NumericReply.hpp"
#include "Command.hpp"
#include "User.hpp"


static int ft_stoi(std::string limit)
{
	long int l = strtol(limit.c_str(), NULL, 10);
    if (errno || l > INT_MAX || l < INT_MIN)
	return (-1);
    return (static_cast<int>(l));
}

static std::string	makeModesArgument(Channel channel)
{
	std::ostringstream	oss;
	std::string			modesArg = "";
	std::string			modes = channel.getModes();

	for (size_t i = 0; i < modes.size(); i++) {
		if (!modesArg.empty())
			modesArg.append(" ");
		if (modes[i] == 'l') {
			oss << channel.getUserLimit();
			modesArg += oss.str();
		}
		if (modes[i] == 'k')
			modesArg.append(channel.getKey());
	}
	return (modesArg);
}

static size_t getNumberArgNeeded(std::string modes)
{
	size_t		nb = 0;
	char		sign = '+';

	for (size_t i = 0; i < modes.size(); i++) {
		if (modes[i] == '+' || modes[i] == '-')
			sign = modes[i];
		else if (modes[i] == 'k' || modes[i] == 'o' || (modes[i] == 'l' && sign == '+'))
			nb++;
	}
	return (nb);
}

static void	builtModeMsg(std::string &msg, char sign, char mode, char &lastSignSent)
{
	char	counterSign = '+';
	
	if (sign == '+')
			counterSign = '-';
	if (lastSignSent == counterSign || msg[msg.size() - 1] == ' ')
		msg.push_back(sign);
	msg.push_back(mode);
	lastSignSent = sign;
}

int executeMode(User &user, std::map<std::string, Channel> &channels, std::string cmdline, std::vector<User> users)
{
	int							modeArgIndex = 3;
	char						sign = '+';
	char						lastSignSent = '+';
	std::string					modeWithSign;
	std::string					errMsg;
	std::string					modeMsg = ":"+user.getFullName()+" MODE ";
	std::string					modesArg;
	std::vector<std::string>	params = getVector(cmdline, ' ');

	if (params.size() < 2) {
		errMsg = ERR_NEEDMOREPARAM(user.getNickname(), "MODE");
		send(user.getSocket(), errMsg.c_str(), errMsg.size(), 0);
	}
	else {
		Channel *chan = getChannelPtr(channels, params[1]);

		if (chan == NULL) {
			errMsg = ERR_NOSUCHCHANNEL(user.getNickname(), params[1]);
			send(user.getSocket(), errMsg.c_str(), errMsg.size(), 0);
		}
		else if (params.size() == 2) {
			modesArg = makeModesArgument(*chan);
			std::string rplChannelModeis = RPL_CHANNELMODEIS(user.getNickname(), chan->getName(), "+"+chan->getModes(), modesArg);
			std::string rplCreationTime = RPL_CREATIONTIME(user.getNickname(), chan->getName(), chan->convertUNIXTimeToString(chan->getCreationTime()));
			send(user.getSocket(), rplChannelModeis.c_str(), rplChannelModeis.size(), 0);
			send(user.getSocket(), rplCreationTime.c_str(), rplCreationTime.size(), 0);
		}
		else {
			std::string	modeParam = params[2];
			modeMsg.append(chan->getName()+" ");

			for (size_t i = 0; i < modeParam.size(); i++) {
				if (modeParam[i] != 'o' && modeParam[i] != 'i' && modeParam[i] != 'k' && modeParam[i] != 'l' && modeParam[i] != 't' && modeParam[i] != '-' && modeParam[i] != '+') {
					errMsg = ERR_UNKNOWNMODE(user.getNickname(), modeParam[i]);
					send(user.getSocket(), errMsg.c_str(), errMsg.size(), 0);
					return (1);
				}
			}
			if (getNumberArgNeeded(params[2]) + 3 > params.size()) {
				errMsg = ERR_NEEDMOREPARAM(user.getNickname(), "MODE");
				send(user.getSocket(), errMsg.c_str(), errMsg.size(), 0);
				return (1);
			}
			if (!chan->isUserOperator(user.getNickname())) {
				errMsg = ERR_CHANOPRIVSNEEDED(user.getNickname(), chan->getName());
				send(user.getSocket(), errMsg.c_str(), errMsg.size(), 0);
				return (1);
			}

			for (size_t index = 0; index < modeParam.size(); index++) {
				if (modeParam[index] == '+' || modeParam[index] == '-')
					sign = modeParam[index];
				else if (modeParam[index] == 't') {
					if ((sign == '+' && !chan->onTopicRestrictedMode()) || (sign == '-' && chan->onTopicRestrictedMode())) {
						modeWithSign.push_back(sign);
						modeWithSign.push_back(modeParam[index]);
						chan->changeMode(modeWithSign);
						builtModeMsg(modeMsg, sign, 't', lastSignSent);
					}
				}
				else if (modeParam[index] == 'i') {
					if ((sign == '+' && !chan->onInviteMode()) || (sign == '-' && chan->onInviteMode())) {
						modeWithSign.push_back(sign);
						modeWithSign.push_back(modeParam[index]);
						chan->changeMode(modeWithSign);
						builtModeMsg(modeMsg, sign, 'i', lastSignSent);
					}
				}
				else if (modeParam[index] == 'k') {
					if ((sign == '+' || (sign == '-' && chan->isChannelProtected()))) {
						modeWithSign.push_back(sign);
						modeWithSign.push_back(modeParam[index]);
						chan->changeMode(modeWithSign);
						chan->setKey(params[modeArgIndex]);
						builtModeMsg(modeMsg, sign, 'k', lastSignSent);
						if (sign == '-')
							modesArg.append("* ");
						else
							modesArg.append(params[modeArgIndex]+" ");
						modeArgIndex++;
					}
				}
				else if (modeParam[index] == 'l') {
					if ((sign == '+' && ft_stoi(params[modeArgIndex]) > 0) || (sign == '-' && chan->onLimiteMode())) {
						modeWithSign.push_back(sign);
						modeWithSign.push_back(modeParam[index]);
						chan->changeMode(modeWithSign);
						builtModeMsg(modeMsg, sign, 'l', lastSignSent);
						if (sign == '+') {
							modesArg.append(params[modeArgIndex]+" ");
							chan->setUserLimit(ft_stoi(params[modeArgIndex]));
							modeArgIndex++;
						}
					}
				}
				else if (modeParam[index] == 'o') {
					if (!userConnectedOnAnyChannel(channels, *getUserPtr(users, params[modeArgIndex]))) {
						errMsg = ERR_NOSUCHNICK(user.getNickname(), params[modeArgIndex]);
						send(user.getSocket(), errMsg.c_str(), errMsg.size(), 0);
					}
					else if (!chan->isUserConnected(params[modeArgIndex])) {
						errMsg = ERR_USERNOTINCHANNEL(user.getNickname(), params[modeArgIndex], chan->getName());
						send(user.getSocket(), errMsg.c_str(), errMsg.size(), 0);
					}
					else {
						if (sign == '-' && chan->isUserOperator(params[modeArgIndex])) {
							chan->takeUserOperator(params[modeArgIndex]);
							builtModeMsg(modeMsg, sign,'o', lastSignSent);
							modesArg.append(params[modeArgIndex]+" ");
						}
						else if (sign == '+' && !chan->isUserOperator(params[modeArgIndex])) {
							chan->giveUserOperator(params[modeArgIndex]);
							builtModeMsg(modeMsg, sign, 'o', lastSignSent);
							modesArg.append(params[modeArgIndex]+" ");
						}
						modeArgIndex++;
					}
				}
			}
			if (modeMsg != ":"+user.getFullName()+" MODE "+chan->getName()+" ") {
				modeMsg.append(" "+modesArg+"\r\n");
				chan->sendToEveryone(modeMsg);
			}
		}
	}
	return (0);
}
