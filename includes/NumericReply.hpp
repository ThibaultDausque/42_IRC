/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NumericReply.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpipi <tpipi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 15:24:32 by tpipi             #+#    #+#             */
/*   Updated: 2025/06/28 16:25:03 by tpipi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NUMERICREPLY_HPP
# define NUMERICREPLY_HPP

# define SERVER_HOSTNAME ":42.42_irc.fr"

# define ERR_NEEDMOREPARAM(nick, channel) SERVER_HOSTNAME" 461 "+nick+" "+channel+" :Not enough parameters\r\n"
# define ERR_ALREADYREGISTRED(nick) SERVER_HOSTNAME" 462 "+nick+" :You may not reregister\r\n"
# define ERR_NOSUCHCHANNEL(nick, channel) SERVER_HOSTNAME" 403 "+nick+" "+channel+" :No such channel\r\n"
# define ERR_BADCHANNELKEY(nick, channel) SERVER_HOSTNAME" 475 "+nick+" "+channel+" :Cannot join channel (+k)\r\n"
# define ERR_CHANNELISFULL(nick, channel) SERVER_HOSTNAME" 471 "+nick+" "+channel+" :Cannot join channel (+l)\r\n"
# define ERR_INVITEONLYCHAN(nick, channel) SERVER_HOSTNAME" 473 "+nick+" "+channel+" :Cannot join channel (+l)\r\n"
# define ERR_NONICKNAMEGIVEN(nick) SERVER_HOSTNAME" 431 "+nick+" :No nickname given\r\n"
# define ERR_ERRONEUSNICKNAME(nick, newnick) SERVER_HOSTNAME" 432 "+nick+" "+newnick+" :Erroneus nickname\r\n"
# define ERR_NICKNAMEINUSE(nick, newnick) SERVER_HOSTNAME" 433 "+nick+" "+newnick+" :Nickname is already in use\r\n"

# define RPL_TOPIC(nick, channel, topic) SERVER_HOSTNAME" 332 "+nick+" "+channel+" :"+topic+"\r\n"
# define RPL_TOPICWHOTIME(nick, channel, fullname, setat) SERVER_HOSTNAME" 333 "+nick+" "+channel+" "+fullname+" "+setat+"\r\n"
# define RPL_NAMREPLY(nick, channel) SERVER_HOSTNAME" 353 "+nick+" = "+channel+" :"
# define RPL_ENDOFNAMES(nick, channel) SERVER_HOSTNAME" 366 "+nick+" "+channel+" :End of /NAMES list.\r\n"

#endif