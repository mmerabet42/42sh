/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_functions_glob.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-rest <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/19 16:25:26 by sle-rest          #+#    #+#             */
/*   Updated: 2018/09/15 18:13:16 by sle-rest         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "globing.h"

int	malloc_glob(t_glob **glob)
{
	if (!glob || !(*glob))
	{
		if (!(*glob = (t_glob *)malloc(sizeof(t_glob))))
			return (0);
		(*glob)->pat = NULL;
		(*glob)->type = -1;
		(*glob)->slash[G_START] = 0;
		(*glob)->slash[G_END] = 0;
		(*glob)->prev = NULL;
		(*glob)->next = NULL;
		return (1);
	}
	if (!((*glob)->next = (t_glob *)malloc(sizeof(t_glob))))
		return (0);
	(*glob)->next->prev = *glob;
	*glob = (*glob)->next;
	(*glob)->pat = NULL;
	(*glob)->type = -1;
	(*glob)->slash[G_START] = 0;
	(*glob)->slash[G_END] = 0;
	(*glob)->next = NULL;
	return (1);
}

int	malloc_acc(t_acc **acc)
{
	if (!acc || !(*acc))
	{
		if (!(*acc = (t_acc *)malloc(sizeof(t_acc))))
			return (0);
		(*acc)->str = NULL;
		(*acc)->split = NULL;
		(*acc)->i_split = 0;
		(*acc)->type = 0;
		(*acc)->prev = NULL;
		(*acc)->next = NULL;
		return (1);
	}
	if (!((*acc)->next = (t_acc *)malloc(sizeof(t_acc))))
		return (0);
	(*acc)->next->prev = *acc;
	*acc = (*acc)->next;
	(*acc)->str = NULL;
	(*acc)->split = NULL;
	(*acc)->i_split = 0;
	(*acc)->type = 0;
	(*acc)->next = NULL;
	return (1);
}
