/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mv_cursor.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/11 13:19:23 by gdufay            #+#    #+#             */
/*   Updated: 2018/07/17 11:06:02 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libedit.h"

t_cmdedit	*move_cursor(t_cmdedit *cmd, t_cursor *cursor, char mv)
{
	if (mv == 'C' && cmd->next)
		cmd = mv_right(cmd, cursor);
	else if (mv == 'D' && cmd->prev)
		cmd = mv_left(cmd, cursor);
	else if ((mv == 'A' && cmd->prev) || (mv == 'B' && cmd->next))
		cmd = mv_multline(cmd, cursor, mv);
	else
		ft_putstr("\a");
	return (cmd);
}

t_cmdedit	*mv_left(t_cmdedit *cmd, t_cursor *cursor)
{
	cmd = cmd->prev;
	if (!cursor->x)
	{
		exec_t("up");
		mv_until_end(cursor->xmax);
		cursor->y--;
	}
	else
		exec_t("le");
	cursor->x -= (!cursor->x ? -cursor->xmax + 1 : 1);
	return (cmd);
}

t_cmdedit	*mv_right(t_cmdedit *cmd, t_cursor *cursor)
{
	cmd = cmd->next;
	if (cursor->x >= cursor->xmax - 1)
	{
		cursor->y++;
		mv_bnl();
	}
	else
		exec_t("nd");
	cursor->x += (cursor->x >= cursor->xmax - 1 ? -cursor->x : 1);
	return (cmd);
}

t_cmdedit	*mv_multline(t_cmdedit *cmd, t_cursor *cursor, char mv)
{
	int i;

	i = -1;
	while (++i < cursor->xmax && ((mv == 'A' && cmd->prev)
				|| (mv == 'B' && cmd->next)))
		cmd = (mv == 'A' ? mv_left : mv_right)(cmd, cursor);
	return (cmd);
}
