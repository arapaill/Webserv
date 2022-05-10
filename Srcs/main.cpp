#include "Webserv/Webserv.hpp"
#include "../Includes/Config.hpp"
#include "../Includes/t_network.hpp"

int main()
{
	Webserv server;
	Parser	parser;

	parser.parse();
	server.setParser(parser);
	server.run();
}