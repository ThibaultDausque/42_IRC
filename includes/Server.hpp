#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <netinet/in.h>
# include <sys/socket.h>
# include <cstdlib>
# include <unistd.h>
# include <poll.h>

class Server
{
	private:
		int				_serverFd;
		std::string		_serverIp;
		std::string		_serverPwd;
		unsigned int	_port;
		bool			_connected;
	
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
