#ifndef PARSER_HPP
# define PARSER_HPP

# include "Config.hpp"

class Parser
{
    public:
        Parser(void);
		Parser(Parser const & src);
		~Parser(void);

        Parser & operator=(const Parser &rhs);

    private:
        Config  _config_file;
        std::string _path_file;
};

#endif