#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <netinet/in.h>
# include <cstdlib>

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
		void	acceptConnection();
};

#endif
