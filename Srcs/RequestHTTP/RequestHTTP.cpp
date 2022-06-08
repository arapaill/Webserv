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

std::vector<std::string>	RequestHTTP::getAccept() { return (_accept); };
std::string					RequestHTTP::getHost() { return (_host); };

// Private
void RequestHTTP::parseKeyword(std::string line)
{
	if (line.find("Accept:") != std::string::npos)
			_accept = split(split(line, ' ')[1], ',');
	if (line.find("Host:") != std::string::npos)
			_host = split(line, ' ')[1];
}

// GET / HTTP1.1
// HOST: 127.0.0.1:5000
// Accept: fsef,fesfes,