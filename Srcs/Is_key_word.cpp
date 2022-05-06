#include "../Includes/Parser.hpp"

bool isNumber(const string& s)
{
    return std::ranges::all_of(s.begin(), s.end(),
                  [](char c){ return isdigit(c) != 0; });
}

void    Parser::is_listen(std::string info)
{
	string cmd;

	while(getline(info, cmd, ' '))
	{
		if(isNumber(cmd))
			Parser::_config_file._network._port = cmd;
	}
	
}