#include "Server.hpp"
#include <functional>
#include <netinet/in.h>
#include <sys/socket.h>

Server::Server(std::string _pwd, unsigned int _port)
{
	this->_serverFd = 0;
	this->_serverIp = NULL;
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
	serverAdress.sin_port = htons(this->_port);
	serverAdress.sin_addr.s_addr = INADDR_ANY;
	
	// link a socket with an IP and a port
	if (!bind(serverFd, (sockaddr *)&serverAddress, sizeof(serverAddress)))
	{
		std::cout << "Error: bind failed" << std::endl;
		return 1;
	}
	if (!listen(serverFd, 5))
	{
		std::cout << "Error: listen failed" << std::endl;
		return 1;
	}


	return 0;
}
