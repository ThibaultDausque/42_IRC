#include "Server.hpp"

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

std::string	Server::parseNick(const char *buff)
{
	std::string	message;
	std::string	nickname;
	char	nick[5] = "NICK";
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (buff[i])
	{
		if (buff[i] == nick[j])
			j++;
		else
			j = 0;
		if (nick[j] == '\0')
			break ;
		i++;
	}
	i++;
	while (buff[i] == ' ')
		i++;
	while (buff[i] && buff[i] != '\n' && buff[i] != '\0')
		nickname.push_back(buff[i++]);
	return nickname;
}
//
// int	Server::parseCmd(char *buff, User user)
// {
// 	if (cmd == "QUIT")
// 	{
// 		// erase the user
// 		return 0;
// 	}
// 	else if (isCmdValid(cmd) && cmd == "JOIN")
// 		executeJoin(user, channels, line);
// 	else if (isCmdValid(cmd) && cmd == "NAMES")
// 		executeNames(user, channels, line, &users);
// 	return 1;
// }

void	Server::acceptNewClient()
{
	struct sockaddr_in	cli;
	struct pollfd	cli_fd;
	int		accept_cli;
	socklen_t	len = sizeof(cli);

	accept_cli = accept(this->_serverFd, (sockaddr *)&(cli_fd), &len);
	if (accept_cli <= 0)
		throw std::runtime_error("Error: client accept failed.");
	else
		std::cout << "* New client *" << std::endl;

	if (fcntl(accept_cli, F_SETFL, O_NONBLOCK))
		throw std::runtime_error("Error: fcntl() failed.");

	cli_fd.fd = accept_cli;
	cli_fd.events = POLLIN;
	cli_fd.revents = 0;
	this->_tab.push_back(cli_fd);
}

void	Server::readMessage(int fd_client)
{
	char	buff[1024];
	int		bytes;

	memset(buff, 0, sizeof(buff));
	bytes = recv(fd_client, buff, sizeof(buff) - 1, 0);
	buff[bytes] = '\0';
	std::cout << buff << std::endl;
}

void	Server::runServer()
{
	bool	run = false;

	// initialise client
	std::string nickname = "null";
	std::string	username = "null";
	std::string	hostname = "null";
	std::string realname = "null";

	pollfd	server_pollfd;
	server_pollfd.fd = this->_serverFd;
	server_pollfd.events = POLLIN;
	server_pollfd.revents = 0;
	this->_tab.push_back(server_pollfd);

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
		for (size_t i = 0; i < this->_tab.size(); i++)
		{
			if (this->_tab[i].revents & POLLIN)
			{
				if (this->_tab[i].fd == this->_serverFd)
				{
					// accept client connection
					acceptNewClient();
				}
				else
				{
					// receive client message
					readMessage(this->_tab[i].fd);
				}
			}
		}
	}
	close(this->_serverFd);
}

