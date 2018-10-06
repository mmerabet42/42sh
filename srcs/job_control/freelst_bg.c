/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freelst_bg.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jraymond <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/17 14:51:51 by jraymond          #+#    #+#             */
/*   Updated: 2018/10/06 22:37:27 by jraymond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "ft_mem.h"

void		del(void *content, size_t size)
{
	t_pids	*pids;
	t_pids	*tmp;

	(void)size;
	ft_delenv(&((t_inffork *)content)->cmd);
	if (((t_inffork *)content)->pid == -1)
	{
		pids = ((t_inffork *)content)->pids;
		while (pids)
		{
			tmp = pids->next;
			ft_memdel((void **)&pids);
			pids = tmp;
		}
	}
	ft_memdel(&content);
}

void		freelst_bg(void)
{
	t_list	*elem;
	t_list	*next;

	if (!(elem = g_shell->bgproc))
		return ;
	g_shell->bgproc = NULL;
	while (elem)
	{
		next = elem->next;
		ft_lstdelone(&elem, del);
		elem = next;
	}
}
