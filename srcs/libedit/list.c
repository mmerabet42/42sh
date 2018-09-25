/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/16 11:01:48 by gdufay            #+#    #+#             */
/*   Updated: 2018/06/28 14:36:41 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libedit.h"

void		ft_free_t_cmdedit(t_cmdedit **cmd)
{
	if (!*cmd)
		return ;
	while ((*cmd)->prev)
		*cmd = (*cmd)->prev;
	while ((*cmd)->next && ((*cmd) = (*cmd)->next))
	{
		free((*cmd)->prev);
		(*cmd)->prev = NULL;
	}
	free(*cmd);
	*cmd = NULL;
}

char		*list_to_str(t_cmdedit **cmd)
{
	int		i;
	char	*s;

	i = 0;
	if (!*cmd)
		return (NULL);
	while ((*cmd)->prev && ++i)
		*cmd = (*cmd)->prev;
	if (!(s = ft_strnew(i)))
		return (NULL);
	i = -1;
	while ((*cmd)->next)
	{
		s[++i] = (*cmd)->c;
		*cmd = (*cmd)->next;
		free((*cmd)->prev);
	}
	free(*cmd);
	return (s);
}

t_cmdedit	*create_cmdedit(char c)
{
	t_cmdedit *tmp;

	if (!(tmp = malloc(sizeof(t_cmdedit))))
		return (NULL);
	tmp->c = c;
	tmp->is_autoc = 0;
	tmp->next = NULL;
	tmp->prev = NULL;
	return (tmp);
}
