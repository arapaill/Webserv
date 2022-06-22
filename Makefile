CPP = c++
CPPFLAGS = #-std=c++98

NAME = webserv
SRCS = Srcs/main.cpp Srcs/Webserv/Webserv.cpp Srcs/ResponseHTTP/ResponseHTTP.cpp \
Srcs/Parser/Parser.cpp Srcs/Parser/Is_key_word.cpp Srcs/RequestHTTP/RequestHTTP.cpp \
Srcs/CGIhandler/CGIhandler.cpp


all: 		$(NAME)

$(NAME): 	$(SRCS)
			$(CPP) $(CPPFLAGS) -o $@ $^
clean:
			rm -f $(NAME)

re: 		clean all

exec: 		all
			./webserv
