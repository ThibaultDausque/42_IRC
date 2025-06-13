#include "Server.hpp"

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
	struct sockaddr_in	serverAdress;

	
}
