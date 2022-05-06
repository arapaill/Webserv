#include "../Includes/Parser.hpp"

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

	file.open("../Configs/config.conf");
	if(!file.is_open())
	{
		std::cout << "Cannot open config file";
		exit(0);
	}
	while(file)
	{
		file >> string_file;
		_vector_file.push_back(string_file);
	}
}

void    Parser::get_info(void)
{
	std::string	info;

	for(std::size_t i = 0; i != _vector_file.size(); i++)
	{
		info = _vector_file.at(i);
		if(info.find("listen"))
			is_listen(info);
			/*
		else if (info.find("server_name"))
			is_server_name(info);
		else if(info.find("root"))
			is_root(info);
		else if(info.find("index"))
			is_index(info);
			*/
	}

}

void    Parser::parse(void)
{
   init_vector_file();
   get_info();

}

int main()
{
	Parser pars;

	pars.parse();
	std::cout << pars.get_config_file().get_network().get_port() << std::endl;
	return(0);
}