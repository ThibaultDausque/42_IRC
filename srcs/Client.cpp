/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpipi <tpipi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 13:01:39 by tpipi             #+#    #+#             */
/*   Updated: 2025/06/19 13:41:36 by tdausque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

/*-------------------------
CONSTRUCTORS AND DESTRUCTOR
---------------------------*/

Client::Client(std::string& nickname, std::string& username, std::string& hostname, std::string& realname, int FD)
{
	// si c'est vide c'est une not enough param
	if (nickname.length() > 9 || nickname.length() < 1)
		throw std::length_error("Length Error : Nickname Length Must Be Between 1-9 Characters.");
	if (hasInvalidChar(nickname))
		throw Client::HasInvalidCharacterException();
	if (username.length() < 1)
		throw std::length_error("Length Error : Username Length Must Be Between 1-9 Characters.");
	if (hasNonAlphanumCharacter(username))
		throw Client::HasInvalidCharacterException();
	if (username.length() > 9)
		username = username.substr(0, 9);
	_nickname = nickname;
	_username.push_back('~');
	_username.append(username);
	_hostname = hostname;
	_realname = realname;
	_FD = FD;
}

Client::~Client(void) {}

/*-----
GETTERS
-------*/

std::string Client::getNickname(void) const
{
	return (this->_nickname);
}

std::string Client::getUsername(void) const
{
	return (this->_username);
}

std::string Client::getHostname(void) const
{
	return (this->_hostname);
}

std::string Client::getRealname(void) const
{
	return (this->_realname);
}

int			Client::getFD(void) const
{
	return (this->_FD);
}

/*--------------
FUNCTIONS MEMBER
----------------*/

/*-------
FUNCTIONS
---------*/

std::ostream &operator<<(std::ostream &os, const Client &client)
{
	os << "Nickname : " << client.getNickname() <<
		"\nUsername : " << client.getUsername() <<
		"\nHostname : " << client.getHostname() <<
		"\nRealname : " << client.getRealname() <<
		"\nFD : " << client.getFD();
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
	for (std::size_t i = 0; i < str.length(); i++) {
		if (!isalnum(str[i]))
			return (true);
	}
	return (false);
}
