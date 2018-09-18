/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_glob.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-rest <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/29 00:05:33 by sle-rest          #+#    #+#             */
/*   Updated: 2018/09/18 15:59:54 by ouralgan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "globing.h"
#include "ft_str.h"
#include "ft_list.h"

static int	process_glob_acc(t_glob **glob, t_list **lst)
{
	char	*dir;

	if ((*glob)->slash[G_START])
	{
		if (!(dir = ft_strdup("/")))
			return (0);
	}
	else
	{
		if (!(dir = ft_strdup(".")))
			return (0);
	}
	if (ultimate_glob_process(glob, lst, &dir) == -1)
		return (0);
	ft_strdel(&dir);
	return (1);
}

int			process_glob(t_glob ***glob, t_list **lst)
{
	int		i;
	t_list	*tmp;

	i = -1;
	while (glob[++i])
	{
		tmp = *lst;
		if (!process_glob_acc(glob[i], lst))
			return (0);
		if (tmp != *lst)
		{
			if (tmp->parent)
				tmp->parent->next = tmp->next;
			if (tmp->next)
				tmp->next->parent = tmp->parent;
			free(tmp->content);
			free(tmp);
		}
		if (lst && *lst && (*lst)->next)
			*lst = (*lst)->next;
	}
	while (lst && *lst && (*lst)->parent)
		*lst = (*lst)->parent;
	return (1);
}
