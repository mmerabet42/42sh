/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_len_simplification.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-rest <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/25 16:25:45 by sle-rest          #+#    #+#             */
/*   Updated: 2018/09/15 18:07:04 by sle-rest         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "globing.h"

static void	len_between_comma_split(char *str, int *len, int *i)
{
	int	bracket;

	bracket = 1;
	while (str[++(*i)])
	{
		(*len)++;
		bracket = str[*i] == '{' && ft_no_backslash(str, *i) ?
								bracket + 1 : bracket;
		bracket = str[*i] == '}' && ft_no_backslash(str, *i) ?
								bracket - 1 : bracket;
		if ((str[*i] == '}' && ft_no_backslash(str, *i)) && !bracket)
			break ;
	}
	(*i)++;
	(*len)++;
}

int			len_between_comma(char *str, int i)
{
	int	len;

	len = 0;
	while ((str[i] != ',' && ft_no_backslash(str, i))
			&& (str[i] != '}' && ft_no_backslash(str, i)))
	{
		if (detect_globbing_at(str, i) == accolade)
			len_between_comma_split(str, &len, &i);
		else
		{
			len++;
			i++;
		}
	}
	return (len);
}

int			get_len_simplification(t_acc *acc, int i, int j)
{
	int	len;

	len = 1;
	while (42)
	{
		j += len_between_comma(acc->split[i], j);
		j = acc->split[i][j] == ',' && ft_no_backslash(acc->split[i], j) ?
											j + 1 : j;
		if (acc->split[i][j] == '}' && ft_no_backslash(acc->split[i], j))
			break ;
		len++;
	}
	return (len);
}
