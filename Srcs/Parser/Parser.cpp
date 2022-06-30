#include "Parser.hpp"

Parser::Parser(void) 
{
	_path_file = "Configs/Config.conf"; // Default Path
}

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
	std::string last;
	std::string string_file;
	std::vector<std::string> vector_string;
	int count = 0;

	file.open(_path_file);
	if(!file.is_open())
	{
		std::cout << "Cannot open config file\n";
		exit(0);
	}
	while(!file.eof())
	{
		std::getline(file, string_file);
		size_t i = 0;
		for (std::string::iterator it = string_file.begin() ; it != string_file.end() ; it++)
		{
			if (!isspace(*it))
				break ;
			i++;
		}
		string_file = string_file.substr(i);
		if(string_file.size() <= 1 && string_file.front() == 0)
		{
			string_file.clear();
			continue;
		}
		while (isspace(string_file.back()))
			string_file.resize(string_file.size() - 1);
		if (string_file.find('{')  != std::string::npos)
			count++;
		if (string_file.find('}') != std::string::npos)
			count--;
		if(string_file.back() != ';' && string_file.back() != '}' && string_file.back() != '{')
		{
			std::cout << RED << "Error: config file: line must finnish with ;, { or }\n" << RESET;
			std::cout << RED << "line: " << string_file << "\n" << RESET;
			exit(1);
		}
		if(string_file.back() == ';')
			string_file.resize(string_file.size() - 1);
		vector_string.push_back(string_file);
		if (count == 0)
		{
			_allblock.push_back(vector_string);
			vector_string.clear();
		}
		string_file.clear();
	}
	if(count != 0)
	{
		std::cout << RED << "Error: config file: bracket numbers not matching\n" << RESET;
		exit(1);
	}
}

void    Parser::get_info(std::vector<std::string> vector_string, Config &config)
{
	std::string	info;
	std::vector<std::string> vector_info;
	
	for(std::size_t i = 0; i != vector_string.size() && vector_string.at(i) != "}"; i++)
	{
		info = vector_string.at(i);
		
		if (info.find("#") != std::string::npos)
			info.erase(info.find("#"));
		if (info.find("location ") != std::string::npos)
		{
			vector_info.push_back(info);
			while(info.find("}") == std::string::npos)
			{
				i++;
				info = vector_string.at(i);
				vector_info.push_back(info);
			}
			is_location(vector_info, config);
            vector_info.clear();
		}
		else if (info.find("listen ") != std::string::npos)
			is_listen(info, config);
		else if (info.find("server_name ") != std::string::npos)
			is_server_name(info, config);
		else if (info.find("root ") != std::string::npos)
			is_root(info, config);
        else if (info.find("autoindex ") != std::string::npos)
			is_autoindex(info, config);
		else if (info.find("index ") != std::string::npos)
			is_index(info, config);
		else if (info.find("error_page ") != std ::string::npos)
			is_error_page(info, config);
		else if (info.find("client_max_body_size ") != std::string::npos)
			is_client_max_body_size(info, config);
		else if (info.find("cgi_pass ") != std::string::npos)
			is_fastcgi_param(info, config);
		else if (info.find("alias ") != std::string::npos)
			is_alias(info, config);
		else if (info.find("allow_methods ") != std::string::npos)
			is_allow_methods(info, config);
		else if (info.find("return ") != std ::string::npos)
			is_return(info, config);
        info.clear();
	}
}

void	Parser::init_config_file(void)
{
	_config_file.get_root().clear();
	_config_file.get_server_name().clear();
	_config_file.get_index().clear();
	_config_file.set_autoindex(false);
	_config_file.set_client_max_body_size(0);
    _config_file.get_cgi_pass().clear();
    _config_file.get_cgi_pass().push_back("cgi_bin");
	_config_file.get_alias().clear();
	_config_file.get_location().clear();
	_config_file.get_host().s_addr = inet_addr("0.0.0.0");
	_config_file.set_port(-1);
	_config_file.get_host_name().clear();
	_config_file.get_methods().clear();
	_config_file.get_return().clear();
}

void    Parser::parse(void)
{
	init_vector_string();
	for(std::size_t i = 0; i < _allblock.size(); i++)
	{
		init_config_file();
		get_info(_allblock.at(i), _config_file);
		_vector_Config.push_back(_config_file);
	}
}