/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autocomplete.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/28 12:13:48 by gdufay            #+#    #+#             */
/*   Updated: 2018/07/10 16:41:52 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libedit.h"

inline int	is_sep(char c)
{
	return (c == ';' || c == '&' || c == '|' || c == '\\');
}

static int	check_type(t_cmdedit *cmd)
{
	t_cmdedit	*tmp;

	if (!cmd)
		return (-1);
	tmp = cmd;
	while (cmd)
	{
		if (cmd->c == '$')
			return (VAR);
		if (is_sep(cmd->c) || ft_isws(cmd->c))
			break ;
		cmd = cmd->prev;
	}
	while (tmp && !is_sep(tmp->c) && !ft_isws(tmp->c))
		tmp = tmp->prev;
	while (tmp && ft_isws(tmp->c))
		tmp = tmp->prev;
	if (!tmp || is_sep(tmp->c))
		return (EXEC);
	return (OTHER);
}

t_cmdedit	*autoc(t_cmdedit *cmd, char *s, t_cursor *cursor)
{
	if (!cmd || !s)
		return (NULL);
	while (cmd->prev && !is_sep(cmd->prev->c) && !ft_isws(cmd->prev->c)
			&& (cmd->prev->c != '/' || cmd->prev->is_autoc))
	{
		cmd = move_cursor(cmd, cursor, 'D');
		del_char(&cmd, cursor);
	}
	add_char(s, cmd, cursor);
	if (cmd && cmd->prev)
		cmd->prev->is_autoc = 1;
	return (cmd);
}

t_cmdedit	*autocomplete(t_cmdedit *cmd, t_cursor *cursor)
{
	int					type;
	static t_complete	*complete = NULL;
	char				*cmp;

	if (!cmd || !cmd->prev || cmd->c != ' ' || ft_isws(cmd->prev->c)
			|| (type = check_type(cmd->prev)) == -1)
		return (cmd);
	if (!cmd->prev->is_autoc)
	{
		free_t_complete(&complete);
		if (!(cmp = find_cmp(cmd)))
			return (cmd);
		if (type == EXEC || type == OTHER)
			complete = (type == EXEC ? fill_complete_exec
					: fill_complete_other)(find_path(cmd->prev), cmp);
		else
			complete = fill_complete_var(cmp);
		ft_strdel(&cmp);
	}
	if (complete && complete->name)
	{
		cmd = autoc(cmd, complete->name, cursor);
		complete = complete->next;
	}
	return (cmd);
}
