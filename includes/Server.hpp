#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <netinet/in.h>
# include <sys/socket.h>
# include <cstdlib>
# include <unistd.h>
# include <poll.h>
# include <vector>
# include "Client.hpp"
# include <map>
# include <string.h>

class Server
{
	private:
		int				_serverFd;
		std::string		_serverIp;
		std::string		_serverPwd;
		unsigned int	_port;
		bool			_connected;
		std::vector<Client>	_clients;

	public:
		Server(std::string _pwd, unsigned int _port);
		~Server();
		int		initServer(void);
		void	acceptNewClient(void);
		void	runServer(void);
		void	eraseClient(void);
		void	signalHandler(int sig);
};

#endif
