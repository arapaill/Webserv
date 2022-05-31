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
	int timeout = 200;
	int nfds = 0;

	init();
	while (g_run)
	{
		errno = 0;

		nfds = epoll_wait(_epollfd, _events, MAX_EVENTS, timeout);
		if (errno == EINVAL || errno == EFAULT || errno == EBADFD)
			std::cerr << RED << "Error: epoll_wait() failed: " << strerror(errno) << RESET << std::endl;
		else if (errno == EINTR) // epoll interrupted by a signal (CTRL+C)
			g_run = false;
		for (int i = 0 ; i < nfds ; i++)
		{
			if (_events[i].events & EPOLLIN && isFDServer(_events[i].data.fd))
				acceptNewClient(_events[i].data.fd);
			else if (_events[i].events & EPOLLIN)
				handleRead(_events[i].data.fd);
		}
		if (nfds == 0) {
			std::cout << YELLOW << "\r" << wait[(n++ % 6)] << " waiting for connection" << RESET << std::flush;
		}
	}
	closeServers();
}


void Webserv::setConfig( std::vector<Config> & configs ) { _serversConfig = configs; };

// Private
void Webserv::init()
{
	_epollfd = epoll_create1(0);

	if (_epollfd == -1)
		throw (std::runtime_error("Error: epoll_create1() failed"));

	launchServers();

	std::memset((struct epoll_event *)&_ev, 0, sizeof(_ev));
	for (std::vector<int>::iterator it = _serversFD.begin(); it != _serversFD.end(); it++)
	{
		_ev.data.fd = *it;
		_ev.events = EPOLLIN;
		epoll_ctl(_epollfd, EPOLL_CTL_ADD, *it, &_ev);
	}
}

void Webserv::launchServers()
{
	for (std::vector<Config>::iterator it = _serversConfig.begin(); it != _serversConfig.end(); it++)
	{
		std::cout << YELLOW << "Launching server \"" << it->get_server_name() << "\"...\n" << RESET;
		_serversFD.push_back(initSocket(*it));
		std::cout << GREEN << "Server successfuly launched.\n" << RESET;
	}
}

void Webserv::closeServers()
{
	std::cout << YELLOW << "\nShutting down server...\n" << RESET;
	for (std::vector<int>::iterator it = _serversFD.begin(); it != _serversFD.end(); it++)
		close(*it);
	close(_epollfd);
}

/* S'occupe de créer le socket avec l'addresse IP 
** ainsi que le port renseigné dans network.
** Le socket est réutilisable et non-bloquant.
*/
int Webserv::initSocket( Config serverConfig )
{
	int 				socket_listening;
	struct sockaddr_in 	server_address;

	fcntl(socket_listening, F_SETFL, O_NONBLOCK);
	if ((socket_listening = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		throw (std::logic_error("Error: socket() failed"));
	}

	memset((char *)&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = serverConfig.get_host().s_addr;
	server_address.sin_port = htons(serverConfig.get_port());

	if (bind(socket_listening, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
	{
		throw (std::logic_error("Error: bind() failed"));
	}
	
	if (listen(socket_listening, MAX_CLIENTS) < 0)
	{
		throw (std::logic_error("Error: listen() failed"));
	}

	return (socket_listening);
}

void Webserv::acceptNewClient( int serverFD )
{
	int new_socket;

	if ((new_socket = accept(serverFD, NULL, NULL)) < 0)
	{
		if (errno != EWOULDBLOCK)
			 std::cerr << RED << "Error: accept() failed" << RESET << std::endl;
	}

	if (fcntl(new_socket, F_SETFL, O_NONBLOCK) < 0)
	{
		std::cerr << RED << "Error: fcntl() failed" << RESET << std::endl;
		return ;
	}

	_ev.data.fd = new_socket;
	_ev.events = EPOLLIN;
	epoll_ctl(_epollfd, EPOLL_CTL_ADD, new_socket, &_ev);
/* 
	client.setSocket(new_socket);
	_clients[new_socket] = client; */
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
		//std::cout << buffer;

		std::string requested_file = &request[4];

		requested_file = requested_file.substr(0, requested_file.find(' '));

		std::cout << GREEN << "\nClient requested file \"" << requested_file << "\"" << RESET << std::flush;

		ResponseHTTP response;

		if (requested_file.size() == 1)
			response.requestFile("index.html");
		else
			response.requestFile(requested_file + ".html");
		std::string s_response = response.getResponseHTTP();
		char * c_response = &s_response[0];
		write(clientFD, c_response, strlen(c_response));
		_ev.events = EPOLLOUT;
		_ev.data.fd = clientFD;
		epoll_ctl(_epollfd, EPOLL_CTL_MOD, clientFD, &_ev);
	}
}

bool	Webserv::isFDServer(int readyFD)
{
	for (std::vector<int>::iterator it = _serversFD.begin(); it != _serversFD.end(); it++)
		if (*it == readyFD)
			return (true);
	return (false);
}