#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <sys/types.h>
# include <sys/socket.h>
# include <cstring>
# include <iostream> // Temporaire
# include <string>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <fstream>
# include <errno.h>

# include "../../Includes/Parser.hpp"
# include "../../Includes/Config.hpp"
# include "../../Includes/t_network.hpp"
# include "../ResponseHTTP/ResponseHTTP.hpp"



# define MAX_CLIENTS 10 // À changer de place

class Webserv
{
	public:
		typedef std::vector<Config>	configVector; // Temporaire
		typedef std::vector<int>	serverFDVector; // Temporaire


		Webserv();
		~Webserv();

		void	run();
		void	setParser( Parser & parser );
	private:
		Parser			_parser;
		configVector 	_serversConfig;
		serverFDVector	_serversFD;

		void	init();
		void	initServers();
		int		initSocket( t_network network );
		void	AcceptNewClient( int server );
};


#endif