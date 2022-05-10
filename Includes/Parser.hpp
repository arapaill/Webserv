#ifndef PARSER_HPP
# define PARSER_HPP

# include "Config.hpp"
# include <vector>
#include <iostream>
#include <fstream>
#include <fstream>
# include <sstream>

class Parser
{
    public:
        Parser(void);
		Parser(Parser const & src);
		~Parser(void);

        Parser & operator=(const Parser &rhs);
        
        void    init_vector_string(void); // while loop that put .conf file in vector
        void	init_config_file(void);
        void    parse(void);
        void    get_info(std::vector<std::string> vector_string);

        void    is_listen(std::string info);
        void    is_server_name(std::string info);
        void    is_root(std::string info);
        void    is_index(std::string info);
        
        Config  &get_config_file(void){return(_config_file);};
    private:
        Config  _config_file;
        std::string _path_file;
         //.conf file in a vector: 1 block
        std::vector<std::vector<std::string>> _allblock;;
        std::vector<Config> _vector_Config;
};

#endif