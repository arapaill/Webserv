#include "RequestHTTP.hpp"

// Public
RequestHTTP::RequestHTTP( std::string request ) 
{
	std::vector<std::string> headers;

	headers = split(request, '\n');

	for (std::vector<std::string>::iterator it = headers.begin() ; it != headers.end() ; it++)
	{
		parseKeyword(*it);
	}
}

RequestHTTP::~RequestHTTP() {};

std::string					RequestHTTP::getFile()		{ return (_file);	};
std::string					RequestHTTP::getBody()		{ return (_body);	};
std::string					RequestHTTP::getMethod()	{ return (_method);	};
std::vector<std::string>	RequestHTTP::getAccept()	{ return (_accept);	};
std::string					RequestHTTP::getHost()		{ return (_host);	};

// Private
void RequestHTTP::parseKeyword(std::string line)
{
	if (str_toupper(line).find("GET") != std::string::npos 
		|| str_toupper(line).find("POST") != std::string::npos
		|| str_toupper(line).find("DELETE") != std::string::npos)
		{
			std::vector<std::string> tmp = split(line, ' ');
			_method = tmp[0];
			_file = tmp[1];
		}
	else if (line.find("Accept:") != std::string::npos)
			_accept = split(split(line, ' ')[1], ',');
	else if (line.find("Host:") != std::string::npos)	
		_host = split(line, ' ')[1];
}

std::string RequestHTTP::str_toupper( std::string s )
{
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::toupper(c); });
    return (s);
}

// GET / HTTP1.1
// HOST: 127.0.0.1:5000
// Accept: fsef,fesfes,