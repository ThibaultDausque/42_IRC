/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpipi <tpipi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 13:56:40 by tpipi             #+#    #+#             */
/*   Updated: 2025/06/20 14:46:36 by tpipi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

/*-------------------------
CONSTRUCTORS AND DESTRUCTOR
---------------------------*/

Channel::Channel(std::string name, std::string topic, std::string modes, std::string key)
{
	if (name[0] != '#' && name[0] != '&')
		throw Channel::InvalidChannelNameException();
	for (std::size_t i = 0; i < name.length(); i++) {
		if (name[i] == ' ' || name[i] == ',' || name[i] == 7)
			throw Channel::InvalidChannelNameException();
	}
	this->_topic = topic;
	this->_name = name;
	this->_modes = modes;
	this->_key = key;
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

std::string	Channel::getTopic(void)
{
	return (this->_topic);
}

std::string	Channel::getModes(void)
{
	return (this->_modes);
}

std::string	Channel::getKey(void)
{
	return (this->_key);
}


/*-----
SETTERS
-------*/

void	Channel::changeTopic(std::string newTopic)
{
	this->_topic = newTopic;
}

void	Channel::setKey(std::string newKey)
{
	this->_key = newKey;
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

void	Channel::changeMode(std::string modeToAdd)
{
	(void)modeToAdd;
	// faire en meme temps que la commande mode
	return ;
}

bool	Channel::isChannelProtected(void)
{
	return (this->_modes.find('k') != std::string::npos);
}

int	Channel::getChannelSize(void)
{
	return (this->_users.size());
}