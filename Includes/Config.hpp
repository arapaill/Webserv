#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "../Includes/headers.hpp"

class Config
{
	public:
		Config() :  _root(""), _server_name(""), _index("") {};
		Config(Config const &other) {*this = other;};
		~Config() {};

		Config & operator=(const Config & rhs)
		{
			this->_host = rhs._host;
			this->_port = rhs._port;
			this->_host_name = rhs._host_name;
			_root = rhs._root;
			_server_name = rhs._server_name;
			_index = rhs._index;
            _autoindex = rhs._autoindex;
            _client_max_body_size = rhs._client_max_body_size;
			return *this;
		}

		void			set_host(struct in_addr host) {_host = host;};
		void			set_port(int port) {_port = port;};
		void			set_host_name(std::string host_name) { _host_name = host_name;};
		void		set_root(std::string &root) {_root = root;};
		void		set_server_name(std::string server_name) {_server_name = server_name;};
		void		set_index(std::string index) {_index = index;};
		void		set_autoindex(bool autoindex) {_autoindex = autoindex;};
		void		set_client_max_body_size(size_t size) {_client_max_body_size = size;};

		struct in_addr	& get_host(void) {return(_host);};
		int				& get_port(void) {return(_port);};
		std::string		& get_host_name(void) {return(_host_name);};
		std::string		& get_root(void) {return(_root);};
		std::string		& get_server_name(void){return(_server_name);};
		std::string		& get_index(void) {return(_index);};
		bool			& get_autoindex(bool autoindex) {return(_autoindex);};
		size_t			& get_client_max_body_size(size_t size) {return(_client_max_body_size);};

	private: 
		struct in_addr	    			_host;
		int				    			_port;
		std::string		    			_host_name;
		std::string         			_root;
		std::string         			_server_name;
		std::string						_index;
		bool							_autoindex;
		size_t							_client_max_body_size;
		std::map<std::string, Config>	_location;
};

#endif