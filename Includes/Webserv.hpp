#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <sys/types.h>
# include <sys/socket.h>
# include "../Includes/Parser.hpp"
# include "../Includes/Config.hpp"

# define MAX_CLIENTS 10 // À changer de place

class Webserv
{
	public:
		typedef std::vector<Config>	configVector; // Temporaire

		Webserv();
		~Webserv();

		void	run();
	private:
		Parser			_parser;
		configVector 	_server;

		void	initServers();
		void	initSocket();
};


#endif