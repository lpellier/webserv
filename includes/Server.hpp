/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dait-atm <dait-atm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/03 04:55:39 by dait-atm          #+#    #+#             */
/*   Updated: 2022/03/03 17:32:39 by dait-atm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"

# define BACK_LOG				10
# define REQUEST_BUFFER_SIZE	1024

# define TIMEOUT				10 * 1000

class ServerExeption : public std::exception
{
public:
	virtual const char* what() const throw ()
	{
		return ("CHILD TERMINATED BEFORE EXECVE");
	}
};

// typedef void (*ScriptFunction)(::Client &) const;

/**
 * @brief a http/1.1 server working for one config
 */
class Server // * ______________________________________________________________
{
	/// * Variables ____________________________________________________________
private:
	int								_listen_sd;
	std::vector<struct pollfd>		_fds;
	std::map<int, Client>			_clients;
	ResponseGenerator				_response_generator;

	std::map<int, int>				_listeners;		// ? key : client id, value : function to trigger

public:
	Conf							_conf;
	std::vector<Conf>				_confs;

	/// * Constructors & Destructors ___________________________________________
public:
	// ? default (1)
	Server ();

	// ? Constructor (2) taking a config
	Server (const Conf &c);

	// ? Constructor (3) by copy
	Server (const Server &other);

	~Server ();

	// * Member functions ______________________________________________________
	
	Server	&operator= (const Server &rhs);

	// ? init the server with a conf_object (for now just a port number).
	// ? this will allow us to reload the server or set it up after using the default constructor.
	int		init (const Conf & c);

	// ? Server's main loop
	int		start ();

private:
	bool	socket_bind ();

	void	set_client_event_to_flag (int client_id, int flag);

	bool	server_poll_loop ();

	bool	is_client_fd (const size_t i) const;

	bool	add_new_client ();

	void	add_cgi_listener (const size_t i);

	int		pipe_to_client (int fd);

	bool	record_client_input (const size_t &i);

	bool	check_timed_out_client (const size_t i);

	int		remove_client (size_t i);

	void	squeeze_fds_array ();

	int		get_client_position (int client_key) const;

}; // * ________________________________________________________________________

