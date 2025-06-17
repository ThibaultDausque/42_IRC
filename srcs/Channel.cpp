/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpipi <tpipi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 13:56:40 by tpipi             #+#    #+#             */
/*   Updated: 2025/06/17 15:49:29 by tpipi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

/*-------------------------
CONSTRUCTORS AND DESTRUCTOR
---------------------------*/

Channel::Channel(std::string name) : _name(name) {}

Channel::~Channel(void) {}

/*-----
GETTERS
-------*/

std::map<Client, bool> Channel::getClients(void)
{
	return (this->_clients);
}

std::string	Channel::getName(void)
{
	return (this->_name);
}

/*--------------
FUNCTIONS MEMBER
----------------*/

void	Channel::addClient(Client client, bool isOperator)
{
	this->_clients.insert(std::pair<Client, bool>(client, isOperator));
}

void	Channel::removeClient(std::string clientNickname)
{
	std::string nick; 
	for (std::map<Client, bool>::iterator it = _clients.begin(); it != _clients.end(); it++) {
		nick = it->first.getNickname();
		if (nick == clientNickname)
		{
			_clients.erase(it);
			break ;
		}
	}
}

bool	Channel::isClientConnected(std::string clientNickname)
{
	std::string nick; 
	for (std::map<Client, bool>::iterator it = _clients.begin(); it != _clients.end(); it++) {
		nick = it->first.getNickname();
		if (nick == clientNickname)
			return (true);
	}
	return (false);
}

bool	Channel::isClientOperator(std::string clientNickname)
{
	std::string nick; 
	for (std::map<Client, bool>::iterator it = _clients.begin(); it != _clients.end(); it++) {
		nick = it->first.getNickname();
		if (nick == clientNickname && it->second)
			return (true);
	}
	return (false);
}