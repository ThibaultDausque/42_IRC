#include "User.hpp"
#include "Server.hpp"
#include "Request.hpp"

int	executePass(User &user, std::string serv_pwd, Server &server)
{
	std::string	err_msg;
	std::vector<struct pollfd>	client_tab = server.getTab();
	if (serv_pwd.empty())
	{
		if (user.getPassword().size() <= 1 || user.getPassword() != serv_pwd)
		{
			err_msg = ERR_PASSWDMISMATCH(user.getNickname());
			send(user.getSocket(), err_msg.c_str(), err_msg.size(), 0);
			for (size_t i = 0; i < client_tab.size(); i++)
			{
				if (client_tab[i].fd == user.getSocket())
				{
					client_tab.erase(client_tab.begin() + i);
					break ;
				}
			}
			close(user.getSocket());
			return 1;
		}
	}
	return 0;
}