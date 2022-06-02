#include "ResponseHTTP.hpp"

// Temp ?
static std::string	ft_itoa(int nb)
{
	return (static_cast<std::ostringstream*>( &(std::ostringstream() << nb) )->str());
}

// Public
ResponseHTTP::ResponseHTTP()
{
	initDirectives();
}

ResponseHTTP::ResponseHTTP(Config config) : _config(config)
{
	initDirectives();
}

ResponseHTTP::~ResponseHTTP() {};

void ResponseHTTP::requestFile(std::string requested_filename)
{
	openFile(requested_filename);
	createStatusLine();
	createHeaders();
}

std::string ResponseHTTP::getResponseHTTP() { return (_statusLine + _headers + _body); }

// Private
void ResponseHTTP::initDirectives()
{
	_directives["Allow"] = "";
	_directives["Content-Language"] = "";
	_directives["Content-Length"] = "";
	_directives["Content-Location"] = "";
	_directives["Content-Type"] = "";
	_directives["Date"] = "";
	_directives["Last-Modified"] = "";
	_directives["Connection"] = "";
	_directives["Location"] = "";
	_directives["Retry-After"] = "";
	_directives["Server"] = "Webserv";
	_directives["Transfer-Encoding"] = "identity";
	_directives["WwwAuthenticate"] = "";
	_directives["Set-Cookie"] = "";
}

void ResponseHTTP::createStatusLine()
{
	_statusLine = "HTTP/1.1 "; // HTTP Version
	_statusLine += _statusCode; // Status Code
	_statusLine += "\n"; // Fin de Status Line
}

void ResponseHTTP::createHeaders()
{
	_headers += "Content-Type: " + _directives["Content-Type"] + "\n";
	_headers += "Content-Length: " + _directives["Content-Length"] + "\n";
	_headers += "\n";
}

void ResponseHTTP::openFile(std::string requested_filename)
{
	std::ifstream		requested_file(_config.get_root() + "/" + requested_filename);
	std::stringstream	buffer;

	if (!requested_file.is_open())
	{
		_statusCode = "404 Not Found";
		_directives["Content-Type"] = "text/html";
		_body = "<!doctype html><html><head><title>404</title></head><body><p><strong>Error : </strong>404 Not Found.</p></body></html>";
		_directives["Content-Length"] = ft_itoa(_body.size());

	}
	else
	{
		_statusCode = "200 OK";
		buffer << requested_file.rdbuf();
		requested_file.close();
		_body = buffer.str();
		_directives["Content-Type"] = "text/html";
		_directives["Content-Length"] = ft_itoa(_body.size());
	}
}