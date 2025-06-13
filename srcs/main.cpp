#include "Server.hpp"

int	main(int ac, char **av)
{
	std::string	server_pwd;
	unsigned int	server_port;

	if (ac != 3)
	{
		std::cout << "* three arguments needed *" << std::endl;
		return 0;
	}
	return 0;

	server_pwd = av[2];
	server_port = atoi(av[1]);

	if (server_port < 0 || server_port > 65535)
	{
		std::cout << "Error: " << server_port << " is incorrect." << std::endl;
		return 1;
	}
	return 0;
}
