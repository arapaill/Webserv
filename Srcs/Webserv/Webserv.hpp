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
		Parser				_parser;
		std::vector<Config>	_serversConfig;
		std::vector<int>	_serversFD;
		int					_epollFD;
		struct epoll_event 	_ev, _events[MAX_EVENTS];


		void	init();
		void	launchServers();
		void 	closeServers();
		int		initSocket( Config serverConfig );
		void	handleRead( int clientFD );
		void	acceptNewClient( int serverFD );
		bool	isFDServer( int readyFD );

};


#endif