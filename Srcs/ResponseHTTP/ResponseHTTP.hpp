#ifndef RESPONSEHTTP_HPP
# define RESPONSEHTTP_HPP

# include <string>

# include <sys/types.h>
# include <sys/socket.h>
# include <cstring>
# include <iostream> // Temporaire
# include <string>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <fstream>
# include <errno.h>
# include <sstream>

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