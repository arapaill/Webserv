#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include "../../Includes/headers.hpp"

class Webserv
{
	public:
		Webserv();
		~Webserv();

		void	run();
		void	setConfig( std::vector<Config> & configs );

	private:
		Parser					_parser;
		std::vector<Config>		_serversConfig;
		std::vector<int>		_serversFD;
		fd_set					_currentSockets, _readySockets;   

		// Inutilisé pour le moment
		std::vector<int>		_clientsFD;
		std::map<int, Config>	_servers;

		void	init();
		void	launchServers();
		void 	closeServers();
		int		initSocket( Config serverConfig );
		void	handleRead( int clientFD );
		int		acceptNewClient( int serverFD );
		bool	isServer( int readyFD );
		void	closeSockets();


};


#endif