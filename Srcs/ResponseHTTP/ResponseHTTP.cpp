#include "ResponseHTTP.hpp"

// Public


ResponseHTTP::ResponseHTTP(Config config, RequestHTTP request) : _config(config), _request(request)
{
	_getLocationFirst = true;
	initDirectives();
	initStatusCode();
}

ResponseHTTP::~ResponseHTTP() {};

std::string ResponseHTTP::getBodySize() { return (std::to_string(_body.size())); }

void ResponseHTTP::GET(std::string path)
{	
	_directives["Date"] = getDate();

	if (checkConfig(path, "GET"))
		return ;

	struct stat buf;
	std::string tmp = _config.get_root() + '/' + path;
	stat(tmp.c_str(), &buf);
	
	if (_config.get_autoindex() && S_ISDIR(buf.st_mode))
	{
		generateAutoindex(path);
		_statusCode = generateStatusCode(200);
		createStatusLine();
		createHeaders();
		return;
	}

	if (_request.getFile().substr(0, 8) == "/cgi-bin")
	{
		CGIhandler cgi(_request, _config, path);
		cgi.init_env();
		cgi.execute_CGI();
		this->_body = cgi.get_body();
		_statusCode = generateStatusCode(cgi.get_status_code());
		_directives["Content-Type"] = "text/html";
		_directives["Content-Length"] = std::to_string(_body.size());
	}
	/* Pour les CGI
	** if (_request.getFile() == "/CGI_folder")
	** 		my_cgi_function(); (Mets le résultat de l'éxecution du programme dans la variable _body)
	** else */
	else
		generateBody(path);
	createStatusLine();
	createHeaders();
}

void ResponseHTTP::POST(std::string path)
{	
	std::ifstream check_file;
	std::ofstream file;

	_directives["Date"] = getDate();

	if (checkConfig(path, "POST"))
		return ;

	_statusCode = generateStatusCode(204);

	//IF CGI
	//ELSE ...
	//Check si le fichier existe change le statut
	check_file.open(_config.get_root() + path);
	if (check_file && _request.getBody().size() > 0)
		_statusCode = generateStatusCode(200);
	else if (_request.getBody().size() > 0)
		_statusCode = generateStatusCode(201);
	check_file.close();

	//open et rajoute le body au fichier
	file.open(_config.get_root() + path, std::ios_base::app);
	file << _request.getBody();
	file.close();
	createStatusLine();
	createHeaders();
}

void ResponseHTTP::DELETE(std::string path)
{
	_directives["Date"] = getDate();

	if (checkConfig(path, "DELETE"))
		return ;

	deleteFile(path);	
	createStatusLine();
	createHeaders();
}

void ResponseHTTP::UNKNOWN(std::string path)
{
	_directives["Date"] = getDate();
	_statusCode = generateStatusCode(405);

	createStatusLine();
	createHeaders();
}

bool ResponseHTTP::checkConfig(std::string path, std::string method)
{
	if (!isAllowedMethod(method, _config.get_root() + path))
	{
		_statusCode = generateStatusCode(405);
		createStatusLine();
		createHeaders();
		return (true);
	}

	if (checkReturn(_config.get_root() + path))
		return (true);

	if (_config.get_client_max_body_size() != 0 && _config.get_client_max_body_size() < _request.getBody().size())
	{
		_statusCode = generateStatusCode(413);
		createStatusLine();
		createHeaders();
		return (true);
	}

	return (false);
}


std::string ResponseHTTP::getResponseHTTP() { return (_statusLine + _headers + _body); }
std::string ResponseHTTP::getStatusCode() { return (_statusCode); }

// Private
void ResponseHTTP::initDirectives()
{
	_directives["Allow"] = "";
	_directives["Content-Language"] = "";
	_directives["Content-Length"] = "0\n";
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
	_headers += "Date: " 			+ _directives["Date"] + "\n";
	_headers += "Server: "			+ _directives["Server"] + "\n";
	_headers += "Content-Length: "	+ _directives["Content-Length"] + "\n";

	if (_directives["Last-Modified"] != "")
		_headers += "Last-Modified: "	+ _directives["Last-Modified"] + "\n"; // Sans doute trop compliqué à implémenter
	if (_directives["Content-Type"] != "")
		_headers += "Content-Type: "	+ _directives["Content-Type"] + "\n";
	if (_directives["Location"] != "")
		_headers += "Content-Type: "	+ _directives["Location"] + "\n";

	_headers += "\n";
}

