/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_del_char.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/11 13:28:32 by gdufay            #+#    #+#             */
/*   Updated: 2018/10/27 10:20:58 by sle-rest         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libedit.h"

void		add_char_between(t_cmdedit *cmd, t_cursor cursor)
{
	t_cmdedit	*tmp;
	int			rememx;
	int			rememy;
	int			max;

	exec_t("cd");
	tmp = cmd;
	rememx = cursor.x + (cursor.y == 1 ? cursor.origin : 1);
	rememy = cursor.y;
	while (tmp->next)
	{
		max = cursor.y == 1 ? cursor.xmax - cursor.origin : cursor.xmax - 1;
		write_char(tmp->c);
		if (cursor.x == max)
		{
			cursor.y++;
			mv_bnl();
		}
		cursor.x += (cursor.x == max ? -cursor.x + 1 : 1);
		tmp = tmp->next;
	}
	while (cursor.y-- > rememy)
		exec_t("up");
	exec_t("cr");
	mv_until_end(rememx);
}

static void	gain_de_place(t_cmdedit *cmd, t_cmdedit *carac)
{
	if (cmd->prev)
		cmd->prev->next = carac;
	carac->prev = cmd->prev;
	cmd->prev = carac;
	carac->next = cmd;
}

static void	extends_add_char(t_cursor *cursor)
{
	cursor->y++;
	mv_bnl();
}

void		add_char(char *buf, t_cmdedit *cmd, t_cursor *cursor)
{
	int			i;
	t_cmdedit	*carac;
	int			max;

	i = -1;
	while (cmd && buf[++i] && buf[i] >= 32 && buf[i] <= 126)
	{
		max = cursor->y == 1 ? cursor->xmax - cursor->origin
			: cursor->xmax - 1;
		if (buf[i] == '\n')
		{
			del_char(&cmd, cursor);
			continue ;
		}
		if (!(carac = create_cmdedit(buf[i])))
			return ;
		gain_de_place(cmd, carac);
		if (cmd->next)
			add_char_between(carac, *cursor);
		else
			write_char(buf[i]);
		if (cursor->x == max)
			extends_add_char(cursor);
		cursor->x += (cursor->x == max ? -cursor->x + 1 : 1);
	}
}

t_cmdedit	*del_char(t_cmdedit **cmd, t_cursor *cursor)
{
	t_cmdedit	*tmp;

	if (!(*cmd)->next)
		return (*cmd);
	tmp = *cmd;
	if ((*cmd)->prev)
		(*cmd)->prev->next = (*cmd)->next;
	(*cmd)->next->prev = (*cmd)->prev;
	*cmd = (*cmd)->next;
	free(tmp);
	add_char_between(*cmd, *cursor);
	exec_t("le");
	return (*cmd);
}
