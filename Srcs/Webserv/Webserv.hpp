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

		void		run();
		void		setConfig( const std::vector<Config> & configs );

	private:
		void		launchServers();
		void 		closeServers();
		int			initServerSocket( Config serverConfig );
		int			handleRead( int clientSocket, RequestHTTP & parsedRequest );
		void 		sendResponse(int clientSocket, RequestHTTP & parsedRequest );
		int			acceptNewClient( int serverSocket );

		// Utils
		Config &	getServerConfig( std::string host );
		std::string getTime() const;
		int 		checkEnd( const std::string & str, const std::string & end ) const;

	private:
		Parser						_parser;
		std::vector<Config>			_serversConfig;
		std::vector<int>			_serversSocket;
		fd_set						_connectionSockets, readSockets;
		int							_maxSocket;
		std::map<int, RequestHTTP>	_clients;

};


#endif