#ifndef CONFIG_HPP
#define CONFIG_HPP

# include "t_network.hpp"

class Config
{
	public:
		Config() :  _root(""), _server_name(""), _index("") {};
		Config(Config const &other) {*this = other;};
		~Config() {};

		Config & operator=(const Config & rhs)
		{
			_network = rhs._network;
			_root = rhs._root;
			_server_name = rhs._server_name;
			_index = rhs._index;
			return *this;
		}

		void		set_network(t_network &network) {_network = network;};
		void		set_root(std::string &root) {_root = root;};
		void		set_server_name(std::string server_name) {_server_name = server_name;};
		void		set_index(std::string index) {_index = index;};

		t_network	&get_network(void){return(_network);};
		std::string	&get_root(void) {return(_root);};
		std::string	&get_server_name(void){return(_server_name);};
		std::string	&get_index(void) {return(_index);};
        /*
        void    debug(void)
        {
            std::cout << "__________ DEBUG __________\n";
            _network.debug();
            std::cout << "ROOT : " << _root << std::endl;
            std::cout << "SERVER_NAME : " << _server_name << std::endl;
            std::cout << "INDEX : " << _index << std::endl;
            std::cout << "__________ END __________\n";
        }
        */
	private:
		t_network			_network;
		std::string         _root;
		std::string         _server_name;
		std::string			_index;
};

#endif