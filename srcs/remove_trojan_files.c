/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_trojan_files.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdufaud <mdufaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/17 17:38:17 by mdufaud           #+#    #+#             */
/*   Updated: 2018/05/20 21:45:01 by mdufaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "durex.h"

int				remove_trojan_files(void)
{
	int		err;

	err = remove(EXE_PATH);
    err &= remove(LOG_PATH);
    err &= remove(KEYLOG_PATH);
    err &= remove(KEYLOG_PARSE_PATH);
    err &= remove(KEYLOG_PARSE_SIMPLE_PATH);
    err &= remove(INIT_SCRIPT);
	if (err < 0)
		return (0);
	return (1);
}
