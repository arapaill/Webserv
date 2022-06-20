#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include "../../Includes/headers.hpp"
# include "../RequestHTTP/RequestHTTP.hpp"
# include "../ResponseHTTP/ResponseHTTP.hpp"

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
		std::vector<int>		_serversFD, _clientsFD;
		fd_set					_currentSockets, _readySockets;
		int						_maxSocket;

		void	init();
		void	launchServers();
		void 	closeServers();
		int		initSocket( Config serverConfig );
		void	handleRead( int clientFD );
		int		acceptNewClient( int serverFD );
		bool	isServer( int readyFD );
		void	closeSockets();

		// Utils
		Config &	getServerConfig( std::string host );
		std::string getTime();


};


#endif