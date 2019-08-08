/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_client.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdufaud <mdufaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/17 18:39:54 by mdufaud           #+#    #+#             */
/*   Updated: 2018/05/20 19:07:26 by mdufaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "durex.h"
#include <signal.h>
#include <sys/file.h>
#include <sys/socket.h>

void		remove_client(t_durex *durex, int socket)
{
	char	*s;

	ft_sprintf(&s, "Client #%d - disconnected\n", socket);
	log_info(durex, s);
	free(s);
    if (durex->clients[socket].shell_pid > 0)
        kill(durex->clients[socket].shell_pid, SIGKILL);
    durex->clients[socket].shell_pid = 0;
	durex->clients[socket].type = FD_FREE;
	ft_bzero(durex->clients[socket].buf, BUFF_SIZE);
	durex->clients[socket].len = 0;
	durex->serv.connected--;
	shutdown(socket, SHUT_RDWR); 
	close(socket);
}
