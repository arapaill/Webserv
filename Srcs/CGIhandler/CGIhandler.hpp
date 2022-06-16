/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIhandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jandre <ajuln@hotmail.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 09:19:04 by jandre            #+#    #+#             */
/*   Updated: 2022/06/15 17:20:17 by jandre           ###   ########.fr       */
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
};

#endif
