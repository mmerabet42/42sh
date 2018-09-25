/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mv_cursor.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/11 13:19:23 by gdufay            #+#    #+#             */
/*   Updated: 2018/09/25 16:55:30 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libedit.h"

t_cmdedit	*move_cursor(t_cmdedit *cmd, t_cursor *cursor, char mv)
{
	if (mv == 'C' && cmd->next)
	{
		cmd = cmd->next;
		mv_right(cursor);
	}
	else if (mv == 'D' && cmd->prev && (cursor->y > 1 || cursor->x > 1))
	{
		cmd = cmd->prev;
		mv_left(cursor);
	}
	else if ((mv == 'A' && cmd->prev) || (mv == 'B' && cmd->next))
		cmd = mv_multline(cmd, cursor, mv);
	else
		ft_putstr("\a");
	return (cmd);
}

void		mv_left(t_cursor *cursor)
{
	if (cursor->x == 1)
	{
		exec_t("up");
		mv_until_end(cursor->xmax - 1);
		cursor->y--;
	}
	else
	{
		exec_t("le");
	}
	cursor->x = (cursor->x == 1 ? cursor->xmax : cursor->x - 1);
	if (cursor->y == 1 && cursor->x == cursor->xmax)
		cursor->x -= cursor->origin;
	if (cursor->y > 1 && cursor->x == cursor->xmax)
		cursor->x -= 1;
}

void		mv_right(t_cursor *cursor)
{
	int		max;

	max = cursor->y == 1 ? cursor->xmax - cursor->origin : cursor->xmax - 1;
	if (cursor->x == max)
	{
		cursor->y++;
		mv_bnl();
	}
	else
		exec_t("nd");
	cursor->x += (cursor->x == max ? -cursor->x + 1 : 1);
}

t_cmdedit	*mv_multline(t_cmdedit *cmd, t_cursor *cursor, char mv)
{
	int i;

	i = 0;
	while (++i < cursor->xmax && ((mv == 'A' && cmd->prev)
				|| (mv == 'B' && cmd->next)))
		cmd = move_cursor(cmd, cursor, (mv == 'A' ? 'D' : 'C'));
	return (cmd);
}
