/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpipi <tpipi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 13:56:40 by tpipi             #+#    #+#             */
/*   Updated: 2025/06/19 14:08:13 by tpipi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

/*-------------------------
CONSTRUCTORS AND DESTRUCTOR
---------------------------*/

Channel::Channel(std::string name)
{
	if (name[0] != '#' && name[0] != '&')
		throw Channel::InvalidChannelNameException();
	// continuer ici
	this->_name = name;
}

Channel::~Channel(void) {}

/*-----
GETTERS
-------*/

std::map<User, bool> Channel::getUsers(void)
{
	return (this->_users);
}

std::string	Channel::getName(void)
{
	return (this->_name);
}

/*--------------
FUNCTIONS MEMBER
----------------*/

bool	Channel::isUserConnected(std::string userNickname)
{
	std::string nick; 
	for (std::map<User, bool>::iterator it = _users.begin(); it != _users.end(); it++) {
		nick = it->first.getNickname();
		if (nick == userNickname)
			return (true);
	}
	return (false);
}

bool	Channel::isUserOperator(std::string userNickname)
{
	std::string nick; 
	for (std::map<User, bool>::iterator it = _users.begin(); it != _users.end(); it++) {
		nick = it->first.getNickname();
		if (nick == userNickname && it->second)
			return (true);
	}
	return (false);
}

void	Channel::addUser(User user, bool isOperator)
{
	this->_users.insert(std::pair<User, bool>(user, isOperator));
}

bool	Channel::removeUser(std::string origin, std::string userNickname)
{
	if (!isUserOperator(origin))
		throw Channel::UserIsNotOperatorException();
	std::string nick;
	for (std::map<User, bool>::iterator it = _users.begin(); it != _users.end(); it++) {
		nick = it->first.getNickname();
		if (nick == userNickname)
		{
			_users.erase(it);
			return (true);
		}
	}
	return (false);
}

void	Channel::giveUserOperator(std::string origin, std::string userNickname)
{
	if (!isUserOperator(origin))
		throw Channel::UserIsNotOperatorException();
	std::string nick;
	for (std::map<User, bool>::iterator it = _users.begin(); it != _users.end(); it++) {
		nick = it->first.getNickname();
		if (nick == userNickname)
			it->second = true; 
	}
}

void	Channel::takeUserOperator(std::string origin, std::string userNickname)
{
	if (!isUserOperator(origin))
		throw Channel::UserIsNotOperatorException();
	std::string nick;
	for (std::map<User, bool>::iterator it = _users.begin(); it != _users.end(); it++) {
		nick = it->first.getNickname();
		if (nick == userNickname)
			it->second = false; 
	}
}

int	Channel::getChannelSize(void)
{
	return (this->_users.size());
}