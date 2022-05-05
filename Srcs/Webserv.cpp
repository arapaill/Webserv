#include "Webserv.hpp"

// Public
Webserv::Webserv() {};
Webserv::~Webserv() {};

void Webserv::run()
{

}

// Private
void Webserv::init()
{
	
}



void Webserv::initServers()
{
	for (configVector::iterator it = _server.begin() ; it != _server.end() ; it++)
	{
		initSocket(network);
	}
}

/* S'occupe de créer le socket avec l'addresse IP 
** ainsi que le port renseigné dans network.
** Le socket est réutilisable et non-bloquant.
*/
int Webserv::initSocket(t_network network)
{
	int 				socket_listening;
	struct sockaddr_in 	server_address;

	if ((socket_listening = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		throw (std::logic_error("Error: socket() failed"));
	}

	set::memset((char *)&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = network.host.s_addr;
	server_address.sin_port = htons(network.port)

	if (bind(sockaddr_in, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
	{
		throw (std::logic_error("Error: bind() failed"));
	}
	
	if (listen(socket_listening, MAX_CLIENTS) < 0)
	{
		throw (std::logic_error("Error: listen() failed"));
	}
	
	return (socket_listening);
}
