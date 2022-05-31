#ifndef RESPONSEHTTP_HPP
# define RESPONSEHTTP_HPP

# include "../../Includes/headers.hpp"

class ResponseHTTP
{
	public:
		ResponseHTTP();
		~ResponseHTTP();

		void		requestFile(std::string requested_filename);
		std::string	getResponseHTTP();
	private:
		std::string							_statusLine;
		std::string							_statusCode;
		std::string							_headers;
		std::string							_body;
		std::map<std::string, std::string>	_directives;

		void		createStatusLine();
		void		createHeaders();
		void		initDirectives();

		void 		openFile(std::string requested_file);
};

#endif