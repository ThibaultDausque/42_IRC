#include "Server.hpp"
#include <cerrno>

Server::Server(std::string _pwd, unsigned int _port)
{
	this->_serverFd = 0;
	this->_serverIp = "";
	this->_serverPwd = _pwd;
	this->_port = _port;
	this->_connected = 0;
	this->_clients.reserve(MAXCLI);
}

Server::Server(const Server &copy)
{
	this->_serverFd = copy._serverFd;
	this->_serverIp = copy._serverIp;
	this->_serverPwd = copy._serverPwd;
	this->_port = copy._port;
	this->_connected = copy._connected;
	this->_clients = copy._clients;
	this->_tab = copy._tab;
	this->_channels = copy._channels;
}

Server	&Server::operator=(const Server &copy)
{
	if (this != &copy) {
		this->_serverFd = copy._serverFd;
		this->_serverIp = copy._serverIp;
		this->_serverPwd = copy._serverPwd;
		this->_port = copy._port;
		this->_connected = copy._connected;
		this->_clients = copy._clients;
		this->_tab = copy._tab;
		this->_channels = copy._channels;
	}
	return (*this);
}

Server::~Server()
{
	for (size_t i = 0; i < _tab.size(); i++)
		close(_tab[i].fd);
	close(this->_serverFd);
}

std::vector<struct pollfd>&	Server::getTab()
{
	return this->_tab;
}

pollfd&	Server::getTabElement(size_t idx)
{
	return this->_tab[idx];
}

int	Server::initServer(void)
{
	struct sockaddr_in	serverAddress;

	// AF_INET = specifies the IPv4 protocol family
	// SOCK_STREAM = defines that the TCP type socket
	this->_serverFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverFd == -1)
		throw std::runtime_error("Error: server socket.");
	// Structure for handling internet addresses
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(this->_port);
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	
	int		yes = 1;
	setsockopt(this->_serverFd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
	// link a socket with an IP and a port
	if (bind(_serverFd, (sockaddr *)&serverAddress, sizeof(serverAddress)))
	{
		close(_serverFd);
		throw(std::runtime_error("Error: Port is already in use\n"));
	}
	// The Server is set on listen mode
	if (listen(_serverFd, SOMAXCONN))
		throw(std::runtime_error("Error: listen failed\n"));
	return 0;
}

void	Server::acceptNewClient()
{
	struct sockaddr_in	cli;
	struct pollfd		cli_fd;
	int					accept_cli;
	socklen_t			len = sizeof(cli);

	if (_clients.size() == MAXCLI) {
		std::cout << "Couldn't accept more client(s) (1020 maximum)" << std::endl;
		return ;
	}

	accept_cli = accept(this->_serverFd, (sockaddr *)&(cli_fd), &len);
	if (accept_cli <= 0)
		throw std::runtime_error("Error: client accept failed.");
	else
		std::cout << "* New client *" << " " << accept_cli  << std::endl;

	if (fcntl(accept_cli, F_SETFL, O_NONBLOCK))
		throw std::runtime_error("Error: fcntl() failed.");

	cli_fd.fd = accept_cli;
	cli_fd.events = POLLIN;
	cli_fd.revents = 0;
	this->_tab.push_back(cli_fd);
	User user(accept_cli);
	this->_clients.push_back(user);
}

std::string	Server::readMessage(int fd_client)
{
	char	buff[1024];
	int		bytes;

	memset(buff, 0, sizeof(buff));
	bytes = recv(fd_client, buff, sizeof(buff) - 1, 0);

	if (bytes <= 0)
	{
		User	&user = getUserRfr(_clients, fd_client);
		executeQuit(user, _channels, "QUIT", _clients, _tab);
		std::cout << "* client " << fd_client << " disconnected *" << std::endl;
	}
	else
	{
		buff[bytes] = '\0';
		std::string	std_buff(buff);
		std::cout << fd_client << ": " << std_buff << std::endl;
		return std_buff;
	}
	return ("");
}

int	Server::runCommands(std::string cmdline, int tabIndex)
{
	size_t			index;
	std::string		tmpNick;
	std::string		cmdToRun;
	User			&user = getUserRfr(_clients, tabIndex);
	int				fd = user.getSocket();

	user.addTextToSend(cmdline);
	index = doesTextHaveCRLF(user.getTextToSend());
	while (index != std::string::npos) {

		if (user.getNickname() == "")
			tmpNick = "*";
		else
			tmpNick = user.getNickname();
		std::string errMsg = ERR_NOTREGISTERED(tmpNick);

		cmdToRun = user.getTextToSend().substr(0, index);
		if (!cmdToRun.empty()) {
			std::string cmd = cmdToRun.substr(0, cmdToRun.find(" "));

			if (cmd == "PASS")
				executePass(user, cmdToRun, this->_serverPwd);
			else if (!user.isPwdEntered())
				send(fd, errMsg.c_str(), errMsg.size(), 0);
			else if (cmd == "USER")
				executeUser(user, cmdToRun);
			else if (cmd == "NICK")
				executeNick(user, _channels, cmdToRun, _clients);
			else if (!user.isNicknameRegistered() || !user.isUsernameRegistered())
				send(fd, errMsg.c_str(), errMsg.size(), 0);
			else if (cmd == "JOIN")
				executeJoin(user, _channels, cmdToRun);
			else if (cmd == "NAMES")
				executeNames(user, _channels, cmdToRun, &_clients);
			else if (cmd == "KICK")
				executeKick(user, _channels, cmdToRun);
			else if (cmd == "PRIVMSG")
				executePrivmsg(user, _channels, cmdToRun, _clients);
			else if (cmd == "INVITE")
				executeInvite(user, _channels, cmdToRun, _clients);
			else if (cmd == "PART")
				executePart(user, _channels, cmdToRun);
			else if (cmd == "TOPIC")
				executeTopic(user, _channels, cmdToRun);
			else if (cmd == "PING")
				executePing(user, cmdToRun);
			else if (cmd == "WHO")
				executeWho(user, _channels, cmdToRun, _clients);
			else if (cmd == "MODE")
				executeMode(user, _channels, cmdToRun, _clients);
			else if (cmd == "QUIT")
				executeQuit(user, _channels, cmdToRun, _clients, _tab);
			else {
				errMsg =  ERR_UNKNOWNCOMMAND(user.getNickname(), cmd);
				send(fd, errMsg.c_str(), errMsg.size(), 0);
			}
		user.removeTextToSend(index + 2);
		index = doesTextHaveCRLF(user.getTextToSend());
		}
	}
	return (0);
}

void	Server::runServer()
{
	bool	run = false;
	pollfd	server_pollfd;
	server_pollfd.fd = this->_serverFd;
	server_pollfd.events = POLLIN;
	server_pollfd.revents = 0;
	this->_tab.push_back(server_pollfd);

	std::string	cmdline;

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
		if (poll(&this->_tab[0], this->_tab.size(), -1) == -1)
		{
			if (errno == EINTR)
			{
				for (size_t i = 0; i < _tab.size(); i++)
					close(_tab[i].fd);
				break ;
			}
			throw std::runtime_error("Error: poll() failed.");
		}
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
					cmdline = readMessage(this->_tab[i].fd);
					if (!cmdline.empty())
						runCommands(cmdline, this->_tab[i].fd);
				}
			}
		}
	}
}
