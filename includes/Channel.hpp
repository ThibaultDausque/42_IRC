/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpipi <tpipi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 13:49:41 by tpipi             #+#    #+#             */
/*   Updated: 2025/06/20 14:35:35 by tpipi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <map>
# include <string>
# include "User.hpp"

class   Channel
{
	public :
		Channel(std::string name, std::string topic, std::string modes, std::string key);

		// GETTERS
		std::map<User, bool>	getUsers(void);
		std::string				getName(void);
		std::string				getTopic(void);
		std::string				getModes(void);
		std::string				getKey(void);

		// SETTERS
		void					changeTopic(std::string newTopic);
		void					setKey(std::string newKey);

		// EXCEPTIONS
		class InvalidChannelNameException : public std::exception
		{
			public:
				const char* what() const throw() {
					return "Invalid Channel Name Exception";
				}
		};

		class UserIsNotOperatorException : public std::exception
		{
			public:
				const char* what() const throw() {
					return "User Is Not Operator Exception";
				}
		};

		// USERS RELATED FUNCTIONS
		bool					isUserConnected(std::string userNickname);
		bool					isUserOperator(std::string userNickname);
		void					addUser(User user, bool isOperator);
		bool					removeUser(std::string origin, std::string userNickname);

		// OPERATORS RELATED FUNCTIONS
		void					giveUserOperator(std::string origin, std::string userNickname);
		void					takeUserOperator(std::string origin, std::string userNickname);

		// MODES
		void					changeMode(std::string modeToAdd);
		bool					isChannelProtected(void);

		// OTHERS
		int						getChannelSize(void);

		~Channel(void);
	private :
		std::map<User, bool>	_users;
		std::string				_name;
		std::string				_topic;
		std::string				_modes;
		std::string				_key;
};

#endif