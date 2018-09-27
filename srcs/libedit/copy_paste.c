/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_paste.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/12 10:21:15 by gdufay            #+#    #+#             */
/*   Updated: 2018/09/25 16:27:03 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libedit.h"

t_cmdedit	*copy_edit(t_cmdedit **cmd, t_cmdedit **cp, t_cursor *cursor)
{
	t_cmdedit *tmp;
	t_cmdedit *del;

	if (*cp)
		ft_free_t_cmdedit(cp);
	if (!(*cmd)->prev)
		return (NULL);
	tmp = *cmd;
	*cmd = mv_exterior(*cmd, cursor, 1);
	if (!(*cp = create_cmdedit((*cmd)->c)))
		return (NULL);
	while (*cmd != tmp)
	{
		del = (*cmd)->next;
		free(*cmd);
		*cmd = del;
		if (!((*cp)->next = create_cmdedit((*cmd)->c)))
			return (NULL);
		(*cp)->next->prev = *cp;
		*cp = (*cp)->next;
	}
	(*cmd)->prev = NULL;
	add_char_between(*cmd, *cursor);
	exec_t("le");
	return (*cp);
}

t_cmdedit	*paste_edit(t_cmdedit *cmd, t_cmdedit *cp, t_cursor *cursor)
{
	char	buf[2];

	if (!cp)
		return (cmd);
	while (cp->prev)
		cp = cp->prev;
	buf[1] = 0;
	while (cp->next)
	{
		buf[0] = cp->c;
		add_char(buf, cmd, cursor);
		cp = cp->next;
	}
	return (cmd);
}
