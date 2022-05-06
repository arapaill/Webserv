#include "../Includes/Parser.hpp"

#include <iostream>
#include <fstream>

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

void    init_vector_file(void)
{
    std::ifstream file;
    std::string string_file;

    file.open(../Configs/config.conf);
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

void    parse(void)
{
   init_vector_file();
   
}