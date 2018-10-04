/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/16 11:24:15 by gdufay            #+#    #+#             */
/*   Updated: 2018/10/04 11:08:34 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libedit.h"

static t_cmdedit	*fill_but_no_write(char *s)
{
	t_cmdedit	*cmd;

	if (!s || !*s)
		return (NULL);
	if (!(cmd = create_cmdedit(*s)))
		return (NULL);
	s += 1;
	while (*s)
	{
		if (!(cmd->next = create_cmdedit(*s)))
			return (NULL);
		cmd->next->prev = cmd;
		cmd = cmd->next;
		s += 1;
	}
	while (cmd->prev)
		cmd = cmd->prev;
	return (cmd);
}

t_cmdedit			*handle_controld(t_cmdedit **cmd, t_cursor *cursor)
{
	if (!(*cmd)->prev && !(*cmd)->next)
	{
		ft_free_t_cmdedit(cmd);
		*cmd = fill_but_no_write("exit ");
		ioctl(0, TIOCSTI, "\n");
		return (*cmd);
	}
	else if ((*cmd)->next)
		del_char(cmd, cursor);
	return (*cmd);
}

t_cmdedit			*end_of_text(t_cmdedit **cmd, t_cursor *cursor)
{
	t_cmdedit	*tmp;

	if (!cmd)
		return (NULL);
	tmp = *cmd;
	mv_exterior(tmp, cursor, 5);
	ft_free_t_cmdedit(cmd);
	*cmd = fill_but_no_write("\3 ");
	ioctl(0, TIOCSTI, "\n");
	return (*cmd);
}

void				sighandler(int sig)
{
	(void)sig;
	ft_putchar('\n');
}
