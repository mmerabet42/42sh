/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_edit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/23 12:27:01 by gdufay            #+#    #+#             */
/*   Updated: 2018/09/25 13:11:58 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libedit.h"

/*static void			clean_autoc(t_cmdedit **cmd)
{
	t_cmdedit	*tmp;

	if (!cmd)
		return ;
	tmp = *cmd;
	while (tmp && tmp->prev)
	{
		tmp->is_autoc = 0;
		tmp = tmp->prev;
	}
}*/

static t_cmdedit	*gain_de_place(char *buf, t_cmdedit *cmd, t_cursor *cursor)
{
	if (buf[0] == 127 && cmd->prev)
	{
		cmd = move_cursor(cmd, cursor, 'D');
		cmd = del_char(&cmd, cursor);
	}
	/*if (buf[0] == '\t')
		cmd = autocomplete(cmd, cursor);
	else*/
		//clean_autoc(&cmd);
	return (cmd);
}

t_cmdedit			*ft_parser_edit(char *buf, t_cmdedit *cmd, t_cursor *cursor)
{
	static t_cmdedit *cp = NULL;

	if (buf[0] >= 32 && buf[0] <= 126)
		add_char(buf, cmd, cursor);
	else if (buf[2] == 'D' || buf[2] == 'C')
		cmd = move_cursor(cmd, cursor, buf[2]);
	else if (buf[5] == 'A' || buf[5] == 'B')
		cmd = move_cursor(cmd, cursor, buf[5]);
	else if (buf[2] == '3' && buf[3] == '~')
		cmd = del_char(&cmd, cursor);
	else if (buf && buf[5] && (buf[5] == 'C' || buf[5] == 'D'))
		cmd = mv_by_word(cmd, cursor, buf[5]);
	else if (buf[0] == 1 || buf[0] == 5)
		cmd = mv_exterior(cmd, cursor, (int)buf[0]);
	else if (buf[0] == 21)
		cp = copy_edit(&cmd, &cp, cursor);
	else if (buf[0] == 24)
		cmd = paste_edit(cmd, cp, cursor);
	else if (buf[0] == 4 && !cmd->prev)
		cmd = end_of_emit(&cmd);
	else if (buf[0] == 3)
		cmd = end_of_text(&cmd);
	//else if (buf[2] == 'A' || buf[2] == 'B')
	//	cmd = mv_history(&cmd, cursor, buf[2], hst);
	return (gain_de_place(buf, cmd, cursor));
}
