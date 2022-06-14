#include "ResponseHTTP.hpp"

// Public
ResponseHTTP::ResponseHTTP(Config config, RequestHTTP request) : _config(config), _request(request)
{
	initDirectives();
	initStatusCode();
}

ResponseHTTP::~ResponseHTTP() {};

void ResponseHTTP::GET(std::string path)
{	
	_directives["Date"] = getDate();

	if (!isAllowedMethod("GET"))
	{
		_statusCode = generateStatusCode(405);
		createStatusLine();
		createHeaders();
		return ;
	}

	/* Pour les CGI
	** if (_request.getFile() == "/CGI_folder")
	** 		my_cgi_function(); (Mets le résultat de l'éxecution du programme dans la variable _body)
	** else */

	generateBody(path);
	createStatusLine();
	createHeaders();
}

void ResponseHTTP::POST(std::string path)
{	
	_directives["Date"] = getDate();

	if (!isAllowedMethod("POST"))
	{
		_statusCode = generateStatusCode(405);
		createStatusLine();
		createHeaders();
		return ;
	}

	_statusCode = generateStatusCode(501);

	createStatusLine();
	createHeaders();
}

void ResponseHTTP::DELETE(std::string path)
{
	_directives["Date"] = getDate();

	if (!isAllowedMethod("DELETE"))
	{
		_statusCode = generateStatusCode(405);
		createStatusLine();
		createHeaders();
		return ;
	}

	deleteFile(path);	
	createStatusLine();
	createHeaders();
}


std::string ResponseHTTP::getResponseHTTP() { return (_statusLine + _headers + _body); }
std::string ResponseHTTP::getStatusCode() { return (_statusCode); }

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

void ResponseHTTP::initStatusCode()
{
	// 1xx (Informational)
	_statusCodes[100] = "Continue";
	_statusCodes[101] = "Switching Protols";

	// 2xx (Successful)
	_statusCodes[200] = "OK";
	_statusCodes[201] = "Created";
	_statusCodes[202] = "Accepted";
	_statusCodes[203] = "Non-Authorative Information";
	_statusCodes[204] = "No Content";
	_statusCodes[205] = "Reset Content";
	_statusCodes[206] = "Partial Content";

	// 3xx (Redirection)
	_statusCodes[301] = "Moved Permanently";
	_statusCodes[302] = "Found";
	_statusCodes[303] = "See Other";
	_statusCodes[304] = "Not Modified";
	_statusCodes[305] = "Use Proxy";
	_statusCodes[307] = "Temporary Redirect";

	// 4xx (Client Error)
	_statusCodes[400] = "Bad Request";
	_statusCodes[401] = "Unauthorized";
	_statusCodes[402] = "Bad Request";
	_statusCodes[403] = "Forbidden";
	_statusCodes[404] = "Not Found";
	_statusCodes[405] = "Method Not Allowed";
	_statusCodes[406] = "Not Acceptable";
	_statusCodes[407] = "Proxy Authentification Required";
	_statusCodes[408] = "Request Timeout";
	_statusCodes[409] = "Conflict";
	_statusCodes[410] = "Gone";
	_statusCodes[411] = "Length Required";
	_statusCodes[412] = "Precondition Failed";
	_statusCodes[413] = "Payload Too Large";
	_statusCodes[414] = "URI Too Long";
	_statusCodes[415] = "Unsupported Media Type";
	_statusCodes[416] = "Range Not Satisfiable";
	_statusCodes[417] = "Expectation Failed";
	_statusCodes[426] = "Upgrade Required";

	// 5xx (Server Error)
	_statusCodes[500] = "Internal Server Error";
	_statusCodes[501] = "Not Implemented";
	_statusCodes[502] = "Bad Gateway";
	_statusCodes[503] = "Service Unavailable";
	_statusCodes[504] = "Gateway Timeout";
	_statusCodes[505] = "HTTP Version Not Supported";
}

std::string ResponseHTTP::generateStatusCode(int statusCode)
{
	return (std::to_string(statusCode) + " " + _statusCodes[statusCode]);
}

void ResponseHTTP::createStatusLine()
{
	_statusLine = "HTTP/1.1 ";
	_statusLine += _statusCode;
	_statusLine += "\n";
}

void ResponseHTTP::createHeaders()
{
	_headers += "Date: "			+ _directives["Date"] + "\n";
	_headers += "Server: "			+ _directives["Server"] + "\n";

	if (_directives["Last-Modified"] != "")
		_headers += "Last-Modified: "	+ _directives["Last-Modified"] + "\n"; // Sans doute trop compliqué à implémenter
	if (_directives["Content-Length"] != "")
		_headers += "Content-Length: "	+ _directives["Content-Length"] + "\n";
	if (_directives["Content-Type"] != "")
		_headers += "Content-Type: "	+ _directives["Content-Type"] + "\n";

	_headers += "\n";
}

void ResponseHTTP::generateBody(std::string path)
{
	if (path == "/.html")
		path = "index.html";

	std::ifstream		requested_file("Configs/" + _config.get_root() + "/" + path);
	std::stringstream	buffer;

	std::string ext = path.substr(path.find_last_of('.') + 1);

	if (isAllowedContentType(ext))
		_directives["Content-Type"] = "text/" + ext;
	else
		_directives["Content-Type"] = "text/" + ext; // renvoie "html", devrait renvoyer "text/html"

	if (requested_file.is_open())
	{
		_statusCode = generateStatusCode(200);
		buffer << requested_file.rdbuf();
		requested_file.close();
		_body = buffer.str();
		_directives["Content-Length"] = std::to_string(_body.size());
	}
	else
	{
		_statusCode = generateStatusCode(404);
		_directives["Content-Type"] = "text/html";
		_body = "<!doctype html><html><head><title>404</title></head><body><p><strong>Error : </strong>404 Not Found.</p></body></html>";
		_directives["Content-Length"] = std::to_string(_body.size());
	}
}

void ResponseHTTP::deleteFile(std::string path)
{
	if (path == "/.html")
		path = "index.html";

	std::string	s		= "Configs/" + _config.get_root() + "/" + path;
	char *		c_str	= &s[0];

	if (std::remove(c_str) != 0)
	{
		if (errno == ENOENT)
		{
			_statusCode = generateStatusCode(204);
			return ;
		}
	}

	_statusCode = generateStatusCode(200);
	_body = "<html><body><h1>File deleted.</h1></body></html>";
	_directives["Content-Length"] = std::to_string(_body.size());
	_directives["Content-Type"] = "text/html";
}


std::string	ResponseHTTP::getDate(void)
{
	char			buffer[100];
	struct timeval	tv;
	struct tm *		tm;

	gettimeofday(&tv, NULL);
	tm = gmtime(&tv.tv_sec);
	strftime(buffer, 100, "%a, %d %b %Y %H:%M:%S GMT", tm);
	return (std::string(buffer));
}

bool ResponseHTTP::isAllowedMethod(std::string method)
{
	std::vector<std::string> configMethods	= _config.get_methods();

	if (configMethods.size() == 0)
		return (true);

	for (std::vector<std::string>::iterator it = configMethods.begin() ; it != configMethods.end() ; it++)
	{
		if (*it == method)
			return (true);
	}

	return (false);
}

bool ResponseHTTP::isAllowedContentType(std::string contentType)
{
	std::vector<std::string> requestAccept	= _request.getAccept();

	for (std::vector<std::string>::iterator it1 = requestAccept.begin() ; it1 != requestAccept.end() ; it1++)
	{
		{
			if (*it1 == contentType)
				return (true);
		}
	}

	return (false);
}
