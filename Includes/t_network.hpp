#ifndef T_NETWORK
#define T_NETWORK

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
				this->host = rhs.host;
				this->port = rhs.port;
				this->hostName = rhs.hostName;
			}
			return (*this);
		}

		void					set_host(struct in_addr host) {_host = host;};
		void					set_port(int port) {_port = port;};
		void					set_host_name(std::string host_name) { _host_name = host_name;};

		struct in_addr	_host	&get_host(void) {return(_host);};
		int						&get_port(void) {return(_port);};
		std::string				&get_host_name(void) {return(_host_name);};

	private:
		struct in_addr	_host;
		int				_port;
		std::string		_host_name;
};

#endif