#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <netinet/in.h>
# include <sys/socket.h>
# include <cstdlib>
# include <unistd.h>
# include <poll.h>
# include <list>
# include "User.hpp"
# include "command.hpp"
# include "Channel.hpp"
# include <map>
# include <string.h>
# include <fcntl.h>

class Server
{
	private:
		int								_serverFd;
		std::string						_serverIp;
		std::string						_serverPwd;
		unsigned int					_port;
		bool							_connected;
		std::list<User>					_clients;
		std::vector<struct pollfd>		_tab;
		std::map<std::string, Channel>	_channels;

	public:
		Server(std::string _pwd, unsigned int _port);
		Server(const Server &copy);
		Server &operator=(const Server &copy);
		~Server();

		int							initServer(void);
		int							runCommands(std::string cmdline, int tabIndex);
		void						acceptNewClient(void);
		void						runServer(void);
		std::string					readMessage(int	fd_client);
		std::vector<struct pollfd>	&getTab(void);
		pollfd&						getTabElement(size_t idx);
};

#endif
