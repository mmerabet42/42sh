/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstpush.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmerabet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/11 19:08:24 by mmerabet          #+#    #+#             */
/*   Updated: 2018/10/16 16:32:32 by mmerabet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_list.h"

t_list	*ft_lstpush(t_list *lst, t_list *elem)
{
	t_list	*lst_end;

	if ((lst_end = ft_lstend(lst)))
		if ((lst_end->next = elem))
			elem->parent = lst_end;
	return (elem);
}

t_list	*ft_lstpush_p(t_list **lst, t_list *elem)
{
	t_list	*tmp;

	if (!lst)
		return (NULL);
	tmp = ft_lstpush(*lst, elem);
	if (!*lst)
		*lst = tmp;
	return (tmp);
}

int		ft_lstpush_pi(t_list **lst, t_list *elem)
{
	t_list	*lst_end;
	int		i;

	if (!lst)
		return (-1);
	i = 0;
	if (!(lst_end = *lst))
	{
		*lst = elem;
		return (i);
	}
	while (++i && lst_end->next)
		lst_end = lst_end->next;
	if ((lst_end->next = elem))
		elem->parent = lst_end;
	return (i);
}
