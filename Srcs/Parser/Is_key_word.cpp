#include "../../Includes/headers.hpp"
#include "Parser.hpp"

std::vector<std::string> split(const std::string & s, char separator)
{
	std::vector<std::string>    ret;
	std::string::size_type      prev_pos = 0, pos = 0;

    while ((pos = s.find(separator, pos)) != std::string::npos)
    {
        std::string substring = s.substr(prev_pos, pos-prev_pos);
        ret.push_back(substring);

        prev_pos = ++pos;
    }
    std::string last = s.substr(prev_pos);
    if (last.size() > 0 && isspace(last.back()))
        last.resize(last.size() - 1);
    ret.push_back(last);
    return (ret);
}

bool isNumber(const std::string& str)
{
    const char *c = str.c_str();

    for (int i = 0; c[i] != '\0'; i++) {
        if (std::isdigit(c[i]) == 0) return false;
    }
    return true;
}

bool isText(const std::string& str)
{
    const char *c = str.c_str();

    for (int i = 0; c[i] != '\0'; i++) {
        if (std::isalpha(c[i]) == 0) return false;
    }
    return true;
}

void    Parser::is_listen(std::string info, Config &config)
{
	std::vector<std::string> cmd;
	std::string tmp;
    std::string	host;

	cmd = split(info, ' ');
    if(cmd.size() < 2)
    {
        std::cout << RED << "not enough directives for network\n" << RESET;
        exit(1);
    }
	for(std::size_t i = 0; i != cmd.size(); i++)
	{
		tmp = cmd.at(i);
        
		if(isNumber(tmp) && config.get_port() == -1)
        {
			config.set_port(atoi(tmp.c_str()));
        }
        if((tmp.find(":")) == std::string::npos &&
        ( tmp.find(".") != std::string::npos || tmp == "localhost"))
        {
            if (tmp == "localhost")
			    host = "127.0.0.1";
            else
			    host = tmp;
            config.set_host_name(host);
            config.get_host().s_addr = inet_addr(host.c_str());
        }
        tmp.clear();
	}
}

void    Parser::is_server_name(std::string info, Config &config)
{
    std::vector<std::string> cmd;
    std::string tmp;
    cmd = split(info, ' ');

    if(cmd.size() < 2)
    {
        std::cout << RED << "not enough directives for server_name\n" << RESET;
        exit(1);
    }
    if(cmd.size() > 3)
    {
        std::cout << RED << "too much directives for server_name\n" << RESET;
        exit(1);
    }
    if (cmd.size() == 3)
        tmp =  cmd.at(1) + " " + cmd.at(2);
    else
        tmp = cmd.at(1);
    
    config.set_server_name(tmp);
}

void    Parser::is_root(std::string info, Config &config)
{
    std::vector<std::string> cmd;
    std::string tmp;
    cmd = split(info, ' ');

    if(cmd.size() < 2)
    {
        std::cout << RED << "not enough directives for root\n" << RESET;
        exit(1);
    }
    if(cmd.size() > 2)
    {
        std::cout << RED << "too much directives for root\n" << RESET;
        exit(1);
    }
    if (cmd.at(1)[0] != '.')
        tmp = '.' + cmd.at(1);
    config.set_root(tmp);
}

void    Parser::is_index(std::string info, Config &config)
{
    std::vector<std::string> cmd;
    std::string tmp;
    cmd = split(info, ' ');

    if(cmd.size() < 2)
    {
        std::cout << RED << "not enough directives for index\n" << RESET;
        exit(1);
    }
    if(cmd.size() > 2)
    {
        std::cout << RED << "too much directives for index\n" << RESET;
        exit(1);
    }
    config.set_index(cmd.at(1));
}

void    Parser::is_autoindex(std::string info, Config &config)
{
    std::vector<std::string> cmd;
    cmd = split(info, ' ');

    if(cmd.size() < 2)
    {
       std::cout << RED << "not enough directives for autoindex\n" << RESET;
        exit(1);
    }
    if(cmd.size() > 2)
    {
        std::cout << RED << "too much directives for autoindex\n" << RESET;
        exit(1);
    }
    if(cmd.at(1) == "on")
        config.set_autoindex(true);
    else if(cmd.at(1) == "off")
        config.set_autoindex(false);
    else
    {
        std::cout << RED << "Error: .conf file: autoindex syntax error\n usage: autoindex on; or autoindex off\n" << RESET;
        exit(1);
    }
}

void    Parser::is_client_max_body_size(std::string info, Config &config)
{
    std::vector<std::string> cmd;
    cmd = split(info, ' ');
    if(cmd.size() < 2)
    {
        std::cout << RED << "not enough directives for clien max body size\n" << RESET;
        exit(1);
    }
    if(cmd.size() > 2)
    {
        std::cout << RED << "too much directives for client max body size\n" << RESET;
        exit(1);
    }
    config.set_client_max_body_size(atoi(cmd.at(1).c_str()));
}

void    Parser::is_error_page(std::string info, Config &config)
{
    std::vector<std::string> cmd;
    cmd = split(info, ' ');

    if(cmd.size() < 3)
    {
        std::cout << RED << "not enough directives for error page\n" << RESET;
        exit(1);
    }
    if(cmd.size() > 3)
    {
        std::cout << RED << "too much directives for error page\n" << RESET;
        exit(1);
    }
    config.get_error_page()[atoi(cmd.at(1).c_str())] = cmd.at(2);
}

void    Parser::is_return(std::string info, Config &config)
{
    std::vector<std::string> cmd;
    cmd = split(info, ' ');

    if(cmd.size() < 3)
    {
        std::cout << RED << "not enough directives for return\n" << RESET;
        exit(1);
    }
    if(cmd.size() > 3)
    {
        std::cout << RED << "too much directives for return\n" << RESET;
        exit(1);
    }
    config.get_return()[atoi(cmd.at(1).c_str())] = cmd.at(2);
}

void    Parser::is_fastcgi_param(std::string info, Config &config)
{
    std::vector<std::string> cmd;
    cmd = split(info, ' ');

    if(cmd.size() < 2)
    {
        std::cout << RED << "not enough directives for fastcgi_param\n" << RESET;
        exit(1);
    }
    if(cmd.size() > 2)
    {
        std::cout << RED << "too much directives for fastcgi_param\n" << RESET;
        exit(1);
    }
    config.set_cgi_pass(cmd.at(1));
}

void    Parser::is_alias(std::string info, Config &config)
{
    std::vector<std::string> cmd;
    cmd = split(info, ' ');

    if(cmd.size() < 2)
    {
        std::cout << RED << "not enough directives for alias\n" << RESET;
        exit(1);
    }
    config.set_alias(cmd.at(1));
}

void    Parser::is_allow_methods(std::string info, Config &config)
{
    std::vector<std::string> cmd;
    cmd = split(info, ' ');

    for(int i = 1; i != cmd.size(); i++)
        config.get_methods().push_back(cmd.at(i));
}

void    Parser::is_location(std::vector<std::string> info, Config &config)
{
    std::string location_name;
    std::vector<std::string> cmd;
    Config location_config;

    cmd = split(info.at(0), ' ');
    location_name = cmd.at(1);
    info.erase(info.begin());
    get_info(info, location_config);
    config.get_location()[location_name] = location_config;
}