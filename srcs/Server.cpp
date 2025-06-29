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


void	Server::runServer()
{
	int		clientSocket;
	char	buffer[1024] = {0};
	std::vector<pollfd>	tab;
	bool	run = false;
	int		bytes;

	// initialise client
	std::string nickname = "null";
	std::string	username = "null";
	std::string	hostname = "null";
	std::string realname = "null";

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
			User cli(nickname, username, hostname, realname, clientSocket);
			if (clientSocket == -1)
				throw(std::runtime_error("Error: client has not been accepted\n"));
			// std::cout << "client connected !" << std::endl;
			pollfd	client_pollfd;
			client_pollfd.fd = clientSocket;
			client_pollfd.events = POLLIN;
			client_pollfd.revents = 0;
			tab.push_back(client_pollfd);
			_clients.push_back(cli);
		}

		// std::string	join = ":tdausque!~@nerd-9AE5B52D.unyc.it JOIN #test\r\n";
		for (size_t i = 1; i < tab.size(); i++)
		{
			int	j = 0;
			if (tab[i].revents & POLLIN)
			{
				bytes = recv(clientSocket, buffer, sizeof(buffer), 0);
				nickname = parseNick(buffer);
				std::cout << buffer << std::endl;
				// if (!parseCmd(buffer))
					// break ;
				try
	   			{
					_clients[j].setNickname(nickname);
					std::cout << _clients[j].getNickname() << std::endl;
	   			}
				catch (const std::length_error &e)
	   			{
					std::cerr << e.what() << std::endl;
	   			}
	   			std::string	welcome = "Hello World!\r\n";
				memset(buffer, 0, sizeof(buffer));
				// send(clientSocket, welcome.c_str(), welcome.size(), 0);
				j++;
			}
		}
	}
	close(this->_serverFd);
}

