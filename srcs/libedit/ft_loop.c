/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_loop.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/23 11:33:35 by gdufay            #+#    #+#             */
/*   Updated: 2018/09/25 17:30:32 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libedit.h"
#include "shell.h"

int			ft_putc(int c)
{
	write(0, &c, 1);
	return (0);
}

inline int	ft_isws(int c)
{
	return (c == ' ' || c == '\t' || c == '\v'
			|| c == '\n' || c == '\f' || c == '\r');
}

t_cmdedit	*ft_main_loop(t_cmdedit *cmd, t_cursor *cursor)
{
	char	buf[8];
	int		ret;

	while (1)
	{
		ft_bzero(buf, 7);
		ret = read(0, buf, 7);
		buf[ret] = 0;
		if (buf[0] == '\n')
			break ;
		if (!(cmd = ft_parser_edit(buf, cmd, cursor)))
			break ;
	}
	while (cmd && cmd->next)
		cmd = move_cursor(cmd, cursor, 'C');
	return (cmd);
}

static void	gain_de_place(char **s)
{
	static char	*t;
	char		*u;
	char		*v;
	int			cursor;

	ft_putchar('\n');
	ft_printf("quote>");
	ft_getcursor(&cursor, NULL);
	u = ft_loop_init(cursor, 1);
	v = ft_strjoin(*s, "\n");
	t = ft_strjoin(v, u);
	ft_strdel(s);
	if (u && *u != 3 && ft_strcmp(u, "exit"))
		*s = t;
	else
	{
		ft_strdel(&t);
		*s = ft_strdup("");
	}
	ft_strdel(&u);
	ft_strdel(&v);
}

char		*ft_loop_init(int prompt, int retry)
{
	t_cmdedit	*cmd;
	t_cursor	cursor;
	char		*s;

	if (init_cursor(prompt, &cursor) == -1)
		return (NULL);
	if (ft_new_term() == -1)
		return (NULL);
	if (!(cmd = create_cmdedit(' ')))
		return (NULL);
	cmd = ft_main_loop(cmd, &cursor);
	if (ft_clean_term() == -1 || !(s = list_to_str(&cmd)))
		return (NULL);
	while (!ft_check_quote((s)) && !retry)
		gain_de_place(&s);
	ft_clean_term();
	return (s);
}
