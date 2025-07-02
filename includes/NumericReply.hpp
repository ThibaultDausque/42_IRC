/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NumericReply.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpipi <tpipi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 15:24:32 by tpipi             #+#    #+#             */
/*   Updated: 2025/07/02 21:24:19 by tpipi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NUMERICREPLY_HPP
# define NUMERICREPLY_HPP

# define SERVER_HOSTNAME ":irc.42angouleme.fr"
# define CLIENT_HOSTNAME "irc.42angouleme.fr"
# define DEFAULT_REASON ":No reason given\r\n"

# define ERR_NOSUCHNICK(nick, nonexistant) SERVER_HOSTNAME" 401 "+nick+" "+nonexistant+" :No such nick/channel\r\n" 
# define ERR_NEEDMOREPARAM(nick, command) SERVER_HOSTNAME" 461 "+nick+" "+command+" :Not enough parameters\r\n"
# define ERR_ALREADYREGISTRED(nick) SERVER_HOSTNAME" 462 "+nick+" :You may not reregister\r\n"
# define ERR_NOSUCHCHANNEL(nick, channel) SERVER_HOSTNAME" 403 "+nick+" "+channel+" :No such channel\r\n"
# define ERR_BADCHANNELKEY(nick, channel) SERVER_HOSTNAME" 475 "+nick+" "+channel+" :Cannot join channel (+k)\r\n"
# define ERR_CHANNELISFULL(nick, channel) SERVER_HOSTNAME" 471 "+nick+" "+channel+" :Cannot join channel (+l)\r\n"
# define ERR_INVITEONLYCHAN(nick, channel) SERVER_HOSTNAME" 473 "+nick+" "+channel+" :Cannot join channel (+i)\r\n"
# define ERR_NONICKNAMEGIVEN(nick) SERVER_HOSTNAME" 431 "+nick+" :No nickname given\r\n"
# define ERR_ERRONEUSNICKNAME(nick, newnick) SERVER_HOSTNAME" 432 "+nick+" "+newnick+" :Erroneus nickname\r\n"
# define ERR_NICKNAMEINUSE(nick, newnick) SERVER_HOSTNAME" 433 "+nick+" "+newnick+" :Nickname is already in use\r\n"
# define ERR_NOTREGISTERED(nick) SERVER_HOSTNAME" 451 "+nick+" :You have not registered\r\n"
# define ERR_CHANOPRIVSNEEDED(nick, channel) SERVER_HOSTNAME" 482 "+nick+" "+channel+" :You're not channel operator\r\n"
# define ERR_USERNOTINCHANNEL(originNick, targetNick, channel) SERVER_HOSTNAME" 441 "+originNick+" "+targetNick+" "+channel+" :They aren't on that channel\r\n"
# define ERR_NOTONCHANNEL(nick, channel) SERVER_HOSTNAME" 442 "+nick+" "+channel+" :You're not on that channel\r\n"
# define ERR_NORECIPIENT(nick, command) SERVER_HOSTNAME" 411 "+nick+" :No recipient given ("+command+")\r\n"
# define ERR_NOTEXTTOSEND(nick) SERVER_HOSTNAME" 412 "+nick+" :No text to send\r\n"
# define ERR_USERONCHANNEL(nick, invited, channel) SERVER_HOSTNAME" 443 "+nick+" "+invited+" "+channel+" :is already on channel\r\n" 
# define ERR_UNKNOWNCOMMAND(nick, command) SERVER_HOSTNAME" 421 "+nick+" "+command+" :Unknown command\r\n"

# define RPL_WELCOME(nick) SERVER_HOSTNAME" 001 "+nick+" :Welcome to the 42Angouleme IRC Network, "+nick+"\r\n"
# define RPL_YOURHOST(nick) SERVER_HOSTNAME" 002 "+nick+" :Your host is irc.42angouleme.fr, running version 4.2\r\n"
# define RPL_CREATED(nick) SERVER_HOSTNAME" 003 "+nick+" :This server was created Tue Jun 10 2025 at 16:56:03 CET\r\n"
# define RPL_MYINFO(nick) SERVER_HOSTNAME" 004 "+nick+" irc.42angouleme.fr version 4.2 o iklt kl\r\n"
# define RPL_ISUPPORT(nick) SERVER_HOSTNAME" 005 "+nick+" CHANMODES=,k,l,it CHANTYPES=#& PREFIX=@ :are supported by this server\r\n"
# define RPL_TOPIC(nick, channel, topic) SERVER_HOSTNAME" 332 "+nick+" "+channel+" :"+topic+"\r\n"
# define RPL_TOPICWHOTIME(nick, channel, fullname, setat) SERVER_HOSTNAME" 333 "+nick+" "+channel+" "+fullname+" "+setat+"\r\n"
# define RPL_INVITING(nick, invited, channel) SERVER_HOSTNAME" 341 "+nick+" "+invited+" "+channel+"\r\n"
# define RPL_NAMREPLY(nick, channel) SERVER_HOSTNAME" 353 "+nick+" = "+channel+" :"
# define RPL_ENDOFNAMES(nick, channel) SERVER_HOSTNAME" 366 "+nick+" "+channel+" :End of /NAMES list.\r\n"

#endif