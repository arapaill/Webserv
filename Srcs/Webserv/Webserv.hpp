#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include "../../Includes/headers.hpp"

class Webserv
{
	public:
		Webserv();
		~Webserv();

		void	run();
		void	setParser( Parser & parser );

	private:
		Parser				_parser;
		configVector 		_serversConfig;
		serverFDVector		_serversFD;
		int					_epollfd;
		struct epoll_event 	_ev, _events[MAX_EVENTS];


		void	init();
		void	launchServers();
		void 	closeServers();
		int		initSocket( t_network network );
		void	handleRead( int clientFD );
		void	acceptNewClient( int serverFD );
		bool	isFDServer( int readyFD );

};


#endif