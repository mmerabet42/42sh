/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/16 11:03:26 by gdufay            #+#    #+#             */
/*   Updated: 2018/10/24 16:54:53 by sle-rest         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libedit.h"

static int	count_backslash(char *start, char *s)
{
	int		i;

	if (!start || !s)
		return (0);
	i = 0;
	while (s != start && *s == '\\')
		if (*s-- == '\\')
			i++;
	return (i);
}

static int	check_quote_split(char *tmp, int i)
{
	int	counter;

	counter = 0;
	if ((i = ft_strlen(tmp) - 1) < 0)
		return (0);
	if (!i && tmp[i] == '\\')
		return ('\\');
	while (i && tmp[i--] == '\\')
		counter++;
	return (counter % 2 ? '\\' : 0);
}

int			ft_check_quote(char *s)
{
	int		i;
	char	*tmp;

	if (!s)
		return (0);
	i = -1;
	tmp = s;
	while (tmp[++i])
		if (tmp[i] == '"' || tmp[i] == 39 || tmp[i] == '`')
		{
			if (!ft_strchr(&tmp[i + 1], tmp[i]))
				return (tmp[i]);
			tmp = ft_strchr(&tmp[i + 1], tmp[i]);
			if (*tmp == '"' && *(tmp - 1) == '\\'
					&& (count_backslash(s, tmp - 1) & 1))
				tmp--;
			i = 0;
		}
	return (check_quote_split(tmp, i));
}
