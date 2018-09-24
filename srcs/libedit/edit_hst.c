/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   edit_hst.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/16 12:27:29 by gdufay            #+#    #+#             */
/*   Updated: 2018/07/11 13:47:16 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libedit.h"

t_cmdedit	*mv_history(t_cmdedit **cmd, t_cursor *cursor, char c, t_hst **hst)
{
	if (!hst || !*hst)
		return (*cmd);
	if ((c == 'A' && (*hst)->prev) || (c == 'B' && (*hst)->next))
	{
		mv_exterior(*cmd, cursor, 1);
		ft_free_t_cmdedit(cmd);
		exec_t("cd");
		*cmd = create_cmdedit(' ');
		*hst = (c == 'A' ? (*hst)->prev : (*hst)->next);
		add_char((*hst)->hst, *cmd, cursor);
	}
	else
		ft_putstr("\a");
	return (*cmd);
}

t_hst		*create_t_hst(char *s)
{
	t_hst *hst;

	if (!(hst = malloc(sizeof(t_hst))))
		return (NULL);
	hst->hst = s;
	hst->next = NULL;
	hst->prev = NULL;
	return (hst);
}

void		add_hst(char *s, t_hst **hst)
{
	t_hst *tmp;

	if (!hst)
		return ;
	if (!*hst)
		if (!(*hst = create_t_hst("")))
			return ;
	while (*hst && (*hst)->next)
		*hst = (*hst)->next;
	if (!(tmp = create_t_hst(s)))
		return ;
	if ((*hst)->prev)
		(*hst)->prev->next = tmp;
	tmp->prev = (*hst)->prev;
	tmp->next = *hst;
	(*hst)->prev = tmp;
}

void		ft_free_t_hst(t_hst **hst)
{
	if (!hst || !*hst)
		return ;
	while ((*hst)->prev)
		*hst = (*hst)->prev;
	while ((*hst)->next && ((*hst) = (*hst)->next))
	{
		free((*hst)->prev);
		(*hst)->prev = NULL;
	}
	free(*hst);
	*hst = NULL;
}
