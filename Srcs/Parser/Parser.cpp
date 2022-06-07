#include "Parser.hpp"

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
	this->_allblock = rhs._allblock;
	this->_vector_Config = rhs._vector_Config;
	return (*this);
}

void    Parser::init_vector_string(void)
{
	std::ifstream file;
	std::string string_file;
	std::string concat_string_file;
	std::vector<std::string> vector_string;
	int count = 0;

	file.open("Configs/config.conf");
	if(!file.is_open())
	{
		std::cout << "Cannot open config file\n";
		exit(0);
	}
	while(file)
	{
		file >> string_file;
		concat_string_file += string_file;
		concat_string_file += " ";
		if (concat_string_file.find('{')  != std::string::npos)
			count++;
		if (concat_string_file.find('}') != std::string::npos)
			count--;
		if (concat_string_file.find(';') != std::string::npos
			|| concat_string_file.find('{') != std::string::npos
			|| concat_string_file.find('}') != std::string::npos)
		{
			if (concat_string_file.find(';') != std::string::npos)
				concat_string_file.resize(concat_string_file.size() - 2);
			else
				concat_string_file.resize(concat_string_file.size() - 1);
			//std::cout << concat_string_file << std::endl;
			vector_string.push_back(concat_string_file);
			if (count == 0)
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
		if (info.find("#") != std::string::npos)
			info.erase(info.find("#"));
	   	//std::cout << "INFO:" << info << std::endl;
		if (info.find("listen ") != std::string::npos)
		{
			//std::cout << "in listen \n";
			is_listen(info);
		}
		else if (info.find("server_name ") != std::string::npos)
			is_server_name(info);
		else if (info.find("root ") != std::string::npos)
			is_root(info);
		else if (info.find("index ") != std::string::npos)
			is_index(info);
		else if (info.find("autoindex ") != std::string::npos)
			is_autoindex(info);
	   /* else if (info.find("error_page ") != std::string::npos)
			is_error_page(info);*/
		else if (info.find("client_max_body_size ") != std::string::npos)
			is_client_max_body_size(info);
		/*else if (info.find("fastcgi_param ") != std::string::npos)
			is_fastcgi_param(info);*/
	}
}

void	Parser::init_config_file(void)
{
	std::string null = "NULL";

	_config_file.set_root(null);
	_config_file.set_server_name(null);
	_config_file.set_index(null);
	_config_file.get_host().s_addr = inet_addr("0.0.0.0");
	_config_file.set_port(-1);
	_config_file.set_host_name(null);
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
//    std::cout << _vector_Config.at(0).get_server_name() << std::endl;
}

/*
int main()
{
	Parser pars;
	std::vector<Config> conffile;

	pars.parse();
	return(0);
}
*/