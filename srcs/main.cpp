#include "Server.hpp"
#include <csignal>
#include <stdexcept>

void	handle_signal(int sig)
{
	(void) sig;
	throw std::runtime_error("* signal handled *");
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
	//
	try
	{
		Server	server("toto", 8080);
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
