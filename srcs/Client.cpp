#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <cstdlib>
#include "Server.hpp"
#include <string.h>

int	main(int ac, char **av)
{
	int		clientSocket;
	struct sockaddr_in	serverAddress;

	if (ac != 2)
		return 0;
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(8080);
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
	
	Server	server("toto", 8080);
	server.initServer();
	send(clientSocket, av[1], strlen(av[1]), 0);
	server.runServer();

	close(clientSocket);
	return 0;
}
