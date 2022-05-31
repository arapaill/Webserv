#ifndef HEADERS_HPP
# define HEADERS_HPP

# include <sys/types.h>
# include <sys/socket.h>
# include <cstring>
# include <iostream>
# include <string>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <fstream>
# include <errno.h>
# include <sstream>
# include <fcntl.h>
# include <sys/epoll.h>
# include <vector>
# include <signal.h>
# include <map>

# include "Parser.hpp"
# include "Config.hpp"

# include "../Srcs/ResponseHTTP/ResponseHTTP.hpp"

//* COLORS
# define RED "\033[31m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define CYANBG "\033[46m"
# define BLUE "\033[96m"
# define ORANGEBG "\033[1;44m"
# define PINKBG "\033[1;41m"
# define GREENBG "\033[42m"
# define PURPLEBG "\033[45m"
# define GRAYBG "\033[40m"
# define RESET "\033[0m"

# define MAX_CLIENTS 10
# define MAX_EVENTS 10
# define BUFFER_SIZE 1024

typedef std::vector<Config>	configVector;
typedef std::vector<int>	serverFDVector;

#endif