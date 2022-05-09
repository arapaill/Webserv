#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <sys/types.h>
# include <sys/socket.h>
# include <cstring>
# include <iostream> // Temporaire

# include "Parser.hpp"
# include "Config.hpp"
# include "t_network.hpp"

#include <errno.h>

#include <string>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fstream>


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