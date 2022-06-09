#ifndef RESPONSEHTTP_HPP
# define RESPONSEHTTP_HPP

# include "../../Includes/headers.hpp"
# include "../RequestHTTP/RequestHTTP.hpp"

class ResponseHTTP
{
	public:
		ResponseHTTP( Config config, RequestHTTP request );
		~ResponseHTTP();

		void		GET(std::string filename);
		void		POST(std::string filename);

		// Getter
		std::string	getResponseHTTP();
		std::string getStatusCode();

	private:
		Config								_config;
		RequestHTTP							_request;
		std::string							_statusLine;
		std::string							_statusCode;
		std::string							_headers;
		std::string							_body;
		std::map<std::string, std::string>	_directives;
		std::map<int, std::string>			_statusCodes;

		void		createStatusLine();
		void		createHeaders();

		// Inits
		void		initDirectives();
		void		initStatusCode();
		
		// Utils
		void 		generateBody(std::string filename);
		std::string generateStatusCode(int statusCode);
		std::string	getDate(void);
};

#endif