#include "Webserv/Webserv.hpp"
#include "../Includes/Config.hpp"

extern volatile bool g_keepRunning;

void INThandler(int signum)
{
	(void)signum;
	g_keepRunning = false;
}

int main()
{
	Webserv server;
	Parser	parser;

	struct sigaction sigIntHandler;

	sigIntHandler.sa_handler = INThandler;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;

	sigaction(SIGINT, &sigIntHandler, NULL);

	parser.parse();
	server.setConfig(parser.get_vector_config());
	server.run();
}