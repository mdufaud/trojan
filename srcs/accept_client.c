/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   accept_client.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdufaud <mdufaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/20 19:02:56 by mdufaud           #+#    #+#             */
/*   Updated: 2018/05/20 20:54:33 by mdufaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "durex.h"
#include <netinet/in.h>
#include <arpa/inet.h>

static void	too_much(t_durex *durex, int co)
{
	send(co, "Too many clients connected\n",
			sizeof("Too many clients connected\n"), 0);
	remove_client(durex, co);
	log_info(durex, "Removed because more than 3 clients connected\n");
}

static void	prepare_client(t_durex *durex, int socket)
{
	ft_bzero(&durex->clients[socket], sizeof(t_client));
	durex->serv.connected++;
	durex->clients[socket].step = STEP_PASSWD;
	durex->clients[socket].type = FD_CLIENT;
}

void		accept_client(t_durex *durex, int co)
{
	int					socket;
	socklen_t			len;
	struct sockaddr_in	csin;
	char				*s;

	len = sizeof(csin);
	if ((socket = accept(co, (struct sockaddr *)&csin, &len)) < 0)
	{
		log_info(durex, "Failed to connect client\n");
		return ;
	}
	prepare_client(durex, socket);
	durex->clients[socket].addr = inet_ntoa(csin.sin_addr);
	durex->clients[socket].port = ntohs(csin.sin_port);
	ft_sprintf(&s, "New client #%d from %s:%d\n", socket,
			durex->clients[socket].addr, durex->clients[socket].port);
	log_info(durex, s);
	ft_strdel(&s);
	if (durex->serv.connected > 3)
		too_much(durex, socket);
	else
		message_client(durex, socket, "Please enter password\n");
}
