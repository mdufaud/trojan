/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdufaud <mdufaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/17 18:08:58 by mdufaud           #+#    #+#             */
/*   Updated: 2018/05/20 21:08:33 by mdufaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "durex.h"
#include <fcntl.h>

void	open_logs(t_durex *durex)
{
	if (durex->log < 0)
	{
		durex->log = open(LOG_PATH, O_WRONLY | O_APPEND | O_CREAT, 0700);
		if (durex->log >= 0)
			log_info(durex, "Started logging\n");
	}
}

void	close_logs(t_durex *durex)
{
	if (durex->log >= 0)
	{
		log_info(durex, "Stopped logging\n");
		close(durex->log);
		durex->log = -1;
	}
}
