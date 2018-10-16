/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/16 11:03:26 by gdufay            #+#    #+#             */
/*   Updated: 2018/10/16 12:22:18 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libedit.h"

int				ft_check_quote(char *s)
{
	int		i;
	char	*tmp;
	int		counter;

	if (!s)
		return (1);
	i = -1;
	tmp = s;
	counter = 0;
	while (tmp[++i])
		if (tmp[i] == '"' || tmp[i] == 39 || tmp[i] == '`')
		{
			if (!ft_strchr(&tmp[i + 1], tmp[i]))
				return (0);
			tmp = ft_strchr(&tmp[i + 1], tmp[i]);
			i = 0;
		}
	if ((i = ft_strlen(s) - 1) < 0)
		return (1);
	while (tmp[i--] == '\\')
		counter++;
	return (counter % 2 ? 0 : 1);
}
