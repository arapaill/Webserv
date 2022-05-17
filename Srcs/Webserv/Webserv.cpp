#include "Webserv.hpp"

// Public
Webserv::Webserv() {};
Webserv::~Webserv() {};

void Webserv::run()
{
	std::cout << "Démarrage du serveur...\n";
	
	int n = 0;
	std::string  wait[] = {"⠋", "⠙", "⠸", "⠴", "⠦", "⠇"};
	int timeout = 200;
	int nfds = 0;

	init();

	while (1)
	{
		errno = 0;

		nfds = epoll_wait(_epollfd, _events, MAX_EVENTS, timeout);
		if (errno == EINVAL || errno == EFAULT || errno == EBADFD)
			std::cerr << "Error: epoll_wait() failed: " << strerror(errno) << '\n';
		else if (errno == EINTR) // epoll interrupted by a signal (CTRL+C)
			break ;
		for (int i = 0 ; i < nfds ; i++)
		{
			if (_events[i].events & EPOLLIN && fd_is_server(_events[i].data.fd))
				acceptNewClient(_events[i].data.fd);
			else if (_events[i].events & EPOLLIN)
				handleRead(_events[i].data.fd);
		}
		if (nfds == 0)
		{
			std::cout << "\r" << wait[(n++ % 6)] << " waiting for connection" << std::flush;
		}
	}
}

void Webserv::setParser( Parser & parser ) { _parser = parser; };

// Private
void Webserv::init()
{
	_serversConfig = _parser.get_vector_config();

	_epollfd = epoll_create1(0);
	if (_epollfd == -1)
		throw (std::runtime_error("Error: epoll_create1() failed"));

	initServers();

	std::memset((struct epoll_event *)&_ev, 0, sizeof(_ev));
	for (serverFDVector::iterator it = _serversFD.begin(); it != _serversFD.end(); it++)
	{
		_ev.data.fd = *it;
		_ev.events = EPOLLIN;
		epoll_ctl(_epollfd, EPOLL_CTL_ADD, *it, &_ev);
	}
}

void Webserv::initServers()
{
	for (configVector::iterator it = _serversConfig.begin(); it != _serversConfig.end(); it++)
	{
		t_network network = it->get_network();
		std::cout << "Lancement du serveur \"" << it->get_server_name() << "\"...\n";
		_serversFD.push_back(initSocket(network));
	}
}

/* S'occupe de créer le socket avec l'addresse IP 
** ainsi que le port renseigné dans network.
** Le socket est réutilisable et non-bloquant.
*/
int Webserv::initSocket( t_network network )
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
	server_address.sin_addr.s_addr = network.get_host().s_addr;
	server_address.sin_port = htons(network.get_port());

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
			 std::cerr << "Error: accept() failed" << '\n';
	}

	if(fcntl(new_socket, F_SETFL, O_NONBLOCK) < 0)
	{
		std::cerr << "Error: fcntl() failed" << '\n';
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
	char buffer[1024] = {0};
	std::string line;

	int valread = read(clientFD, buffer, 1024);

	//std::cout << buffer;

	std::string requested_file = &buffer[4];

	requested_file = requested_file.substr(0, requested_file.find(' '));

	std::cout << "\nClient requested file \"" << requested_file << "\"";

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

bool	Webserv::fd_is_server(int ready_fd)
{
	for (serverFDVector::iterator it = _serversFD.begin(); it != _serversFD.end(); it++)
		if (*it == ready_fd)
			return true;
	return false;
}