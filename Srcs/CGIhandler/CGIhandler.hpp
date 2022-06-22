/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIhandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jandre <jandre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 09:19:04 by jandre            #+#    #+#             */
/*   Updated: 2022/06/22 16:00:34 by jandre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIHANDLER_HPP
# define CGIHANDLER_HPP

# include "../RequestHTTP/RequestHTTP.hpp"

class CGIhandler {
	private:
		std::string							_path;
		RequestHTTP							_request;
		std::string							_body;
		std::map<std::string, std::string>	_env;
		Config								_config;

	public:
		//CONSTRUCTOR DESTRUCTOR COPY
		CGIhandler(RequestHTTP request, Config config, std::string path);
		CGIhandler(CGIhandler const & cpy);
		~CGIhandler();
		CGIhandler & operator=(CGIhandler const & rhs);
		
		//MEMBER FUNCTION
		void			init_env();
		std::string		& get_body();
		void			execute_CGI();
		char			**get_env_as_char_array();
		int				get_status_code();
};

#endif
