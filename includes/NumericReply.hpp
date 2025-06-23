/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NumericReply.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpipi <tpipi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 15:24:32 by tpipi             #+#    #+#             */
/*   Updated: 2025/06/23 15:25:01 by tpipi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NUMERICREPLY_HPP
# define NUMERICREPLY_HPP

#define SERVER_HOSTNAME ":42.42_irc.fr"

#define ERR_NEEDMOREPARAM_NB " 461 "
#define ERR_NEEDMOREPARAM " :Not enough parameters\r\n"
#define ERR_NOSUCHCHANNEL_NB " 403 "
#define ERR_NOSUCHCHANNEL " :No such channel\r\n"
#define ERR_BADCHANNELKEY_NB " 475 "
#define ERR_BADCHANNELKEY " :Cannot join channel (+k)\r\n"
#define ERR_CHANNELISFULL_NB " 471 "
#define ERR_CHANNELISFULL " :Cannot join channel (+l)\r\n"
#define ERR_INVITEONLYCHAN_NB " 473 "
#define ERR_INVITEONLYCHAN " :Cannot join channel (+i)\r\n"

#endif