void ResponseHTTP::generateAutoindex(std::string path)
{
	std::string dirPath = _config.get_root() + path;
	DIR * dir 			= opendir(dirPath.c_str());
	std::string	index	= "<!DOCTYPE html><html><head><title>" + dirPath + "</title></head><body><h1>Index of " + path + "</h1><hr>";

	//std::cout << "Current working directory: " << dirPath << std::endl;

	if (dir == NULL)
	{
		std::cout << RED << "GenerateAutoindex(): Could not open \"" << dirPath << "\"\n"  << RESET;
		exit(EXIT_FAILURE); // Pas bon
	}

	for (struct dirent *dirEntry = readdir(dir); dirEntry; dirEntry = readdir(dir))
	{
		std::stringstream ss;
		if (path != "/")
			ss << "<li><a href=\"" << path << "/" << dirEntry->d_name << "\">" << dirEntry->d_name << "</a></li>\n";
		else
			ss << "<li><a href=\"" << dirEntry->d_name << "\">" << dirEntry->d_name << "</a></li>\n";
		index += ss.str();
		ss.clear();
	}
	index += "<hr></body></html>";
	closedir(dir);
	_body = index;
	_directives["Content-Length"] = std::to_string(_body.size());
}

void ResponseHTTP::generateBody(std::string path)
{
	if (path == "/")
		path = "/" + _config.get_index();

	std::ifstream		requested_file(_config.get_root() + path);
	std::stringstream	buffer;

	// std::cout << "(" << _config.get_root() << ")" << path << std::endl;
/* 	std::string ext = path.substr(path.find_last_of('.') + 1);  // A REFAIRE

	if (isAllowedContentType(ext))
		_directives["Content-Type"] = "text/" + ext; // renvoie "html", devrait renvoyer "text/html"
	else if (isAllowedContentType("plain/text"))
		_directives["Content-Type"] = "plain/text"; */


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

	std::string	s		= _config.get_root() + "/" + path;
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

bool ResponseHTTP::isAllowedMethod(std::string method, std::string path)
{
	std::vector<std::string>	configMethods = _config.get_methods();
	std::vector<std::string>	locationMethods;
	Config						locationConfig;

	while (getLocation(path, locationConfig) && locationConfig.get_methods().size() > 0)
		locationMethods = locationConfig.get_methods();

	if (locationMethods.size() > 0)
		configMethods = locationMethods;

	if (configMethods.size() == 0)
		return (true);

	for (std::vector<std::string>::iterator it = configMethods.begin() ; it != configMethods.end() ; it++) {
		if (*it == method)
			return (true);
	}

	return (false);
}

bool ResponseHTTP::getLocation(std::string path, Config & locationConfig)
{
	// path = HTML/TEST/index.html

	std::map<std::string, Config>	location;

	if (_getLocationFirst)
	{
		location = _config.get_location();
		_getLocationFirst = false;
	}
	else
	{
		//std::cout << "Current server name1: " << locationConfig.get_server_name() << "\n";
		location = locationConfig.get_location();
		//std::cout << "Location size: " << location.size() << "\n";
	}

	for (std::map<std::string, Config>::iterator it = location.begin() ; it != location.end() ; it++)
	{
		//std::cout << "Current location: " << it->second.get_server_name() << "\n";

		if (path.find(it->first) != std::string::npos)
		{
			locationConfig = it->second;
			//std::cout << "TRUE\n";
			//std::cout << "Current server name2: " << locationConfig.get_server_name() << "\n";
			return (true);
		}
	}

	//std::cout << "FALSE\n";
	return (false);
}

// On vérifie que le contentType qu'on veut renvoyer au client est bien dans la liste des accept qu'il nous a envoyé.
bool ResponseHTTP::isAllowedContentType(std::string contentType)
{
	std::vector<std::string> requestAccept = _request.getAccept();

	for (std::vector<std::string>::iterator it = requestAccept.begin() ; it != requestAccept.end() ; it++)
	{
		if (*it == contentType)
				return (true);
	}

	return (false);
}

// Vérifie si il y a un éventuel return dans le .conf ou ses location
bool ResponseHTTP::checkReturn(std::string path)
{
	Config locationConfig;

	while (getLocation(_config.get_root() + path, locationConfig))
	{
		if (locationConfig.get_return().size() == 1)
		{
			_statusCode = generateStatusCode(locationConfig.get_return().begin()->first);
			_directives["Location"] = locationConfig.get_return().begin()->second;

			createStatusLine();
			createHeaders();
			return (true);
		}
	}
	return (false);
}
