/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_match_functions.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-rest <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/04 18:43:47 by sle-rest          #+#    #+#             */
/*   Updated: 2018/09/14 16:54:31 by sle-rest         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "globing.h"

int	check_match_normal(t_glob **glob, char *name, int *i)
{
	int	j;

	j = 0;
	while (name[*i] && (*glob)->pat[j] && name[*i] == (*glob)->pat[j])
	{
		(*i)++;
		j++;
	}
	if (!(*glob)->pat[j])
		return (1);
	return (0);
}

int	split_check_match_stars(t_glob **glob, char *name, int *i)
{
	int		j;
	int		x;
	t_glob	*tmp;

	j = *i;
	if (!name[*i])
		return (1);
	while (name[*i] && name[*i + 1])
		(*i)++;
	while (name[*i] && *i >= j)
	{
		x = *i;
		tmp = (*glob)->next;
		if (is_match_stars(&tmp, name, i))
			return (1);
		*i = x - 1;
	}
	return (0);
}

int	check_match_stars(t_glob **glob, char *name, int *i)
{
	if (!(*glob)->prev && name[0] == '.')
		return (0);
	if (!(*glob)->next)
	{
		while (name[*i])
			(*i)++;
		return (1);
	}
	return (split_check_match_stars(glob, name, i));
}

int	check_match_question(t_glob **glob, char *name, int *i)
{
	(void)glob;
	if (!(*i) && !(*glob)->prev && name[0] == '.')
		return (0);
	if (name[*i])
	{
		(*i)++;
		return (1);
	}
	return (0);
}

int	check_match_crochet(t_glob **glob, char *name, int *i)
{
	int	j;

	j = -1;
	while ((*glob)->pat[++j])
	{
		if ((*glob)->pat[j] == name[*i])
		{
			(*i)++;
			return (1);
		}
	}
	return (0);
}
