#include "Webserv/Webserv.hpp"
#include "../Includes/Config.hpp"
#include "../Includes/t_network.hpp"

extern volatile bool g_run;

void signal_handler(int signum)
{
	(void)signum;
	g_run = false;
}

int main()
{
	Webserv server;
	Parser	parser;

	signal(SIGINT, signal_handler);
	parser.parse();
	server.setConfig(parser.get_vector_config());
	server.run();
}