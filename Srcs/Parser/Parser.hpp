#ifndef PARSER_HPP
# define PARSER_HPP

# include "../../Includes/headers.hpp"
# include "../../Includes/Config.hpp"

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
        void    is_autoindex(std::string info);
        void    is_client_max_body_size(std::string info);
        
        Config  &get_config_file(void){return(_config_file);};
        std::vector<Config> &get_vector_config(void) {return(_vector_Config);};
    private:
        Config  _config_file;
        std::string _path_file;
         //.conf file in a vector: 1 block
        std::vector<std::vector<std::string>> _allblock;;
        std::vector<Config> _vector_Config;
};

#endif