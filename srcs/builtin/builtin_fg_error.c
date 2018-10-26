/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_fg_error.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jraymond <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/19 16:24:28 by jraymond          #+#    #+#             */
/*   Updated: 2018/10/26 17:10:47 by jraymond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_io.h"
#include "ft_list.h"

t_list		*error_fg(char **argv, int i)
{
	if (!i)
		ft_printf_fd(2, "42sh: fg: %s: no such job\n", argv[1]);
	else
		ft_putstr_fd("42sh: fg: current: no such job\n", 2);
	return (NULL);
}
