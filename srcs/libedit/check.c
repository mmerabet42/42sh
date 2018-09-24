/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/16 11:03:26 by gdufay            #+#    #+#             */
/*   Updated: 2018/06/11 14:21:58 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libedit.h"

int		ft_check_quote(char *s)
{
	int		i;
	char	*tmp;

	if (!s)
		return (1);
	i = -1;
	tmp = s;
	while (tmp[++i])
		if (tmp[i] == '"' || tmp[i] == 39 || tmp[i] == '`')
		{
			if (!ft_strchr(&tmp[i + 1], tmp[i]))
				return (0);
			tmp = ft_strchr(&tmp[i + 1], tmp[i]);
			i = 0;
		}
	return (1);
}
