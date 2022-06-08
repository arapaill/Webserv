#ifndef RESPONSEHTTP_HPP
# define RESPONSEHTTP_HPP

# include "../../Includes/headers.hpp"
# include "../RequestHTTP/RequestHTTP.hpp"

class ResponseHTTP
{
	public:
		ResponseHTTP( Config config, RequestHTTP request );
		~ResponseHTTP();

		void		GET(std::string requested_filename);
		std::string	getResponseHTTP();
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
		void		initDirectives();
		void		initStatusCode();
		std::string generateStatusCode(int statusCode);

		void 		openFile(std::string requested_file);
		
		// Utils
		std::string	getDate(void);
};

#endif