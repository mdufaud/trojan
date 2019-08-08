/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_loop.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdufaud <mdufaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/17 17:46:18 by mdufaud           #+#    #+#             */
/*   Updated: 2018/05/18 12:28:00 by mdufaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "durex.h"

static void check_fd(t_durex *durex, int ret)
{
	unsigned int		i;

	i = 0;
	while ((i < durex->serv.max_fd) && (ret > 0))
	{
        if (durex->clients[i].shell_pid == 0)
        {
            if (FD_ISSET(i, &durex->serv.fd_read))
                do_read(durex, i);
            if (FD_ISSET(i, &durex->serv.fd_write))
                do_write(durex, i);
            if (FD_ISSET(i, &durex->serv.fd_write) || FD_ISSET(i, &durex->serv.fd_read))
                --ret;
        }
		i++;
	}
}

static int	do_select(t_durex *durex, unsigned int max)
{
	int		ret;

	ret = select(max + 1, &durex->serv.fd_read, &durex->serv.fd_write, NULL, NULL);
	return (ret);
}

static int	init_archive(t_durex *durex)
{
	unsigned int		i;
	unsigned int		max;

	i = 0;
	max = 0;
	FD_ZERO(&durex->serv.fd_read);
	FD_ZERO(&durex->serv.fd_write);
	while (i < durex->serv.max_fd)
	{
		if (durex->clients[i].type != FD_FREE)
		{
			FD_SET(i, &durex->serv.fd_read);
			if (durex->clients[i].type == FD_CLIENT)
			{
				if (durex->clients[i].len > 0)
					FD_SET(i, &durex->serv.fd_write);
			}
			max = (max > i) ? max : i;
		}
		i++;
	}
	return (max);
}

void        server_loop(t_durex *durex)
{
	int				ret;
	unsigned int	max;

	while (1)
	{
		max = init_archive(durex);
		ret = do_select(durex, max);
		check_fd(durex, ret);
	}
}
