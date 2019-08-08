/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_write.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdufaud <mdufaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/17 18:32:30 by mdufaud           #+#    #+#             */
/*   Updated: 2018/05/20 18:25:58 by mdufaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "durex.h"
#include <sys/socket.h>

static void	remove_buf(t_durex *durex, int socket, int ret)
{
	if (ret < (int)durex->clients[socket].len)
	{
		ft_memmove(durex->clients[socket].buf, durex->clients[socket].buf + ret,
				durex->clients[socket].len - ret);
		durex->clients[socket].len -= ret;
	}
	else
	{
		durex->clients[socket].buf[0] = 0;
		durex->clients[socket].len = 0;
	}
}

static void	debug_write(t_durex *durex, int co, char *buf, int ret)
{
	int		nl;

	if (durex->debug)
	{
		nl = 0;
		if (buf[ret - 1] == '\n')
		{
			nl = 1;
			buf[ret - 1] = 0;
		}
		ft_printf("To client[%d] - (sent %d): %s\n", co, ret, buf);
		if (nl)
			buf[ret - 1] = '\n';
	}
}

void		do_write(t_durex *durex, int socket)
{
	int		ret;

	if (durex->clients[socket].type != FD_CLIENT)
		return ;
	ret = send(socket, durex->clients[socket].buf,
			durex->clients[socket].len, 0);
	debug_write(durex, socket, durex->clients[socket].buf, ret);
	if (ret <= 0)
		remove_client(durex, socket);
	else
		remove_buf(durex, socket, ret);
}
