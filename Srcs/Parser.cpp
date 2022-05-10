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

void    Parser::init_vector_string(void)
{
	std::ifstream file;
	std::string string_file;
	std::string concat_string_file;
    std::vector<std::string> vector_string;
    int count = 0;

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
        if(concat_string_file.find('{')  != std::string::npos)
            count++;
        if(concat_string_file.find('}') != std::string::npos)
            count--;
		if(concat_string_file.find(';') != std::string::npos
			|| concat_string_file.find('{') != std::string::npos
			|| concat_string_file.find('}') != std::string::npos)
		{
			if(concat_string_file.find(';') != std::string::npos)
				concat_string_file.resize(concat_string_file.size() - 2);
            else
                concat_string_file.resize(concat_string_file.size() - 1);
			//std::cout << concat_string_file << std::endl;
			vector_string.push_back(concat_string_file);
            if(count == 0)
            {
                _allblock.push_back(vector_string);
                vector_string.clear();
            }
			concat_string_file.clear();
		}
	}
}

void    Parser::get_info(std::vector<std::string> vector_string)
{
	std::string	info;

	for(std::size_t i = 0; i != vector_string.size() && vector_string.at(i) != "}"; i++)
	{
		info = vector_string.at(i);
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
    std::string null = "NULL";

	_config_file.set_root(null);
	_config_file.set_server_name(null);
	_config_file.set_index(null);
	_config_file.get_network().get_host().s_addr = inet_addr("0.0.0.0");
	_config_file.get_network().set_port(-1);
	_config_file.get_network().set_host_name(null);
}

void    Parser::parse(void)
{
	
	init_vector_string();
    for(std::size_t i = 0; i < _allblock.size(); i++)
    {
        init_config_file();
	    get_info(_allblock.at(i));
        _vector_Config.push_back(_config_file);
    }
    std::cout << _vector_Config.at(1).get_server_name() << std::endl;

}

int main()
{
	Parser pars;
    std::vector<Config> conffile;

	pars.parse();
	conffile.push_back(pars.get_config_file());
	return(0);
}