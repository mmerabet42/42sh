/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mv_by_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/11 13:59:11 by gdufay            #+#    #+#             */
/*   Updated: 2018/09/25 14:33:22 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libedit.h"

t_cmdedit	*mv_by_word(t_cmdedit *cmd, t_cursor *cursor, char mv)
{
	if ((!cmd->next && mv == 'C') || (!cmd->prev && mv == 'D'))
		return (cmd);
	cmd = move_cursor(cmd, cursor, mv);
	while (cmd->c != '\t' && cmd->c != ' ' && cmd->c != '\n'
			&& cmd->prev && cmd->next)
		cmd = move_cursor(cmd, cursor, mv);
	return (cmd);
}

t_cmdedit	*mv_exterior(t_cmdedit *cmd, t_cursor *cursor, int touch)
{
	if ((!cmd->next && touch == 5) || (!cmd->prev && touch == 1))
		return (cmd);
	while ((touch == 1 && cmd->prev) || (touch == 5 && cmd->next))
		cmd = move_cursor(cmd, cursor, (touch == 1 ? 'D' : 'C'));
	return (cmd);
}
