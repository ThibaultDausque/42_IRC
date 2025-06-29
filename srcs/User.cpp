/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpipi <tpipi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 13:01:39 by tpipi             #+#    #+#             */
/*   Updated: 2025/06/29 10:57:23 by tdausque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

/*-------------------------
CONSTRUCTORS AND DESTRUCTOR
---------------------------*/

User::User(std::string nn, std::string un, std::string hn, std::string rn, int socket)
{
	// si c'est vide c'est une not enough param
	if (nn.length() > 9 || nn.length() < 1)
		throw std::length_error("Length Error : Nickname Length Must Be Between 1-9 Characters.");
	// if (hasInvalidChar(nn))
		// throw User::HasInvalidCharacterException("Invalid characters");
	if (un.length() < 1)
		throw std::length_error("Length Error : Username Length Must Be Between 1-9 Characters.");
	// if (hasNonAlphanumCharacter(un))
	// 	throw User::HasInvalidCharacterException("Characters non alpha numeric");
	if (un.length() > 9)
		un = un.substr(0, 9);
	_nickname = nn;
	_username.push_back('~');
	_username.append(un);
	_hostname = hn;
	_realname = rn;
	_socket = socket;
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

int			User::getSocket(void) const
{
	return (this->_socket);
}

void	User::setNickname(std::string& Nickname)
{
	this->_nickname = Nickname;
}

void	User::setUsername(std::string& Username)
{
	this->_username = Username;
}

void	User::setHostname(std::string& Hostname)
{
	this->_hostname = Hostname;
}

void	User::setRealname(std::string& Realname)
{
	this->_realname = Realname;
}

/*--------------
FUNCTIONS MEMBER
----------------*/

void	User::addAnInvitation(std::string channelName)
{
	this->_inviteList.push_back(channelName);
}

void	User::deleteAnInvitation(std::string channelName)
{
	for (std::vector<std::string>::iterator it = this->_inviteList.begin(); it != this->_inviteList.end(); it++) {
		if (*it == channelName) {
			this->_inviteList.erase(it);
			break ;
		}
	}
}

bool	User::isInvitedTo(std::string channelName)
{
	std::string caca = (channelName);
	std::vector<std::string>::iterator it;
	for (it = _inviteList.begin(); it != _inviteList.end(); ++it) {
		if (*it == channelName)
			return (true);
	}
	return (false);
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
		if (!isalnum(str[i]) && str[i] != '.' && str[i] != '_' && str[i] != '-')
			res = true;
	}
	return (res);
}
