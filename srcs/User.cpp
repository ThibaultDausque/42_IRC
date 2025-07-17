/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpipi <tpipi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 13:01:39 by tpipi             #+#    #+#             */
/*   Updated: 2025/07/17 20:31:35 by tpipi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

/*-------------------------
CONSTRUCTORS AND DESTRUCTOR
---------------------------*/

User::User(int socket)
{
	_nickname = "";
	_username = "";
	_hostname = "";
	_realname = "";
	_socket = socket;
	_pwdEntered = false;
}

User::User(std::string nn, std::string un, std::string hn, std::string rn, int socket)
{
	// si c'est vide c'est une not enough param
	if (nn.length() > 9 || nn.length() < 1)
		throw std::length_error("Length Error : Nickname Length Must Be Between 1-9 Characters.");
	if (hasInvalidChar(nn))
		throw User::HasInvalidCharacterException();
	if (un.length() < 1)
		throw std::length_error("Length Error : Username Length Must Be Between 1-9 Characters.");
	if (hasNonAlphanumCharacter(un))
		throw User::HasInvalidCharacterException();
	if (un.length() > 9)
		un = un.substr(0, 9);
	_nickname = nn;
	_username.push_back('~');
	_username.append(un);
	_hostname = hn;
	_realname = rn;
	_socket = socket;
	_pwdEntered = false;
}

User::User(const User &copy)
{
	_nickname = copy.getNickname();
	_username = copy.getUsername();
	_hostname = copy.getHostname();
	_realname = copy.getRealname();
	_socket = copy.getSocket();
	_pwdEntered = true;
}

User	&User::operator=(const User &copy)
{
	if (this != &copy) {
		_nickname = copy.getNickname();
		_username = copy.getUsername();
		_hostname = copy.getHostname();
		_realname = copy.getRealname();
		_socket = copy.getSocket();
		_pwdEntered = true;
	}
	return (*this);
}

User::~User(void) {}

/*-----
GETTERS
-------*/

std::string User::getNickname(void) const
{
	return (this->_nickname);
}

std::string User::getUsername(void) const
{
	return (this->_username);
}

std::string User::getHostname(void) const
{
	return (this->_hostname);
}

std::string User::getRealname(void) const
{
	return (this->_realname);
}

int	User::getSocket(void) const
{
	return (this->_socket);
}

void	User::setNickname(std::string &str)
{
	this->_nickname = str;
}

void	User::setUsername(std::string &str)
{
	this->_username = str;
}

void	User::setHostname(std::string &str)
{
	this->_hostname = str;
}

void	User::setRealname(std::string &str)
{
	this->_realname = str;
}

void	User::setPassword(bool val)
{
	this->_pwdEntered = val;
}

/*--------------
FUNCTIONS MEMBER
----------------*/

bool	User::isInvitedTo(std::string channelName)
{
	return std::find(_inviteList.begin(), _inviteList.end(), channelName) != _inviteList.end();
}

void	User::addAnInvitation(std::string channelName)
{
	if (!isInvitedTo(channelName))
		this->_inviteList.push_back(channelName);
}

void	User::deleteAnInvitation(std::string channelName)
{
	for (std::vector<std::string>::iterator it = _inviteList.begin(); it != _inviteList.end(); it++) {
		if (*it == channelName) {
			this->_inviteList.erase(it);
			break ;
		}
	}
}

bool	User::isNicknameRegistered(void)
{
	if (this->getNickname() == "")
		return (false);
	return (true);
}

bool	User::isUsernameRegistered(void)
{
	if (this->getUsername() == "")
		return (false);
	return (true);
}

bool	User::isUserRegistered(void)
{
	if (this->isNicknameRegistered() && this->isUsernameRegistered()) {
		std::string rplWelcome = RPL_WELCOME(this->getNickname());
		std::string	rplYourHost = RPL_YOURHOST(this->getNickname());
		std::string	rplCreated = RPL_CREATED(this->getNickname());
		std::string	rplMyInfo = RPL_MYINFO(this->getNickname());
		std::string	rplISupport = RPL_ISUPPORT(this->getNickname());

		send(this->getSocket(), rplWelcome.c_str(), rplWelcome.size(), 0);
		send(this->getSocket(), rplYourHost.c_str(), rplYourHost.size(), 0);
		send(this->getSocket(), rplCreated.c_str(), rplCreated.size(), 0);
		send(this->getSocket(), rplMyInfo.c_str(), rplMyInfo.size(), 0);
		send(this->getSocket(), rplISupport.c_str(), rplISupport.size(), 0);
		return (true);
	}
	return (false);
}

bool	User::isPwdEntered(void)
{
	return this->_pwdEntered;
}

std::string	User::getFullName(void)
{
	std::string fullname = this->getNickname()+"!"+this->getUsername()+"@"+this->getHostname();
	return (fullname);
}

bool User::operator<(const User &other) const
{
	return (this->_nickname < other._nickname);
}

/*-------
FUNCTIONS
---------*/

std::ostream &operator<<(std::ostream &os, const User &user)
{
	os << "Nickname : " << user.getNickname() <<
		"\nUsername : " << user.getUsername() <<
		"\nHostname : " << user.getHostname() <<
		"\nRealname : " << user.getRealname() <<
		"\nFD : " << user.getSocket();
	return os;
}

bool	hasInvalidChar(std::string& str)
{
	std::string invalidChar = " ,*?!@.";
	std::string invalidFirstChar = "$:#&~%+";

	for (std::size_t i = 0; i < str.length(); i++) {
		if (i == 0)
		{
			for (std::size_t f = 0; f < invalidFirstChar.length(); f++) {
				if (invalidFirstChar[f] == str[i])
					return (true);
			}
		}
		for (std::size_t j = 0; j < invalidChar.length(); j++) {
			if (invalidChar[j] == str[i] || (str[i] >= 0 && str[i] <= 32) || str[i] == 127)
				return (true);
		}
	}
	return (false);
}

bool	hasNonAlphanumCharacter(std::string& str)
{
	bool	res = false;
	for (std::size_t i = 0; i < str.length(); i++) {
		if (!isalnum(str[i]) && str[i] != '.' && str[i] != '_' && str[i] != '-' && str[i] != '+')
			res = true;
	}
	return (res);
}
