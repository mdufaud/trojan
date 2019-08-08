/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_system.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdufaud <mdufaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/17 17:38:17 by mdufaud           #+#    #+#             */
/*   Updated: 2018/09/19 16:45:01 by mdufaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "durex.h"
#include <sys/types.h>
#include <sys/wait.h>

static void	exec(const char *s)
{
	char		**tab;

	if ((tab = ft_strsplit(s, ' ')))
    {
	    execve(tab[0], tab, NULL);
	    ft_tabfree(&tab);
    }
	exit(1);
}

int			my_system(const char *s)
{
	pid_t	pid;
	int		err;

	err = 0;
	if ((pid = fork()) == -1)
		return (-1);
	if (!pid)
		exec(s);
	else
		waitpid(pid, &err, 0);
	return (err);
}
