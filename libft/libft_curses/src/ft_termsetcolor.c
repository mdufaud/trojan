/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_termsetcolor.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdufaud <mdufaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/26 16:49:17 by mdufaud           #+#    #+#             */
/*   Updated: 2016/10/25 17:31:04 by mdufaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_curses.h"

void	ft_termsetcolor(t_term *term, int opt, t_color color)
{
	if ((opt & FT_TERM_PROMPT))
		term->prompt_color = color;
	if ((opt & FT_TERM_STR))
		term->str_color = color;
}
