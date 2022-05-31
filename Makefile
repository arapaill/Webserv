CPP = c++
CPPFLAGS = -std=c++98

NAME = webserv
CLIENT = Srcs/TEST_client.cpp
SERVER = Srcs/TEST_server.cpp

all: 
		c++ Srcs/main.cpp Srcs/Webserv/Webserv.cpp Srcs/ResponseHTTP/ResponseHTTP.cpp Srcs/Parser/Parser.cpp Srcs/Parser/Is_key_word.cpp && ./a.out
