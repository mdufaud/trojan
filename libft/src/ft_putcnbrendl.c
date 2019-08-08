/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putcnbrendl.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdufaud <mdufaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/01/23 19:02:05 by mdufaud           #+#    #+#             */
/*   Updated: 2014/05/07 10:56:43 by mdufaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putcnbrendl(int n, t_color color)
{
	ft_put_color(color);
	ft_putnbr(n);
	ft_end_color();
	ft_putchar('\n');
}
