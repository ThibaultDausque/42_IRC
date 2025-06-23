/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NumericReply.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpipi <tpipi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 15:24:32 by tpipi             #+#    #+#             */
/*   Updated: 2025/06/23 15:51:13 by tpipi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NUMERICREPLY_HPP
# define NUMERICREPLY_HPP

#define SERVER_HOSTNAME ":42.42_irc.fr"

#define ERR_NEEDMOREPARAM(nick, channel) SERVER_HOSTNAME" 461 "+nick+" "+channel+" :Not enough parameters\r\n"
#define ERR_NOSUCHCHANNEL(nick, channel) SERVER_HOSTNAME" 403 "+nick+" "+channel+" :No such channel\r\n"
#define ERR_BADCHANNELKEY(nick, channel) SERVER_HOSTNAME" 475 "+nick+" "+channel+" :Cannot join channel (+k)\r\n"
#define ERR_CHANNELISFULL(nick, channel) SERVER_HOSTNAME" 471 "+nick+" "+channel+" :Cannot join channel (+l)\r\n"
#define ERR_INVITEONLYCHAN(nick, channel) SERVER_HOSTNAME" 473 "+nick+" "+channel+" :Cannot join channel (+l)\r\n"

#endif