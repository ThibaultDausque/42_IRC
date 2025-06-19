/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpipi <tpipi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 13:49:41 by tpipi             #+#    #+#             */
/*   Updated: 2025/06/19 14:05:33 by tpipi            ###   ########.fr       */
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
		Channel(std::string name);

		std::map<User, bool>	getUsers(void);
		std::string				getName(void);

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

		bool					isUserConnected(std::string userNickname);
		bool					isUserOperator(std::string userNickname);
		void					addUser(User user, bool isOperator);
		bool					removeUser(std::string origin, std::string userNickname);
		void					giveUserOperator(std::string origin, std::string userNickname);
		void					takeUserOperator(std::string origin, std::string userNickname);
		int						getChannelSize(void);

		~Channel(void);
	private :
		std::map<User, bool>	_users;
		std::string				_name;
};

#endif