/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_splitacc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-rest <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/23 14:10:03 by sle-rest          #+#    #+#             */
/*   Updated: 2018/09/15 18:10:14 by sle-rest         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "globing.h"
#include "ft_str.h"

static int	get_len_splitacc(t_acc *acc)
{
	int	len;

	len = 1;
	while (acc)
	{
		if (acc->type == accolade)
			len *= (int)ft_tabsize(acc->split);
		acc = acc->next;
	}
	return (len);
}

static int	get_len_one_splitacc(t_acc *acc)
{
	int	len;

	len = 0;
	while (acc)
	{
		if (acc->type == normal)
			len += ft_strlen(acc->str);
		else
			len += ft_strlen(acc->split[acc->i_split]);
		acc = acc->next;
	}
	return (len);
}

static char	*get_one_splitacc(t_acc *acc)
{
	int		len;
	char	*one_splitacc;

	len = get_len_one_splitacc(acc);
	if (!(one_splitacc = (char *)malloc(sizeof(char) * (len + 1))))
		return (NULL);
	*one_splitacc = '\0';
	while (acc)
	{
		if (acc->type == normal)
			ft_strcat(one_splitacc, acc->str);
		else
			ft_strcat(one_splitacc, acc->split[acc->i_split]);
		acc = acc->next;
	}
	return (one_splitacc);
}

static void	incr_i_split(t_acc *acc)
{
	while (acc && acc->next)
		acc = acc->next;
	while (1)
	{
		while (acc && acc->type == normal)
			acc = acc->prev;
		if (!acc)
			return ;
		if (acc->i_split + 1 >= (int)ft_tabsize(acc->split))
		{
			acc->i_split = 0;
			acc = acc->prev;
		}
		else
		{
			acc->i_split++;
			return ;
		}
	}
}

char		**get_splitacc(t_acc *acc)
{
	char	**splitacc;
	int		len;
	int		i;

	splitacc = NULL;
	len = get_len_splitacc(acc);
	if (!(splitacc = (char **)malloc(sizeof(char *) * (len + 1))))
		return (NULL);
	i = 0;
	while (len--)
	{
		if (!(splitacc[i] = get_one_splitacc(acc)))
			return (NULL);
		incr_i_split(acc);
		i++;
	}
	splitacc[i] = NULL;
	return (splitacc);
}
