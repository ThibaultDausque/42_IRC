/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpipi <tpipi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 12:52:11 by tpipi             #+#    #+#             */
/*   Updated: 2025/07/03 11:14:36 by tdausque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

# include <string>
# include <stdexcept> 
# include <iostream>
# include <ctype.h>
# include <vector>
# include <set>

# include "NumericReply.hpp"

class User
{
	public :
		User(int socket);
		User(std::string nn, std::string un, std::string hn, std::string rn, int socket);
		std::string getNickname(void) const;
		std::string getUsername(void) const;
		std::string getHostname(void) const;
		std::string getRealname(void) const;
		int         getSocket(void) const;

		void		setNickname(std::string &str);
		void		setUsername(std::string &str);
		void		setHostname(const char *str);
		void		setRealname(std::string &str);
		
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
		bool		isUsernameRegistered(void);
		bool		isNicknameRegistered(void);
		bool		isUserRegistered(void);
		std::string	getFullName(void);

		bool operator<(const User &other) const;

		~User(void);
	private :
		std::string 				_nickname;
		std::string 				_username;
		std::string 				_hostname;
		std::string 				_realname;
		int							_socket;
		std::set<std::string>		_inviteList;
};

std::ostream &operator<<(std::ostream &os, const User &user);

bool	hasInvalidChar(std::string& str);
bool	hasNonAlphanumCharacter(std::string& str);

#endif
