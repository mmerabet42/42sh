/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_split_acc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-rest <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/22 17:08:14 by sle-rest          #+#    #+#             */
/*   Updated: 2018/10/26 19:51:09 by sle-rest         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "globing.h"

static void	bracket_nb_token_in_acc(char *str, int *i)
{
	int	bracket;

	bracket = 1;
	while (str[++(*i)])
	{
		bracket = str[(*i)] == '{' && ft_no_backslash(str, *i) ?
								bracket + 1 : bracket;
		bracket = str[(*i)] == '}' && ft_no_backslash(str, *i) ?
								bracket - 1 : bracket;
		if ((str[(*i)] == '}' && ft_no_backslash(str, *i)) && !bracket)
			break ;
	}
	(*i)++;
}

static int	nb_token_in_acc(char *str, int i)
{
	int	count;

	count = 1;
	while (str[i] && (str[i] != '}' && ft_no_backslash(str, i)))
	{
		if (detect_globbing_at(str, i) == accolade)
			bracket_nb_token_in_acc(str, &i);
		else if (str[i] == ',' && ft_no_backslash(str, i))
		{
			i++;
			count++;
		}
		else
			i++;
	}
	return (count);
}

static int	fill_between_comma(char *str, int *i, int j, t_acc **acc)
{
	int	len;
	int	x;

	len = 0;
	len = len_between_comma(str, *i);
	if (!((*acc)->split[j] = (char *)malloc(sizeof(char) * (len + 1))))
		return (error_glob("mg", "42sh: error_glob malloc"));
	x = 0;
	while (len > 0)
	{
		(*acc)->split[j][x] = str[*i];
		x++;
		(*i)++;
		len--;
	}
	(*acc)->split[j][x] = '\0';
	*i = str[*i] == ',' ? *i + 1 : *i;
	return (1);
}

int			fill_acc_acc(char *str, int *i, t_acc **acc)
{
	int	len;
	int	j;

	if (!malloc_acc(acc))
		return (0);
	(*i)++;
	(*acc)->type = accolade;
	len = nb_token_in_acc(str, *i);
	if (!((*acc)->split = (char **)malloc(sizeof(char *) * (len + 1))))
		return (error_glob("mg", "42sh: error_glob malloc"));
	j = 0;
	while (len--)
	{
		if (!fill_between_comma(str, i, j, acc))
			return (0);
		j++;
	}
	*i = str[*i] == '}' ? *i + 1 : *i;
	(*acc)->split[j] = NULL;
	return (1);
}
