/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_termdel.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdufaud <mdufaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/23 18:47:47 by mdufaud           #+#    #+#             */
/*   Updated: 2016/11/27 16:44:11 by mdufaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_curses.h"
#include <stdlib.h>

void	ft_termdel(t_term **term)
{
	ft_strdel(&(*term)->buffer);
	if ((*term)->prompt)
		ft_strdel(&(*term)->prompt);
	if ((*term)->hist)
		ft_dlstdel(&(*term)->hist, NULL);
	if ((*term)->comp)
		ft_dlstdel(&(*term)->comp, NULL);
	if ((*term)->cmds)
		ft_dlstdel(&(*term)->cmds, NULL);
	if ((*term)->options)
		ft_hashfree(&(*term)->options);
	ft_bzero(*term, sizeof(t_term));
	free(*term);
	*term = NULL;
	ft_termexit();
}
