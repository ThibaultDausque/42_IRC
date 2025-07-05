#pragma once

#define CLIENT(nick, user)(nick + "!" + user + "@localhost")
#define PRIVMSG(client, target, message)(":" + client + " PRIVMSG " + target + " :" + message + "\r\n")
#define NOTICE(client, target, message)(":" + client + " NOTICE " + target + " :" + message + "\r\n")

#define WELCOME(nickname)(":127.0.0.1 001 " + nickname + " :Welcome to the Internet Relay Network, " + nickname + "\r\n")
#define HOST(name) std::string(":127.0.0.1 002 " + name + " :Your host is 127.0.0.1, running version ft_irc-1.0\r\n")
#define DATE(name) std::string(":127.0.0.1 003 " + name + " :This server was created Mon June 2 2025 at 08:00 UTC\r\n")
#define MYINFO(name) std::string(":127.0.0.1 004 " + name + " 127.0.0.1 ft_irc-1.0 -\r\n")
#define SERVCAP(name) std::string(":127.0.0.1 005 " + name + " :CHANTYPES=# PREFIX=(o)@ MODES=1 TARGMAX=JOIN:1,PRIVMSG:1,KICK:1\r\n")
#define MOTDST(name) std::string(":127.0.0.1 375 " + name + " :- Message of the day -\n" + "Today we learn why people talk.\r\n")
#define MOTDED(name) std::string(":127.0.0.1 376 " + name + " :End of /MOTD command\r\n")
