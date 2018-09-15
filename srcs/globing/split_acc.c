/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_acc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-rest <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/21 14:23:34 by sle-rest          #+#    #+#             */
/*   Updated: 2018/09/14 16:29:19 by sle-rest         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "globing.h"

static int	fill_acc_normal(char *str, int *i, t_acc **acc)
{
	int	len;
	int	j;

	if (!malloc_acc(acc))
		return (0);
	(*acc)->type = normal;
	len = *i;
	while (str[len] && detect_globbing_at(str, len) != accolade)
		len++;
	if (!((*acc)->str = (char *)malloc(sizeof(char) * (len - *i + 1))))
		return (0);
	j = 0;
	len -= *i;
	while (len--)
	{
		(*acc)->str[j] = str[*i];
		j++;
		(*i)++;
	}
	(*acc)->str[j] = '\0';
	return (1);
}

int			get_struct_acc(char *str, t_acc **acc)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (detect_globbing_at(str, i) == accolade)
		{
			if (!fill_acc_acc(str, &i, acc))
				break ;
		}
		else
		{
			if (!fill_acc_normal(str, &i, acc))
				break ;
		}
	}
	while (acc && *acc && (*acc)->prev)
		*acc = (*acc)->prev;
	return (str[i] ? 0 : 1);
}

char		**split_acc(char *str)
{
	t_acc	*acc;
	char	**splitacc;

	acc = NULL;
	if (!get_struct_acc(str, &acc))
	{
		free_acc(&acc);
		return (0);
	}
	if (!simplification_bracket(acc))
	{
		free_acc(&acc);
		return (0);
	}
	splitacc = NULL;
	if (!(splitacc = get_splitacc(acc)))
	{
		free_acc(&acc);
		return (0);
	}
	free_acc(&acc);
	return (splitacc);
}
