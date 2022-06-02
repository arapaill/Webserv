#include "Webserv.hpp"

// Public
volatile bool g_run = true;

Webserv::Webserv() {};
Webserv::~Webserv() {};

void Webserv::run()
{
	std::cout << YELLOW << "Démarrage...\n" << RESET;
	
	int n = 0;
	std::string wait[] = {"⠋", "⠙", "⠸", "⠴", "⠦", "⠇"};
	int ret = 0;

	init();
	while (g_run)
	{
		errno = 0;

		_readySockets = _currentSockets;

		ret = select(FD_SETSIZE, &_readySockets, NULL, NULL, NULL);

		if (errno == EINTR) // Je ne sais pas si c'est encore nécessaire..
			g_run = false;
		if (ret < 0)
		{
			std::cerr << RED << "Error: select() failed: " << RESET << std::endl;
			exit(EXIT_FAILURE);
		}

		for (int i = 0 ; i < FD_SETSIZE ; i++)
		{
			if (FD_ISSET(i, &_readySockets))
			{
				if (isServer(i))
				{
					int clientSocket = acceptNewClient(i);
					FD_SET(clientSocket, &_currentSockets);
				}
				else
				{
					handleRead(i);
					FD_CLR(i, &_currentSockets);
				}
			}
		}

		if (ret == 0) {
			std::cout << YELLOW << "\r" << wait[(n++ % 6)] << " waiting for connection" << RESET << std::flush;
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
		std::cout << YELLOW << "Launching server \"" << it->get_server_name() << "\"...\n" << RESET;
		int serverSocket = initSocket(*it);
		_serversFD.push_back(serverSocket);
		FD_SET(serverSocket, &_currentSockets);
		std::cout << GREEN << "Server successfuly launched.\n" << RESET;
	}
}

void Webserv::closeServers()
{
	std::cout << YELLOW << "\nShutting down server...\n" << RESET;
	for (std::vector<int>::iterator it = _serversFD.begin(); it != _serversFD.end(); it++)
		close(*it);
}

/* S'occupe de créer le socket avec l'addresse IP 
** ainsi que le port renseigné dans network.
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
	char request[BUFFER_SIZE + 1] = {0};
	std::string line;

	ssize_t ret = recv(clientFD, request, BUFFER_SIZE, 0);

	std::cout << "\n" << request;

	if (ret == -1)
	{
		std::cerr << "Error: recv() failed\n";
		return ;
	}
	else if (ret == 0)
		return ;
	else
	{
		//std::cout << _serversConfig[clientFD].get_port();

		std::string requestedFile = &request[4];

		requestedFile = requestedFile.substr(0, requestedFile.find(' '));

		std::cout << GREEN << "\nREQUEST :" << RESET << std::flush;

		ResponseHTTP response;

		if (requestedFile.size() == 1)
			response.requestFile("index.html");
		else
			response.requestFile(requestedFile + ".html");
		std::string s_response = response.getResponseHTTP();
		char * c_response = &s_response[0];
		write(clientFD, c_response, strlen(c_response));
	}
}

bool	Webserv::isServer(int readyFD)
{
	for (std::vector<int>::iterator it = _serversFD.begin(); it != _serversFD.end(); it++)
		if (*it == readyFD)
			return (true);
	return (false);
}

void Webserv::closeSockets()
{
	for (std::vector<int>::iterator it = _serversFD.begin(); it != _serversFD.end(); it++)
		close(*it);
}