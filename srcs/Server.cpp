#include "Server.hpp"
#include <functional>
#include <netinet/in.h>
#include <sys/socket.h>

Server::Server(std::string _pwd, unsigned int _port)
{
	this->_serverFd = 0;
	this->_serverIp = "toto";
	this->_serverPwd = _pwd;
	this->_port = _port;
	this->_connected = 0;
}

Server::~Server()
{

}

int	Server::initServer(void)
{
	struct sockaddr_in	serverAddress;
	// AF_INET = specifies the IPv4 protocol family
	// SOCK_STREAM = defines that the TCP type socket
	this->_serverFd = socket(AF_INET, SOCK_STREAM, 0);
	
	// Structure for handling internet addresses
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(this->_port);
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	
	// link a socket with an IP and a port
	if (bind(_serverFd, (sockaddr *)&serverAddress, sizeof(serverAddress)))
		throw std::runtime_error("Error: bind failed\n");
	// The Server listen on 5 port max
	if (listen(_serverFd, 5))
		throw std::runtime_error("Error: listen failed\n");
	
	return 0;
}

void	Server::runServer(void)
{
	int		clientSocket;
	char	buffer[1024] = {0};

	// wait for a client to connect
	clientSocket = accept(this->_serverFd, NULL, NULL);
	recv(clientSocket, buffer, sizeof(buffer), 0);
	std::cout << "Client Message: " << buffer << std::endl;
	close(this->_serverFd);
}
