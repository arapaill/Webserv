#include "Webserv.hpp"

// Public
Webserv::Webserv() {};
Webserv::~Webserv() {};

void Webserv::run()
{
	std::cout << "Démarrage du serveur...\n";
	init();
	while (1)
	{
		for (int i = 0 ; i < _serversFD.size() ; i++)
			AcceptNewClient(_serversFD[i]);
	}
}

void Webserv::setParser( Parser & parser ) { _parser = parser; };

// Private
void Webserv::init()
{
	_serversConfig = _parser.get_vector_config();
	initServers();
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

void Webserv::AcceptNewClient( int server )
{
	int new_socket;

	if ((new_socket = accept(server, NULL, NULL)) < 0)
	{
		throw (std::logic_error("Error: accept() failed"));
	}
	std::cout << "Client connected to server !\n";


	char buffer[1024] = {0};
	std::string line;

	int valread = read(new_socket, buffer, 1024);

	std::string requested_file = &buffer[4];
	requested_file = requested_file.substr(0, requested_file.find(' '));

	std::cout << "Requested File : \"" << requested_file << "\"\n";

	// Temporaire
	ResponseHTTP response;

	if (requested_file.size() == 1)
		response.requestFile("index.html");
	else
		response.requestFile(requested_file + ".html");
	std::string s_response = response.getResponseHTTP();
	char * c_response = &s_response[0];
	write(new_socket, c_response, strlen(c_response));
}