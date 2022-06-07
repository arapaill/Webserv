#include "../../Includes/headers.hpp"
#include "Parser.hpp"

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

bool isText(const std::string& str)
{
    for (char const &c : str) {
        if (std::isalpha(c) == 0) return false;
    }
    return true;
}

void    Parser::is_listen(std::string info)
{
	std::vector<std::string> cmd;
	std::string tmp;
    std::string	host;

	cmd = split(info, ' ');
    if(cmd.size() < 3)
    {
        std::cout << RED << "not enough directives for network\n" << RESET;
        exit(1);
    }
	for(std::size_t i = 0; i != cmd.size(); i++)
	{
		tmp = cmd.at(i);
        
       //std::cout << "TMP:" << tmp << std::endl;
		if(isNumber(tmp) && _config_file.get_port() == -1)
        {
			_config_file.set_port(atoi(tmp.c_str()));
        }
        if((tmp.find(":")) == std::string::npos &&
        ( tmp.find(".") != std::string::npos || tmp == "localhost"))
        {
            if (tmp == "localhost")
			    host = "127.0.0.1";
            else
			    host = tmp;
            _config_file.set_host_name(host);
            //std::cout << "HOST: " << host.c_str() << std::endl;
            _config_file.get_host().s_addr = inet_addr(host.c_str());
           // std::cout << _config_file.get_network().get_host().s_addr << std::endl;
        }
        if(isText(tmp) && tmp != "listen")
        {
           // std::cout << "TMP:" << tmp << std::endl;
            _config_file.set_host_name(tmp);
            //std::cout << "host_name: " << _config_file.get_network().get_host_name() << std::endl;
        }
        tmp.clear();
	}
}

void    Parser::is_server_name(std::string info)
{
    std::vector<std::string> cmd;
    std::string tmp;
    cmd = split(info, ' ');

    if(cmd.size() < 2)
    {
        std::cout << "not enough directives for server_name\n";
        exit(1);
    }
    if(cmd.size() > 3)
    {
        std::cout << "too much directives for server_name\n";
        exit(1);
    }
    if (cmd.size() == 3)
        tmp =  cmd.at(1) + " " + cmd.at(2);
    else
        tmp = cmd.at(1);
    
    _config_file.set_server_name(tmp);
   
}

void    Parser::is_root(std::string info)
{
    std::vector<std::string> cmd;
    std::string tmp;
    cmd = split(info, ' ');

    if(cmd.size() < 2)
    {
        std::cout << "not enough directives for root\n";
        exit(1);
    }
    if(cmd.size() > 2)
    {
        std::cout << "too much directives for root\n";
        exit(1);
    }
    _config_file.set_root(cmd.at(1));
}

void    Parser::is_index(std::string info)
{
    std::vector<std::string> cmd;
    std::string tmp;
    cmd = split(info, ' ');

    if(cmd.size() < 2)
    {
        std::cout << "not enough directives for index\n";
        exit(1);
    }
    if(cmd.size() > 2)
    {
        std::cout << "too much directives for index\n";
        exit(1);
    }
    _config_file.set_index(cmd.at(1));
}

void    Parser::is_autoindex(std::string info)
{
    std::vector<std::string> cmd;
    cmd = split(info, ' ');

    if(cmd.size() < 2)
    {
        std::cout << "not enough directives for autoindex\n";
        exit(1);
    }
    if(cmd.size() > 2)
    {
        std::cout << "too much directives for autoindex\n";
        exit(1);
    }
    if(cmd.at(1) == "On")
        _config_file.set_autoindex(true);
    else
        _config_file.set_autoindex(false);
}

void    Parser::is_client_max_body_size(std::string info)
{
    std::vector<std::string> cmd;
    cmd = split(info, ' ');
     if(cmd.size() < 2)
    {
        std::cout << "not enough directives for index\n";
        exit(1);
    }
    if(cmd.size() > 2)
    {
        std::cout << "too much directives for index\n";
        exit(1);
    }
    _config_file.set_client_max_body_size(atoi(cmd.at(1).c_str()));
}