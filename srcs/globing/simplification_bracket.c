/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simplification_bracket.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-rest <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/25 16:01:41 by sle-rest          #+#    #+#             */
/*   Updated: 2018/09/15 18:16:29 by sle-rest         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "globing.h"
#include "ft_str.h"

static int	index_end_globacc(char *str)
{
	int	i;
	int	bracket;

	i = detect_globacc(str) + 1;
	bracket = 1;
	while (1)
	{
		bracket = str[i] == '{' && ft_no_backslash(str, i) ?
								bracket + 1 : bracket;
		bracket = str[i] == '}' && ft_no_backslash(str, i) ?
								bracket - 1 : bracket;
		if ((str[i] == '}' && ft_no_backslash(str, i)) && !bracket)
			break ;
		i++;
	}
	return (++i);
}

static int	do_simplification_split(t_acc *acc, char **new_split,
										int *i, int *j)
{
	int	x;
	int	y;

	x = detect_globacc(acc->split[*i]) + 1;
	y = index_end_globacc(acc->split[*i]);
	while (acc->split[*i][x] != '}' && ft_no_backslash(acc->split[*i], *j))
	{
		if (!(new_split[*j] = get_elemacc_simplified(acc->split[*i], &x, y)))
			return (0);
		(*j)++;
	}
	while (acc->split[++(*i)])
	{
		if (!(new_split[*j] = ft_strdup(acc->split[*i])))
			return (0);
		(*j)++;
	}
	new_split[*j] = NULL;
	return (1);
}

static int	do_simplification(t_acc *acc, int i)
{
	char	**new_split;
	int		len;
	int		j;

	len = get_len_simplification(acc, i, detect_globacc(acc->split[i]) + 1);
	len = len + (int)ft_tabsize(acc->split) - 1;
	if (!(new_split = (char **)malloc(sizeof(char *) * (len + 1))))
		return (0);
	j = -1;
	while (++j < i)
	{
		if (!(new_split[j] = ft_strdup(acc->split[j])))
			return (0);
	}
	if (!do_simplification_split(acc, new_split, &i, &j))
		return (0);
	ft_free_tab(&(acc->split));
	acc->split = new_split;
	return (1);
}

int			simplification_bracket(t_acc *acc)
{
	int	i;

	while (acc)
	{
		i = 0;
		while (acc->type == accolade && acc->split[i])
		{
			if (detect_globacc(acc->split[i]) != -1)
			{
				if (!do_simplification(acc, i))
					return (0);
				i = -1;
			}
			i++;
		}
		acc = acc->next;
	}
	return (1);
}
