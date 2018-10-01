/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   edit_hst.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/16 12:27:29 by gdufay            #+#    #+#             */
/*   Updated: 2018/10/01 11:48:23 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libedit.h"

t_cmdedit	*mv_history(t_cmdedit **cmd, t_cursor *cursor, char c)
{
	t_list *hst;

	hst = g_shell->history;
	if (!hst)
		return (*cmd);
	if ((c == 'A' && hst->parent) || (c == 'B' && hst->next))
	{
		mv_exterior(*cmd, cursor, 1);
		ft_free_t_cmdedit(cmd);
		exec_t("cd");
		*cmd = create_cmdedit(' ');
		hst = (c == 'A' ? hst->parent : hst->next);
		add_char(hst->content, *cmd, cursor);
	}
	else
		ft_putstr("\a");
	g_shell->history = hst;
	return (*cmd);
}
