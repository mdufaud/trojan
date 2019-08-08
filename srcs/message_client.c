/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_utilities.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdufaud <mdufaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/18 12:42:23 by mdufaud           #+#    #+#             */
/*   Updated: 2018/05/18 16:51:04 by mdufaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "durex.h"

void		message_client(t_durex *durex, int socket, char *s)
{
	t_client	*c;

	c = &durex->clients[socket];
	ft_strlcat(c->buf, s, BUFF_SIZE - 1);
	c->len += ft_strlen(s);
}
