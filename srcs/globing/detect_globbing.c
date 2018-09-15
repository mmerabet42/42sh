/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   detect_globbing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-rest <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/10 22:30:09 by sle-rest          #+#    #+#             */
/*   Updated: 2018/09/14 16:51:05 by sle-rest         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "globing.h"

/*
**	cet ordre est a suivre
**			- normal
**			- stars
**			- question
**			- accolade
**			- crochet
*/

static int	detect_globbing_acc(char *str, int i)
{
	int		count;
	int		bracket;

	bracket = 1;
	count = 0;
	while (str[++i])
	{
		bracket = str[i] == '{' && ft_no_backslash(str, i) ?
								bracket + 1 : bracket;
		bracket = str[i] == '}' && ft_no_backslash(str, i) ?
								bracket - 1 : bracket;
		count = str[i] == ',' && ft_no_backslash(str, i) ? count + 1 : count;
		if (str[i] == '}' && ft_no_backslash(str, i) && !bracket)
			break ;
	}
	if (count && (str[i] == '}' && ft_no_backslash(str, i)) && !bracket)
		return (1);
	return (0);
}

int			detect_globbing_at(char *str, int i)
{
	int		c;

	if ((str[i] == '*') && ft_no_backslash(str, i))
		return (1);
	if (str[i] == '?')
		return (2);
	if (str[i] == '{' && ft_no_backslash(str, i))
	{
		if (detect_globbing_acc(str, i))
			return (3);
	}
	if (str[i] == '[' && ft_no_backslash(str, i))
	{
		c = i;
		if (str[++i] && str[++i] == ']')
			return (4);
		i = c;
		if ((c = str[++i]) && str[++i] == '-' &&
				(c < str[++i]) && str[++i] == ']')
			return (5);
	}
	return (0);
}

int			detect_globacc(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (detect_globbing_at(str, i) == accolade)
			return (i);
		i++;
	}
	return (-1);
}

int			detect_globbing(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (detect_globbing_at(str, i))
			return (1);
		i++;
	}
	return (0);
}
