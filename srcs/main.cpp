#include "Server.hpp"
#include <csignal>
#include <stdexcept>
#include <cctype>

void	handle_signal(int sig)
{
	(void) sig;
	throw std::runtime_error("Server terminated\n");
}

int	main(int ac, char **av)
{

	std::string	server_pwd;
	unsigned int	server_port;
	if (ac != 3)
	{
		std::cout << "* two arguments needed *" << std::endl;
		return 0;
	}
	
	server_port = atoi(av[1]);
	server_pwd = av[2];
	std::string	port(av[1]);
	for (size_t i = 0; i < port.size(); i++)
	{
		if (!isdigit(av[1][i]))
		{
			std::cout << "Error: incorrect port." << std::endl;
			return 1;
		}
	}
	if (server_port < 1024 || server_port > 65535)
	{
		std::cout << "Error: " << server_port << " is incorrect." << std::endl;
		return 1;
	}

	try
	{
		Server	server(av[2], server_port);
		std::signal(SIGINT, handle_signal);
		server.initServer();
		server.runServer();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
	return 0;
}
