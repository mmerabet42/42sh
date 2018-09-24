/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_loop.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/23 11:33:35 by gdufay            #+#    #+#             */
/*   Updated: 2018/09/24 16:42:37 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libedit.h"

t_cmdedit	*ft_main_loop(t_cmdedit *cmd, t_cursor *cursor, t_hst **hst)
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
		if (!(cmd = ft_parser_edit(buf, cmd, cursor, hst)))
			break ;
	}
	while (cmd && cmd->next)
		cmd = mv_right(cmd, cursor);
	return (cmd);
}

static void	gain_de_place(t_env *lstenv, char **s)
{
	static char	*t;
	char		*u;
	char		*v;

	ft_putchar('\n');
	u = ft_loop_init("quote>", 1, NULL, lstenv);
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

char		*ft_loop_init(char *prompt, int retry, t_hst **hst, t_env *lstenv)
{
	t_cmdedit	*cmd;
	t_cursor	cursor;
	char		*s;

	ft_putstr((g_statut_exit ? "\033[31m" : "\033[32m"));
	ft_putstr(prompt);
	ft_putstr("\033[0m");
	if (!(cmd = create_cmdedit(' ')))
		return (NULL);
	if (init_cursor(prompt, &cursor) == -1)
		return (NULL);
	if (ft_new_term(lstenv) == -1)
		return (NULL);
	cmd = ft_main_loop(cmd, &cursor, hst);
	if (ft_clean_term() == -1 || !(s = list_to_str(&cmd)))
		return (NULL);
	while (!ft_check_quote((s)) && !retry)
		gain_de_place(lstenv, &s);
	if (s && *s && s[ft_strlen(s) - 1] != 3)
		add_hst(s, hst);
	ft_clean_term();
	return (s);
}
