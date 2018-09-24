/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mv_by_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/11 13:59:11 by gdufay            #+#    #+#             */
/*   Updated: 2018/07/17 11:04:18 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libedit.h"

t_cmdedit	*mv_by_word(t_cmdedit *cmd, t_cursor *cursor, char mv)
{
	if ((!cmd->next && mv == 'C') || (!cmd->prev && mv == 'D'))
		return (cmd);
	cmd = (mv == 'D' ? mv_left : mv_right)(cmd, cursor);
	while (cmd->c != '\t' && cmd->c != ' ' && cmd->c != '\n'
			&& cmd->prev && cmd->next)
	{
		cmd = (mv == 'D' ? mv_left : mv_right)(cmd, cursor);
	}
	return (cmd);
}

t_cmdedit	*mv_exterior(t_cmdedit *cmd, t_cursor *cursor, int touch)
{
	if ((!cmd->next && touch == 5) || (!cmd->prev && touch == 1))
		return (cmd);
	while ((touch == 1 && cmd->prev) || (touch == 5 && cmd->next))
		cmd = (touch == 1 ? mv_left : mv_right)(cmd, cursor);
	return (cmd);
}
