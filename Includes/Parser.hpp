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
        
        void    init_vector_file(void)
        void    parse(void);

    private:
        Config  _config_file;
        std::string _path_file;
        std::vector<std::string> _vector_file;
};

#endif