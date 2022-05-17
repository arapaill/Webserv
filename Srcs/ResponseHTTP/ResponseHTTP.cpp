#include "ResponseHTTP.hpp"

// Temp ?
static std::string	ft_itoa(int nb)
{
	return (static_cast<std::ostringstream*>( &(std::ostringstream() << nb) )->str());
}

// Public
ResponseHTTP::ResponseHTTP() {};

ResponseHTTP::~ResponseHTTP() {};

void ResponseHTTP::requestFile(std::string requested_filename)
{
	openFile(requested_filename);
	createHeader();
}

std::string ResponseHTTP::getResponseHTTP() { return (_header + _body); }

// Private
void ResponseHTTP::createHeader()
{
	_header = "HTTP/1.1 ";
	_header += "200 OK\n"; // STATUS CODE
	_header += "Content-Type: text/html\n"; // Content-Type
	_header += "Content-Length: "; // Content-Length
	_header += ft_itoa(_body.size());
	_header += "\n\n";
}

void ResponseHTTP::openFile(std::string requested_filename)
{
	std::ifstream requested_file("../HTML/" + requested_filename);
	std::ifstream error_file("../HTML/404.html");
	std::string r_file;
	std::string line;

	if (!requested_file.is_open())
	{
		while (getline(error_file, line))
			r_file += line + '\n';
		error_file.close();
	}
	else
	{
		while (getline(requested_file, line))
			r_file += line + '\n';
		requested_file.close();
	}

	_body = r_file;
}