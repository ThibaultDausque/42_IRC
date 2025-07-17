/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpipi <tpipi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 13:49:41 by tpipi             #+#    #+#             */
/*   Updated: 2025/07/17 20:42:07 by tpipi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <map>
# include <string>
# include <sys/socket.h>
# include <ctime>
# include <sstream>

# include "User.hpp"
# include "numeric_reply.hpp"

class   Channel
{
	public :
		Channel(std::string name);
		Channel(std::string name, std::string topic, std::string modes, std::string key, int limit);
		Channel(const Channel &copy);
		
		// GETTERS
		std::map<User*, bool>	getUsers(void) const;
		std::string				getName(void) const;
		std::string				getTopic(void) const;
		std::string				getModes(void) const;
		std::string				getKey(void) const;
		int						getUserLimit(void) const;
		std::time_t				getLastTimeTopicChange(void) const;
		std::time_t				getCreationTime(void) const;
		std::string				getLastUserToChangeTopic(void) const;

		// SETTERS
		void					changeTopic(std::string newTopic, User newUser);
		void					setKey(std::string newKey);
		void					setUserLimit(int newUserLimit);				

		// EXCEPTIONS
		class InvalidChannelNameException : public std::exception
		{
			public:
				const char* what() const throw() {
					return "Invalid Channel Name Exception";
				}
		};

		// USERS RELATED FUNCTIONS
		bool					isUserConnected(std::string userNickname);
		bool					isUserOperator(std::string userNickname);
		void					addUser(User &user, bool isOperator);
		void					removeUser(std::string userNickname);
		void					kickUser(std::string userNickname, std::string originFullname, std::string reason);
		void					sendToEveryone(std::string message);

		// OPERATORS RELATED FUNCTIONS
		void					giveUserOperator(std::string userNickname);
		void					takeUserOperator(std::string userNickname);

		// MODES
		void					changeMode(std::string mode);
		bool					isChannelProtected(void);
		bool					onInviteMode(void);
		bool					onLimiteMode(void);
		bool					onTopicRestrictedMode(void);

		// OTHERS
		bool					doesChannelHaveATopic(void);
		int						getChannelSize(void);
		std::string				convertUNIXTimeToString(time_t time);

		Channel &operator=(Channel const &copy);

		~Channel(void);
	private :
		std::map<User*, bool>	_users;
		std::string				_name;
		std::string				_topic;
		std::string				_modes;
		std::string				_key;
		int						_userLimit;
		std::time_t				_lastTopicChange;
		std::time_t				_creationTime;
		std::string				_lastUserToChangeTopic;
};

#endif