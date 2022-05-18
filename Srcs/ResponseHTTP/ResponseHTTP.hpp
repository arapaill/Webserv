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
		std::string	_header;
		std::string _body;
		char *		_response;

		void		createHeader();
		void 		openFile(std::string requested_file);
};

#endif