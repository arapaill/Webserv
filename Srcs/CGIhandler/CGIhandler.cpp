/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIhandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jandre <ajuln@hotmail.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 09:19:08 by jandre            #+#    #+#             */
/*   Updated: 2022/06/15 18:24:53 by jandre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGIhandler.hpp"

//CONSTRUCTOR DESTRUCTOR COPY
CGIhandler::CGIhandler(RequestHTTP request, Config config, std::string path) : _request(request), _config(config), _path(path)
{
    this->init_env();
};

CGIhandler::CGIhandler(CGIhandler const & cpy)
{
	*this = cpy;
};

CGIhandler::~CGIhandler() {};

CGIhandler & CGIhandler::operator=(CGIhandler const & rhs)
{
	this->_path = rhs._path;
	this->_request = rhs._request;
	this->_body = rhs._body;
	this->_env = rhs._env;
	this->_config = rhs._config;
	return (*this);
};

void CGIhandler::init_env()
{
	//port int to port std::string
	std::string 		port;
	std::stringstream 	s;
	s << this->_config.get_port();
	port = s.str();

	//get everything from the path, path will have this form : /path_to_cgi/some_other_directories/cgi_script?query_string
	std::string			path_info;
	std::string			script_name;
	std::string			query_string;

	std::string::size_type pos = this->_path.find('?');
	std::string::size_type pos2 = this->_path.find('/');
	while (pos2 != std::string::npos) //find the last occurence of '/' before the '?' so we can find the script_name
	{
		std::string::size_type tmp = pos2;
		pos2 = this->_path.find('/', tmp);
		if (pos2 == std::string::npos || pos2 > pos)
		{
			if (pos2 < pos)
				script_name = this->_path.substr(pos2, pos);
			else
				script_name = this->_path.substr(tmp, pos);
			break ;
		}
	}
    if (pos != std::string::npos) // take the path before the query string (separated by a '?')
    {
		path_info = this->_path.substr(0, pos);
		query_string = this->_path.substr(pos, std::string::npos);
    }
    else // No Query_string
    {
		path_info = this->_path;
		query_string = "";
    }

	//every MIME type accepted, need to parse all the vector into one string
	std::string content_type = "";
	
	for (std::vector<std::string>::iterator it = this->_request.getAccept().begin(); it != this->_request.getAccept().end(); it++)
	{
		content_type += *it;
		content_type += ", ";
	}
	
    //this->_env["AUTH_TYPE"] = "";											//SHOULD
    this->_env["CONTENT LENGTH"] = "";										//MUST
    this->_env["CONTENT_TYPE"] = content_type;								//MUST
    this->_env["GATEWAY_INTERFACE"] = "CGI/1.1";							//MUST
    //this->_env["HTTP_ACCEPT"] = ;											//MAY
    //this->_env["HTTP_ACCEPT_CHARSET"] = ;									//MAY
    //this->_env["HTTP_ACCEPT_ENCODING"] = ;								//MAY
    //this->_env["HTTP_ACCEPT_LANGUAGE"] = ;								//MAY
    //this->_env["HTTP_COOKIE"] = ;											//MAY
    //this->_env["HTTP_FORWARDED"] = ;										//MAY
    //this->_env["HTTP_HOST"] = ;											//MAY
    //this->_env["HTTP_PROXY_AUTHORIZATION"] = ;							//MAY
    //this->_env["HTTP_USER_AGENT"] = ;										//MAY
    this->_env["PATH_INFO"] = path_info;									//MUST
    this->_env["PATH_TRANSLATED"] = this->_config.get_root() + path_info;	//SHOULD
    this->_env["QUERY_STRING"] = query_string;								//MUST
    this->_env["REDIRECT_STATUS"] = "200";									//MAY
    this->_env["REMOTE_ADDR"] = this->_request.getHost();					//MUST
    //this->_env["REMOTE_HOST"] = "";										//SHOULD
    //this->_env["REMOTE_USER"] = "";										//SHOULD
    this->_env["REQUEST_METHOD"] = this->_request.getMethod();				//MAY
    this->_env["REQUEST_URI"] = this->_path;								//MAY
    this->_env["SCRIPT_NAME"] = script_name;								//MUST
    this->_env["SERVER_NAME"] = this->_config.get_server_name();			//MUST
    this->_env["SERVER_PORT"] = port;										//MUST
    this->_env["SERVER_PROTOCOL"] = "HTTP/1.1";;							//MUST
    this->_env["SERVER_SOFTWARE"] = "webserv19";							//MUST
    //this->_env["SCRIPT_FILENAME"] = ;										//MAY
};

void CGIhandler::execute_CGI()
{

};

std::string & CGIhandler::get_body() { return (this->_body); };

