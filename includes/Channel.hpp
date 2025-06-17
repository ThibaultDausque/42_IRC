/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpipi <tpipi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 13:49:41 by tpipi             #+#    #+#             */
/*   Updated: 2025/06/17 15:19:55 by tpipi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <map>
# include <string>
# include "Client.hpp"

class   Channel
{
	public :
		Channel(std::string name);

		std::map<Client, bool>	getClients(void);
		std::string				getName(void);

		void					addClient(Client client, bool isOperator);
		void					removeClient(std::string clientNickname);
		// void					giveClientOperator(Client origin, std::string clientNickname);
		// void					takeClientOperator(Client origin, std::string clientNickname);
		bool					isClientConnected(std::string clientNickname);
		bool					isClientOperator(std::string clientNickname);

		~Channel(void);
	private :
		std::map<Client, bool>	_clients;
		std::string				_name;
};

#endif