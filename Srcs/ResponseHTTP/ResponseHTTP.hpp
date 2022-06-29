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

		std::string	getResponseHTTP();
		std::string getStatusCode();
		std::string getBodySize();

	private:
		void		initDirectives();
		void		initStatusCode();
		void		createStatusLine();
		void		createHeaders();
		void 		generateBody( std::string filename );
		std::string	generateStatusCode( int statusCode );
		void		generateAutoindex( std::string path );
		void 		deleteFile( std::string path );
		bool 		checkConfigRules( std::string path, std::string method );
		bool		isThereReturn( std::string path );
		bool		isAllowedContentType( std::string contentType );
		bool		isAllowedMethod( std::string method, std::string path );
		std::string	getDate();
		bool		getLocation( std::string path, Config & locationConfig );
		bool		checkReturn( std::string path );
        bool	    CheckAutoIndex(std::vector<std::string > path, std::map<std::string, Config> location, size_t i);
		bool		_isCgi();

	private:
		Config								_config;
		RequestHTTP							_request;
		std::map<std::string, std::string>	_directives;
		std::map<int, std::string>			_statusCodes;
		std::string							_statusLine;
		std::string							_headers;
		std::string							_body;
		std::string							_statusCode;
		bool								_getLocationFirst;
};

#endif