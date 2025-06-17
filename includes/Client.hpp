/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpipi <tpipi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 12:52:11 by tpipi             #+#    #+#             */
/*   Updated: 2025/06/17 15:13:21 by tpipi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>
# include <stdexcept> 
# include <iostream>
# include <ctype.h>

class Client
{
	public :
		Client(std::string nn, std::string un, std::string hn, std::string rn, int FD);

		std::string getNickname(void) const;
		std::string getUsername(void) const;
		std::string getHostname(void) const;
		std::string getRealname(void) const;
		int         getFD(void) const;

		class HasInvalidCharacterException : public std::exception
		{
			public:
				const char* what() const throw() {
					return "Invalid Character Exception";
				}
		};

		bool operator<(const Client &other) const;

		~Client(void);
	private :
		std::string _nickname;
		std::string _username;
		std::string _hostname;
		std::string _realname;
		int         _FD;
};

std::ostream &operator<<(std::ostream &os, const Client &client);

bool	hasInvalidChar(std::string str);
bool	hasNonAlphanumCharacter(std::string str);

#endif