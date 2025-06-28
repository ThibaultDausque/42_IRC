/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpipi <tpipi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 12:52:11 by tpipi             #+#    #+#             */
/*   Updated: 2025/06/17 15:13:21 by tpipi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

# include <string>
# include <stdexcept> 
# include <iostream>
# include <ctype.h>
# include <vector>

class User
{
	public :
		User(std::string nn, std::string un, std::string hn, std::string rn, int socket);

		std::string getNickname(void) const;
		std::string getUsername(void) const;
		std::string getHostname(void) const;
		std::string getRealname(void) const;
		int         getSocket(void) const;

		class HasInvalidCharacterException : public std::exception
		{
			public:
				const char* what() const throw() {
					return "Invalid Character Exception";
				}
		};

		void		addAnInvitation(std::string channelName);
		void		deleteAnInvitation(std::string channelName);
		bool		isInvitedTo(std::string channelName);
		std::string	getFullName(void);

		bool operator<(const User &other) const;

		~User(void);
	private :
		std::string 				_nickname;
		std::string 				_username;
		std::string 				_hostname;
		std::string 				_realname;
		int							_socket;
		std::vector<std::string>	_inviteList;
};

std::ostream &operator<<(std::ostream &os, const User &user);

bool	hasInvalidChar(std::string str);
bool	hasNonAlphanumCharacter(std::string &str);

#endif