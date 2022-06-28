#include "Webserv.hpp"

// Public
volatile bool g_keepRunning = true;

Webserv::Webserv() {};
Webserv::~Webserv() {};

void Webserv::run()
{
	int ret = 0;
	struct timeval timeout;

	timeout.tv_sec = 1;
	timeout.tv_usec = 0;

	std::cout << YELLOW << getTime() << "Démarrage..." << RESET << std::endl;

	init();
	while (g_keepRunning)
	{
		_readySockets = _currentSockets;

		ret = select(_maxSocket + 1, &_readySockets, NULL, NULL, &timeout);

		if (ret < 0) {
			if (g_keepRunning == false)
				continue ;
			std::cerr << RED << "Error: select() failed: " << RESET << std::endl;
			exit(EXIT_FAILURE);
		}

		for (std::vector<int>::iterator it = _serversFD.begin() ; it != _serversFD.end() ; it++)
		{
			if (FD_ISSET(*it, &_readySockets)) {
				if (isServer(*it)) {
					int clientSocket = acceptNewClient(*it);
					RequestHTTP clientRequest;
					_clients[clientSocket] = clientRequest;
					if (clientSocket > _maxSocket)
						_maxSocket = clientSocket;
					FD_SET(clientSocket, &_currentSockets);
				}
			}
		}

		for (std::map<int, RequestHTTP>::iterator it = _clients.begin() ; it != _clients.end() ;)
		{
			if (FD_ISSET(it->first, &_readySockets)) {
				handleRead(it->first, it->second);
				FD_CLR(it->first, &_currentSockets);
				if (it->first == _maxSocket)
					_maxSocket -= 1;
				if (it->second.isOver())
				{
					sendResponse(it->first, it->second);
					close(it->first);
					it = _clients.erase(it);
				}
			}
			else {
				++it;
			}
		}
	}
	closeServers();
}

void Webserv::setConfig( std::vector<Config> & configs ) { _serversConfig = configs; };

// Private
void Webserv::init()
{
	FD_ZERO(&_currentSockets);

	launchServers();
}

void Webserv::launchServers()
{
	for (std::vector<Config>::iterator it = _serversConfig.begin(); it != _serversConfig.end(); it++) {
		std::cout << YELLOW << getTime() << "Launching server « " << it->get_server_name() << " »..." << RESET << std::endl;
		int serverSocket = initServerSocket(*it);
		_serversFD.push_back(serverSocket);
		FD_SET(serverSocket, &_currentSockets);
		_maxSocket = serverSocket;
		std::cout << GREEN << getTime() << "Server successfuly launched. Listening on « " << it->get_host_name() << ":" << std::to_string(it->get_port()) << " »." << RESET << std::endl;
	}
	std::cout << std::endl;
}

void Webserv::closeServers()
{
	std::cout << YELLOW << getTime() << "Shutting down server(s)..." << RESET;
	for (std::vector<int>::iterator it = _serversFD.begin(); it != _serversFD.end(); it++)
		close(*it);
}

/* S'occupe de créer le socket avec l'addresse IP 
** ainsi que le port renseigné dans le fichier config.
** Le socket est réutilisable et non-bloquant.
*/
int Webserv::initServerSocket( Config serverConfig )
{
	int 				serverSocket;
	struct sockaddr_in 	serverAddress;

	fcntl(serverSocket, F_SETFL, O_NONBLOCK);
	if ((serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		closeServers();
		throw (std::logic_error("Error: socket() failed"));
	}

	// Permet de réutiliser directement un socket lorsqu'on relance le programme.
	int optVal = 1;
	int optLen = sizeof(optVal);
	if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &optVal, optLen) < 0) {
		closeServers();
		throw (std::logic_error("Error: setsockopt(SO_REUSEADDR) failed"));
	}

	memset((char *)&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = serverConfig.get_host().s_addr;
	serverAddress.sin_port = htons(serverConfig.get_port());

	if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
		closeServers();
		throw (std::logic_error("Error: bind() failed"));
	}
	
	if (listen(serverSocket, MAX_CLIENTS) < 0) {
		closeServers();
		throw (std::logic_error("Error: listen() failed"));
	}

	return (serverSocket);
}

