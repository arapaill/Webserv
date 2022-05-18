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

# include "Parser.hpp"
# include "Config.hpp"
# include "t_network.hpp"

# include "../Srcs/ResponseHTTP/ResponseHTTP.hpp"

# define MAX_CLIENTS 10
# define MAX_EVENTS 10

typedef std::vector<Config>	configVector;
typedef std::vector<int>	serverFDVector;

#endif