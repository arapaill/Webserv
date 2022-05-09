#include "../Includes/Parser.hpp"
 #include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
# define BUFFER_SIZE 4096

Parser::Parser(void) {}
Parser::Parser(Parser const & other)
{
	*this = other;
}
Parser::~Parser(void) {}

Parser&			Parser::operator=(const Parser &rhs)
{
	this->_config_file = rhs._config_file;
	this->_path_file = rhs._path_file;
	return (*this);
}

void    Parser::init_vector_file(void)
{
	std::ifstream file;
	std::string string_file;
	std::string concat_string_file;
	file.open("../Configs/config.conf");
	if(!file.is_open())
	{
		std::cout << "Cannot open config file";
		exit(0);
	}
	while(file)
	{
		file >> string_file;
		concat_string_file += string_file;
		concat_string_file += " ";
		if(concat_string_file.find(';') != std::string::npos
			|| concat_string_file.find('{') != std::string::npos
			|| concat_string_file.find('}') != std::string::npos)
		{
			if(concat_string_file.find(';') != std::string::npos)
				concat_string_file.resize(concat_string_file.size() - 2);
			//std::cout << concat_string_file << std::endl;
			_vector_file.push_back(concat_string_file);
			concat_string_file.clear();
		}
	}
}

void    Parser::get_info(void)
{
	std::string	info;

	for(std::size_t i = 0; i != _vector_file.size() && _vector_file.at(i) != "}"; i++)
	{
		info = _vector_file.at(i);
       	//std::cout << "INFO:" << info << std::endl;
		if(info.find("listen ") != std::string::npos)
		{
			//std::cout << "in listen \n";
			is_listen(info);
		}
		else if (info.find("server_name ") != std::string::npos)
			is_server_name(info);
		else if(info.find("root ") != std::string::npos)
			is_root(info);
		else if(info.find("index ") != std::string::npos)
			is_index(info);
	}
}

void	Parser::init_config_file(void)
{
	//_config_file.set_root(NULL);
	//_config_file.set_server_name(NULL);
	//_config_file.set_index(NULL);
	//_config_file.get_network().set_host(NULL);
	_config_file.get_network().set_port(-1);
	//_config_file.get_network().set_host_name(NULL);
}

void    Parser::parse(void)
{
	//init_config_file();
	init_vector_file();
	get_info();

}
