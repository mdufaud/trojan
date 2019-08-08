/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_kill.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdufaud <mdufaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/17 17:38:17 by mdufaud           #+#    #+#             */
/*   Updated: 2018/05/20 21:45:01 by mdufaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "durex.h"
#include <sys/types.h>
#include <signal.h>

static char			*get_pid(char *process)
{
	char	result[40 + 1];
	FILE	*pipe;
	char	*cmd;

	if ((cmd = ft_strjoin("/usr/bin/pgrep -f ", process)) == NULL)
        return (NULL);
	if (!(pipe = popen(cmd, "r")))
		return (NULL);
	ft_bzero(result, 40);
	fgets(result, 40, pipe);
	free(cmd);
	pclose(pipe);
	return (ft_strdup(result));
}

int 				my_kill(char *process, int sig)
{
	char	*pid_str;
	char	**pids;
	pid_t	pid;
	int		i;

	if ((pid_str = get_pid(process)) == NULL)
        return (1);
	if ((pids = ft_strsplit(pid_str, ' ')) == NULL)
        return (1);
	i = 0;
	while (pids[i])
	{
		pid = ft_atoi(pids[i]);
		if (pid > 0)
			kill(pid, sig);
		free(pids[i]);
		i++;
	}
	free(pids);
	free(pid_str);
    return (0);
}
