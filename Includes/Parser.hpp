#ifndef PARSER_HPP
# define PARSER_HPP

# include "Config.hpp"
# include <vector>
#include <iostream>
#include <fstream>

class Parser
{
    public:
        Parser(void);
		Parser(Parser const & src);
		~Parser(void);

        Parser & operator=(const Parser &rhs);
        
        void    init_vector_file(void); // while loop that put .conf file in vector
        void    parse(void);
        void    get_info(void);

        void    is_listen(std::string info);
        

    private:
        Config  _config_file;
        std::string _path_file;
        std::vector<std::string> _vector_file; //.conf file in a vector
};

#endif