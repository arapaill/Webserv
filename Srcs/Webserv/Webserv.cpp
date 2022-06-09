#include "Webserv.hpp"

// Public
volatile bool g_keepRunning = true;

Webserv::Webserv() {};
Webserv::~Webserv() {};

void Webserv::run()
{
	int ret = 0;

	std::cout << YELLOW << "Démarrage..." << RESET << std::endl;

	init();
	while (g_keepRunning)
	{
		_readySockets = _currentSockets;

		ret = select(FD_SETSIZE, &_readySockets, NULL, NULL, NULL);

		if (ret < 0) {
			if (g_keepRunning == false)
				continue ;
			std::cerr << RED << "Error: select() failed: " << RESET << std::endl;
			exit(EXIT_FAILURE);
		}

		for (int i = 0 ; i < FD_SETSIZE ; i++) {
			if (FD_ISSET(i, &_readySockets)) {
				if (isServer(i)) {
					int clientSocket = acceptNewClient(i);
					FD_SET(clientSocket, &_currentSockets);
				}
				else {
					handleRead(i);
					FD_CLR(i, &_currentSockets);
				}
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
	for (std::vector<Config>::iterator it = _serversConfig.begin(); it != _serversConfig.end(); it++)
	{
		std::cout << YELLOW << "Launching server \"" << it->get_server_name() << "\"..." << RESET << std::endl;
		int serverSocket = initSocket(*it);
		_serversFD.push_back(serverSocket);
		FD_SET(serverSocket, &_currentSockets);
		std::cout << GREEN << "Server successfuly launched." << RESET << std::endl;
	}
	std::cout << std::endl;
}

void Webserv::closeServers()
{
	std::cout << YELLOW << "\nShutting down server..." << RESET << std::endl;
	for (std::vector<int>::iterator it = _serversFD.begin(); it != _serversFD.end(); it++)
		close(*it);
}

/* S'occupe de créer le socket avec l'addresse IP 
** ainsi que le port renseigné dans le fichier config.
** Le socket est réutilisable et non-bloquant.
*/
int Webserv::initSocket( Config serverConfig )
{
	int 				serverSocket;
	struct sockaddr_in 	serverAddress;

	fcntl(serverSocket, F_SETFL, O_NONBLOCK);
	if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		closeSockets();
		throw (std::logic_error("Error: socket() failed"));
	}

	/* 
	** Permet de réutiliser directement un socket lorsqu'on relance le programme.
	*/
	int		optVal = 1;
	int		optLen = sizeof(optVal);
	if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &optVal, optLen) < 0)
	{
		closeSockets();
		throw (std::logic_error("Error: setsockopt(SO_REUSEADDR) failed"));
	}

	memset((char *)&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = serverConfig.get_host().s_addr;
	serverAddress.sin_port = htons(serverConfig.get_port());

	if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
	{
		closeSockets();
		throw (std::logic_error("Error: bind() failed"));
	}
	
	if (listen(serverSocket, MAX_CLIENTS) < 0)
	{
		closeSockets();
		throw (std::logic_error("Error: listen() failed"));
	}

	return (serverSocket);
}

int Webserv::acceptNewClient( int serverSocket )
{
	int clientSocket;

	if ((clientSocket = accept(serverSocket, NULL, NULL)) < 0)
	{
		if (errno != EWOULDBLOCK)
		{
			 std::cerr << RED << "Error: accept() failed" << RESET << std::endl;
			 return (-1);
		}
	}

	if (fcntl(clientSocket, F_SETFL, O_NONBLOCK) < 0)
	{
		std::cerr << RED << "Error: fcntl() failed" << RESET << std::endl;
		return (-1);
	}

	return (clientSocket);
}

void Webserv::handleRead( int clientFD )
{
	char			request[BUFFER_SIZE + 1] = {0};
	ssize_t			ret = recv(clientFD, request, BUFFER_SIZE, 0);

	if (ret == -1)
	{
		std::cerr << "Error: recv() failed\n";
		return ;
	}
	else if (ret == 0)
		return ;
	else
	{
		std::cout << GREEN << "INCOMING DATA :" << RESET << std::endl;
		
		RequestHTTP		parsedRequest(request);
		Config			serverConfig = getServerConfig(parsedRequest.getHost());
		ResponseHTTP	response(serverConfig, parsedRequest);

		std::cout	<< "Method : "	<< parsedRequest.getMethod() << std::endl
					<< "Host : "	<< parsedRequest.getHost()	 << std::endl
					<< "File : "	<< parsedRequest.getFile()	 << std::endl;

		if (parsedRequest.getMethod() == "GET")
			response.GET(parsedRequest.getFile() + ".html");
		else if (parsedRequest.getMethod() == "POST")
			response.POST(parsedRequest.getFile());

		if (write(clientFD, response.getResponseHTTP().c_str(), response.getResponseHTTP().size()) == -1)
			std::cerr << RED << "Coulnd't respond to the client." << RESET << std::endl;
		else
			std::cout	<< "Return Code : " << response.getStatusCode() << std::endl << std::endl;
	}
}

bool Webserv::isServer( int readyFD )
{
	for (std::vector<int>::iterator it = _serversFD.begin() ; it != _serversFD.end() ; it++)
		if (*it == readyFD)
			return (true);
	return (false);
}

void Webserv::closeSockets()
{
	for (std::vector<int>::iterator it = _serversFD.begin() ; it != _serversFD.end() ; it++)
		close(*it);
}

Config & Webserv::getServerConfig( std::string host )
{
	for (std::vector<Config>::iterator it = _serversConfig.begin() ; it != _serversConfig.end() ; it++)
	{
		std::string configHost = it->get_host_name() + ":" + std::to_string(it->get_port());
		if (host == configHost)
			return (*it);
	}
	throw (std::logic_error("Error: Server Config Not Found"));
}
