#include "Server.hpp"
#include <string>

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
	
	int		yes = 1;
	setsockopt(this->_serverFd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
	// link a socket with an IP and a port
	if (bind(_serverFd, (sockaddr *)&serverAddress, sizeof(serverAddress)))
		throw(std::runtime_error("Error: bind failed\n"));
	// The Server listen on 5 port max
	if (listen(_serverFd, 5))
		throw(std::runtime_error("Error: listen failed\n"));

	return 0;
}

void	Server::runServer()
{
	int		clientSocket;
	char	buffer[1024] = {0};
	std::vector<pollfd>	tab;
	bool	run = false;
	int		bytes;

	// initialise client
	std::string nickname = "toto";
	std::string	username = "titi";
	std::string	hostname = "tata";
	std::string realname = "tutu";

	pollfd	server_pollfd;
	server_pollfd.fd = this->_serverFd;
	server_pollfd.events = POLLIN;
	server_pollfd.revents = 0;
	tab.push_back(server_pollfd);

	std::cout << "\e[1;36m╔───────────────────────────────────────────────╗" << std::endl;
	std::cout << "\e[1;36m│   ███████╗████████╗     ██╗██████╗  ██████╗   │" << std::endl;
	std::cout << "\e[1;36m│   ██╔════╝╚══██╔══╝     ██║██╔══██╗██╔════╝   │" << std::endl;
	std::cout << "\e[1;36m│   █████╗     ██║        ██║██████╔╝██║        │" << std::endl;
	std::cout << "\e[1;36m│   ██╔══╝     ██║        ██║██╔══██╗██║        │" << std::endl;
	std::cout << "\e[1;36m│   ██║        ██║███████╗██║██║  ██║╚██████╗   │" << std::endl;
	std::cout << "\e[1;36m│   ╚═╝        ╚═╝╚══════╝╚═╝╚═╝  ╚═╝ ╚═════╝   │" << std::endl;
	std::cout << "\e[1;36m╚───────────────────────────────────────────────╝" << std::endl;
	std::cout << "\e[1;32m🚀 Server started successfully" << std::endl << std::endl;
	
	while (run == false)
	{
		if (poll(tab.data(), tab.size(), -1) == -1)
			throw(std::runtime_error("Error: poll failed\n"));
		if (tab[0].revents & POLLIN)
		{
			clientSocket = accept(this->_serverFd, NULL, NULL);
			Client cli(nickname, username, hostname, realname, clientSocket);
			if (clientSocket == -1)
				throw(std::runtime_error("Error: client has not been accepted\n"));
			std::cout << "client connected !" << std::endl;
			pollfd	client_pollfd;
			client_pollfd.fd = clientSocket;
			client_pollfd.events = POLLIN;
			client_pollfd.revents = 0;
			tab.push_back(client_pollfd);
			_clients.push_back(cli);
		}
		for (size_t i = 1; i < tab.size(); i++)
		{
			if (tab[i].revents & POLLIN)
			{
				bytes = recv(clientSocket, buffer, sizeof(buffer), 0);
				std::cout << "Message received !" << std::endl;
				std::cout << "Client Message: " << buffer << std::endl;
				std::string	welcome = ":tdausque!~@nerd-9AE5B52D.unyc.it JOIN #test\r\n";
	   			send(clientSocket, welcome.c_str(), welcome.size(), 0);
			}
		}
	}
	close(this->_serverFd);
}

