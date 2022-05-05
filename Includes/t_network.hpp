#ifndef T_NETWORK
#define T_NETWORK

# include <string>
# include <netinet/in.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <ostream>

class   t_network
{
	public:
		t_network(void);
		t_network(t_network const & other);
		~t_network(void){};

		t_network & operator=(const t_network &	rhs)
		{
			if (this != &rhs)
			{
				this->_host = rhs._host;
				this->_port = rhs._port;
				this->_host_name = rhs._host_name;
			}
			return (*this);
		}

		void			set_host(struct in_addr host) {_host = host;};
		void			set_port(int port) {_port = port;};
		void			set_host_name(std::string host_name) { _host_name = host_name;};

		struct in_addr	&get_host(void) {return(_host);};
		int				&get_port(void) {return(_port);};
		std::string		&get_host_name(void) {return(_host_name);};

	private:
		struct in_addr	_host;
		int				_port;
		std::string		_host_name;
};

#endif