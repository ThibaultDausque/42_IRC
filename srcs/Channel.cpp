/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpipi <tpipi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 13:56:40 by tpipi             #+#    #+#             */
/*   Updated: 2025/07/04 00:30:19 by tpipi            ###   ########.fr       */
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
	for (std::size_t i = 0; i < name.length(); i++) {
		if (name[i] == ' ' || name[i] == ',' || name[i] == 7)
			throw Channel::InvalidChannelNameException();
	}
	this->_name = name;
	this->_topic = "";
	this->_modes = "";
	this->_key = "";
	this->_userLimit = -1;
	this->_lastTopicChange = 0;
	this->_lastUserToChangeTopic = "";
}

Channel::Channel(std::string name, std::string topic, std::string modes, std::string key, int limit)
{
	if (name[0] != '#' && name[0] != '&')
		throw Channel::InvalidChannelNameException();
	for (std::size_t i = 0; i < name.length(); i++) {
		if (name[i] == ' ' || name[i] == ',' || name[i] == 7)
			throw Channel::InvalidChannelNameException();
	}
	this->_name = name;
	this->_topic = topic;
	this->_modes = modes;
	this->_key = key;
	this->_userLimit = limit;
	this->_lastTopicChange = 0;
	this->_lastUserToChangeTopic = "";
}

Channel::~Channel(void) {}

/*-----
GETTERS
-------*/

std::map<User*, bool> Channel::getUsers(void)
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

int	Channel::getUserLimit(void)
{
	return (this->_userLimit);
}

std::time_t	Channel::getLastTimeTopicChange(void)
{
	return (this->_lastTopicChange);
}

std::string	Channel::getLastUserToChangeTopic(void)
{
	return (this->_lastUserToChangeTopic);
}

/*-----
SETTERS
-------*/

void	Channel::changeTopic(std::string newTopic, User newUser)
{
	this->_topic = newTopic;
	this->_lastTopicChange = std::time(0);
	this->_lastUserToChangeTopic = newUser.getFullName();
}

void	Channel::setKey(std::string newKey)
{
	this->_key = newKey;
}

void	Channel::setUserLimit(int newUserLimit)
{
	this->_userLimit = newUserLimit;
}

/*--------------
FUNCTIONS MEMBER
----------------*/

bool	Channel::isUserConnected(std::string userNickname)
{
	std::string nick; 
	for (std::map<User*, bool>::iterator it = _users.begin(); it != _users.end(); it++) {
		nick = (*it->first).getNickname();
		if (nick == userNickname)
			return (true);
	}
	return (false);
}

bool	Channel::isUserOperator(std::string userNickname)
{
	std::string nick; 
	for (std::map<User*, bool>::iterator it = _users.begin(); it != _users.end(); it++) {
		nick = (*it->first).getNickname();
		if (nick == userNickname && it->second)
			return (true);
	}
	return (false);
}

void	Channel::addUser(User &user, bool isOperator)
{
	std::string	fullname = user.getFullName();
	std::string	joinMsg = ":"+fullname+" JOIN "+this->_name+"\r\n";

	this->_users.insert(std::pair<User*, bool>(&user, isOperator));
	this->sendToEveryone(joinMsg);
	
	if (doesChannelHaveATopic()) {
		std::string	rplTopic = RPL_TOPIC(user.getNickname(), this->_name, this->_topic);
		std::string	rplTopicWhoTime = RPL_TOPICWHOTIME(user.getNickname(), this->_name, this->_lastUserToChangeTopic, this->convertUNIXTimeToString());
		//send(user.getSocket(), rplTopic.c_str(), rplTopic.size(), 0);
		std::cout << rplTopic << std::endl;
		//send(user.getSocket(), rplTopicWhoTime.c_str(), rplTopicWhoTime.size(), 0);
		std::cout << rplTopicWhoTime << std::endl;
	}
	user.deleteAnInvitation(this->_name);
}

void	Channel::removeUser(std::string userNickname)
{
	std::string 					nick;
	std::map<User*, bool>::iterator	userToRemoveIter;
	
	for (std::map<User*, bool>::iterator it = _users.begin(); it != _users.end(); it++) {
		nick = (*it->first).getNickname();

		if (nick == userNickname)
			userToRemoveIter = it;
	}
	this->_users.erase(userToRemoveIter);
}

void	Channel::kickUser(std::string userNickname, std::string originFullname, std::string reason)
{
	std::string 					nick;
	std::string						kickMsg = ":"+originFullname+" KICK "+this->_name+" "+userNickname+" "+reason;
	std::map<User*, bool>::iterator	userToRemoveIter;
	
	for (std::map<User*, bool>::iterator it = _users.begin(); it != _users.end(); it++) {
		nick = (*it->first).getNickname();
		std::cout << kickMsg << std::endl;
		//send((*it->first).getSocket(), kickMsg.c_str(), kickMsg.size(), 0);

		if (nick == userNickname)
			userToRemoveIter = it;
	}
	this->_users.erase(userToRemoveIter);
}

void	Channel::sendToEveryone(std::string message)
{
	int	socket;
	for (std::map<User*, bool>::iterator it = _users.begin(); it != _users.end(); it++) {
		socket = (*it->first).getSocket();
		std::cout << message << std::endl;
		send(socket, message.c_str(), message.size(), 0);
	}
}

void	Channel::giveUserOperator(std::string userNickname)
{
	std::string nick;
	for (std::map<User*, bool>::iterator it = _users.begin(); it != _users.end(); it++) {
		nick = (*it->first).getNickname();
		if (nick == userNickname)
			it->second = true; 
	}
}

void	Channel::takeUserOperator(std::string userNickname)
{
	std::string nick;
	for (std::map<User*, bool>::iterator it = _users.begin(); it != _users.end(); it++) {
		nick = (*it->first).getNickname();
		if (nick == userNickname)
			it->second = false; 
	}
}

void	Channel::changeMode(std::string modeToAdd)
{
	this->_modes.append(modeToAdd);
	return ;
}

bool	Channel::isChannelProtected(void)
{
	return (this->_modes.find('k') != std::string::npos);
}

bool	Channel::onInviteMode(void)
{
	return (this->_modes.find('i') != std::string::npos);
}

bool	Channel::onLimiteMode(void)
{
	return (this->_modes.find('l') != std::string::npos);
}

bool	Channel::onTopicRestrictedMode(void)
{
	return (this->_modes.find('t') != std::string::npos);
}

bool	Channel::doesChannelHaveATopic(void)
{
	return (!this->_topic.empty());
}

int	Channel::getChannelSize(void)
{
	return (this->_users.size());
}

std::string	Channel::convertUNIXTimeToString(void)
{
	std::stringstream ss;
	ss << this->_lastTopicChange;
	std::string lastChange = ss.str();

	return (lastChange);
}
