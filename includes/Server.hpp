#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <netinet/in.h>
# include <sys/socket.h>
# include <cstdlib>
# include <unistd.h>
# include <poll.h>
# include <vector>
# include "User.hpp"
# include "Command.hpp"
# include "Channel.hpp"
# include <map>
# include <string.h>
# include <fcntl.h>

class Server
{
	private:
		int				_serverFd;
		std::string		_serverIp;
		std::string		_serverPwd;
		unsigned int	_port;
		bool			_connected;
		std::vector<User>	_clients;
		std::vector<struct pollfd>	_tab;
		std::map<std::string, Channel>	_channels;

	public:
		Server(std::string _pwd, unsigned int _port);
		~Server();
		int							initServer(void);
		void						acceptNewClient(void);
		std::string					readMessage(int	fd_client);
		void						runServer(void);
		void						eraseClient(void);
		void						signalHandler(int sig);
		std::vector<struct pollfd>&	getTab();
		pollfd&						getTabElement(size_t idx);
		// int		parseCmd(char* buff);
		std::string					parseNick(const char* buff);
};

#endif
