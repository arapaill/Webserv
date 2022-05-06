#ifndef PARSER_HPP
# define PARSER_HPP

# include "Config.hpp"
# include <vector>

class Parser
{
    public:
        Parser(void);
		Parser(Parser const & src);
		~Parser(void);

        Parser & operator=(const Parser &rhs);
        
        void    init_vector_file(void) // while loop that put .conf file in vector
        void    parse(void);

    private:
        Config  _config_file;
        std::string _path_file;
        std::vector<std::string> _vector_file; //the while .conf file in a vector
};

#endif