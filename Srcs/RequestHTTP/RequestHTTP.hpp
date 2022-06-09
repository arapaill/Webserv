#ifndef REQUESTHTTP_HPP
# define REQUESTHTTP_HPP

# include "../../Includes/headers.hpp"

class RequestHTTP
{
	public:
		RequestHTTP( std::string request );
		~RequestHTTP();

		std::string					getMethod();
		std::vector<std::string>	getAccept();
		std::string					getHost();

	private:
		std::string					_method;
		std::vector<std::string>	_accept;
		//std::vector<std::string	_accept-language;
		std::string					_host;

		void parseKeyword( std::string line );
		std::string str_toupper( std::string s );
};

#endif