#include "Server.hpp"
#include <csignal>

bool	forever = true;

void	handleSignal(int signal)
{
	std::cout << "* caught signal * " << signal << std::endl;
	forever = false;
}

int	main(int ac, char **av)
{

	// std::string	server_pwd;
	// unsigned int	server_port;
	(void) av;
	if (ac != 3)
	{
		std::cout << "* three arguments needed *" << std::endl;
		return 0;
	}
	
	// server_pwd = av[2];
	// server_port = atoi(av[1]);
	
	// if (server_port < 0 || server_port > 65535)
	// {
	// 	std::cout << "Error: " << server_port << " is incorrect." << std::endl;
	// 	return 1;
	// }
	Server	server("toto", 8080);
	if (std::signal(SIGINT, handleSignal))
	{
		for (size_t	i = 0; i < server.getTab().size(); i++)
			close(server.getTabElement(i).fd);
	}
	server.initServer();
	server.runServer();
	
	return 0;
}
