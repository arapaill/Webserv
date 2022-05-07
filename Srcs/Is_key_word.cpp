#include "../Includes/Parser.hpp"
#include <algorithm>

std::vector<std::string> split(const std::string& s, char seperator)
{
	std::vector<std::string> output;
	std::string::size_type prev_pos = 0, pos = 0;

    while((pos = s.find(seperator, pos)) != std::string::npos)
    {
        std::string substring( s.substr(prev_pos, pos-prev_pos) );

        output.push_back(substring);

        prev_pos = ++pos;
    }
    output.push_back(s.substr(prev_pos, pos-prev_pos)); // Last word
    return output;
}

bool isNumber(const std::string& str)
{
    for (char const &c : str) {
        if (std::isdigit(c) == 0) return false;
    }
    return true;
}

void    Parser::is_listen(std::string info)
{
	std::vector<std::string> cmd;
	std::string tmp;
    std::string	host;
    std::string address;

	cmd = split(info, ' ');
    address = cmd.at(1);
	for(std::size_t i = 0; i != _vector_file.size(); i++)
	{
		tmp = _vector_file.at(i);
		if(isNumber(tmp) && !_config_file.get_network().get_port())
			_config_file.get_network().get_port() = atoi(tmp.c_str());
        if((address.find(":")) == std::string::npos &&
        ( address.find(".") != std::string::npos || address == "localhost"))
        {
            if (address == "localhost")
			    host = "127.0.0.1";
            else
			    host = address;
            _config_file.get_network().get_host().s_addr = inet_addr(host.c_str());
        }
        
	}
    if(!_config_file.get_network().get_port())
			_config_file.get_network().get_port() = 80;
	if(!_config_file.get_network().get_host().s_addr)
		_config_file.get_network().get_host().s_addr = inet_addr("0.0.0.0");
}