/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpipi <tpipi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 13:01:39 by tpipi             #+#    #+#             */
/*   Updated: 2025/06/17 15:14:12 by tpipi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

/*-------------------------
CONSTRUCTORS AND DESTRUCTOR
---------------------------*/

User::User(std::string nn, std::string un, std::string hn, std::string rn, int FD)
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
	_FD = FD;
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

int			User::getFD(void) const
{
	return (this->_FD);
}

/*--------------
FUNCTIONS MEMBER
----------------*/

bool User::operator<(const User &other) const
{
	return (this->_nickname < other._nickname);
}

/*-------
FUNCTIONS
---------*/

std::ostream &operator<<(std::ostream &os, const User &User)
{
	os << "Nickname : " << User.getNickname() <<
		"\nUsername : " << User.getUsername() <<
		"\nHostname : " << User.getHostname() <<
		"\nRealname : " << User.getRealname() <<
		"\nFD : " << User.getFD();
	return os;
}

bool	hasInvalidChar(std::string str)
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

bool	hasNonAlphanumCharacter(std::string str)
{
	for (std::size_t i = 0; i < str.length(); i++) {
		if (!isalnum(str[i]))
			return (true);
	}
	return (false);
}
