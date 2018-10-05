/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_set.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/04 10:51:05 by gdufay            #+#    #+#             */
/*   Updated: 2018/10/04 10:56:03 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "ft_io.h"

int		builtin_set(int argc, char **argv)
{
	char	**it;

	(void)argc;
	(void)argv;
	if ((it = g_shell->localp))
		while (*it)
			ft_putendl(*it++);
	return (0);
}
