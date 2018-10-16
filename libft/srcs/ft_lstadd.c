/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmerabet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/16 16:24:21 by mmerabet          #+#    #+#             */
/*   Updated: 2018/10/16 16:24:38 by mmerabet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_list.h"

void	ft_lstpushfront(t_list **alst, t_list *elem)
{
	t_list	*lst_end;

	if (alst && elem)
	{
		lst_end = ft_lstend(elem);
		if ((lst_end->next = *alst))
			(*alst)->parent = lst_end;
		*alst = elem;
	}
}

void	ft_lstadd(t_list **alst, t_list *elem)
{
	if (alst && elem)
	{
		if ((elem->next = *alst))
			(*alst)->parent = elem;
		*alst = elem;
	}
}

t_list	*ft_lstinsert(t_list **alst, t_list *elem, int invert)
{
	t_list	*tmp;
	t_list	*end;

	end = ft_lstend(elem);
	if (!*alst)
	{
		*alst = elem;
		return (end);
	}
	if (invert && (end->next = *alst))
	{
		if ((tmp = (*alst)->parent))
			ft_lstconnect(tmp, elem);
		(*alst)->parent = end;
	}
	else if ((elem->parent = *alst))
	{
		if ((tmp = (*alst)->next))
			ft_lstconnect(end, tmp);
		(*alst)->next = elem;
	}
	return (end);
}
