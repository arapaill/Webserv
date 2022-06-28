#ifndef RESPONSEHTTP_HPP
# define RESPONSEHTTP_HPP

# include "../../Includes/headers.hpp"
# include "../RequestHTTP/RequestHTTP.hpp"
# include "../CGIhandler/CGIhandler.hpp"

class ResponseHTTP
{
	public:
		ResponseHTTP( Config config, RequestHTTP request );
		~ResponseHTTP();

		void		GET( std::string path );
		void		POST( std::string path );
		void		DELETE( std::string path );
		void 		UNKNOWN( std::string path );


		// Getter
		std::string	getResponseHTTP();
		std::string getStatusCode();
		std::string getBodySize();

	private:
		Config								_config;
		RequestHTTP							_request;
		std::string							_statusLine;
		std::string							_statusCode;
		std::string							_headers;
		std::string							_body;
		std::map<std::string, std::string>	_directives;
		std::map<int, std::string>			_statusCodes;

		bool								_getLocationFirst;
		bool								_isCgi();

		void		createStatusLine();
		void		createHeaders();

		// Inits
		void		initDirectives();
		void		initStatusCode();
		
		// Utils
		void 		generateBody( std::string filename );
		std::string	generateStatusCode( int statusCode );
		void		generateAutoindex( std::string path );
		bool 		checkConfig(std::string path, std::string method);
		std::string	getDate( void );
		bool		isAllowedContentType( std::string contentType );
		bool		isAllowedMethod( std::string method, std::string path );
		void 		deleteFile( std::string path );
		bool		getLocation( std::string path, Config & locationConfig );
		bool		checkReturn( std::string path );
};

#endif