int Webserv::acceptNewClient( int serverSocket )
{
	int clientSocket;

	if ((clientSocket = accept(serverSocket, NULL, NULL)) < 0) {
		if (errno != EWOULDBLOCK) {
			 std::cerr << RED << "Error: accept() failed" << RESET << std::endl;
			 return (-1);
		}
	}

	if (fcntl(clientSocket, F_SETFL, O_NONBLOCK) < 0) {
		std::cerr << RED << "Error: fcntl() failed" << RESET << std::endl;
		return (-1);
	}

	return (clientSocket);
}

void Webserv::handleRead( int clientFD, RequestHTTP & parsedRequest )
{
	char	request[BUFFER_SIZE + 1] = {0};
	ssize_t	ret;
	
	ret = recv(clientFD, request, BUFFER_SIZE, 0);

	parsedRequest.setRequest(request);

	//std::cout << "End of request: (" << &request[ret-2] << ")\nRet: " << ret << "\n";
	//std::cout << "-------DEBUG-------\n" << parsedRequest.getRequest() << "\n-------------------\n";
	
	if (parsedRequest.getRequest().find("\r\n\r\n") != std::string::npos) {
		if (parsedRequest.getRequest().find("Content-Length: ") == std::string::npos) {
			if (parsedRequest.getRequest().find("Transfer-Encoding: chunked") != std::string::npos) {
				if (checkEnd(parsedRequest.getRequest(), "0\r\n\r\n") == 0)
					parsedRequest.setIsOver(true);
			}
			else
				parsedRequest.setIsOver(true);
		}
	}

	if (ret == -1) {
		// Remplit trop l'écran
		//std::cerr << RED << getTime() << "Error recv(): " << strerror(errno) << RESET << std::endl;
		return ;
	}

	if (parsedRequest.isOver()) {
		parsedRequest.parse();

		//std::cout << "-------DEBUG-------\n" << parsedRequest.getRequest() << "\n-------------------\n";

		if (parsedRequest.isOver()) {
			std::cout	<<	YELLOW << getTime()
						<< "<< [Host: "	<< parsedRequest.getHost()		<< "] "
						<< "[Method: "	<< parsedRequest.getMethod()	<< "] "
						<< "[File : "	<< parsedRequest.getFile()		<< "]" << RESET << std::endl;
		}
	}
}

void Webserv::sendResponse( int clientFD, RequestHTTP parsedRequest )
{
		Config			serverConfig = getServerConfig(parsedRequest.getHost());
		ResponseHTTP	response(serverConfig, parsedRequest);

		if (parsedRequest.getMethod() == "GET")
			response.GET(parsedRequest.getFile());
		else if (parsedRequest.getMethod() == "POST")
			response.POST(parsedRequest.getFile());
		else if (parsedRequest.getMethod() == "DELETE")
			response.DELETE(parsedRequest.getFile());
		else
			response.UNKNOWN(parsedRequest.getFile());


		if (write(clientFD, response.getResponseHTTP().c_str(), response.getResponseHTTP().size()) == -1)
			std::cerr << RED << "Coulnd't respond to the client." << RESET << std::endl;
		else
			std::cout << YELLOW << getTime() 
			<< ">> [Return Code: " << response.getStatusCode() << "] "
			<< "[Body Size: " << response.getBodySize() << "]"
			<< RESET << std::endl << std::endl;
}

bool Webserv::isServer( int readyFD )
{
	for (std::vector<int>::iterator it = _serversFD.begin() ; it != _serversFD.end() ; it++)
		if (*it == readyFD)
			return (true);
	return (false);
}

Config & Webserv::getServerConfig( std::string host )
{
	for (std::vector<Config>::iterator it = _serversConfig.begin() ; it != _serversConfig.end() ; it++) {
		std::string configHost = it->get_host_name() + ":" + std::to_string(it->get_port());
		if (host == configHost)
			return (*it);
	}
	throw (std::logic_error("Error: Server Config Not Found"));
}

std::string Webserv::getTime()
{
	char			buffer[32];
	struct timeval	tv;
	struct tm *		tm;

	gettimeofday(&tv, NULL);
	tm = localtime(&tv.tv_sec);
	strftime(buffer, 32, "[%H:%M:%S] ", tm);

	return (std::string(buffer));
}

int Webserv::checkEnd(const std::string & str, const std::string & end)
{
	size_t	i = str.size();
	size_t	j = end.size();

	while (j > 0) {
		i--;
		j--;
		if (i < 0 || str[i] != end[j])
			return (1);
	}
	return (0);
}