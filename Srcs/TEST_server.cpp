#include <sys/types.h>
#include <sys/socket.h>
#include <string>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fstream>

#define PORT 8080


/* struct sockaddr_in
{
	__uint8_t         sin_len;
    sa_family_t       sin_family;	// La famille de l'addresse de notre socket = IF_INET
    in_port_t         sin_port;		// Le port à utiliser, par défaut 80
    struct in_addr    sin_addr;		// L'addresse IP du serveur (IP du PC) On peut ça vide par défaut
    char              sin_zero[8];
}; */

int main()
{
	// 1. Create the socket
	int test_server = socket(AF_INET, SOCK_STREAM, 0);

	if (test_server < 0)
	{
		std::cout << "Error creating Socket\n";
		return (-1);
	}

	// 2. Identify (name) the socket
	struct sockaddr_in address;
	int addrlen = sizeof(address);

	memset((char *)&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	if (bind(test_server, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		std::cout << "Error binding Socket\n";
		return (-1);
	}

	std::cout << "Server Listening...\n";
	while (1)
	{
		// 3. On the server, wait for an incoming connection
		if (listen(test_server, 5) < 0)
		{
			std::cout << "Error listening\n";
			return (-1);
		}
		int new_socket;
		if ((new_socket = accept(test_server, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
		{
			std::cout << "Error accepting\n";
			return (-1);
		}

		// 4. Send and receive messages
		char buffer[1024] = {0};

		int valread = read(new_socket, buffer, 1024);
		std::cout << buffer;
		if (valread < 0)
		{
			std::cout << "Nothing received\n";
		}

		std::ifstream index("../HTML/index.html");
		std::string file;
		std::string line;

		while (getline(index, line))
			file += line + '\n';
		index.close();

		std::string s_response = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: "; //<< file.size() << "\n\n" << file;
		s_response += file.size();
		s_response += "\n\n";
		s_response += file;
		char *		c_response = &s_response[0];
		write(new_socket, c_response, strlen(c_response));

		// 5. Close the socket
		close(new_socket);
	}
	close(test_server);
	return (0);
}

