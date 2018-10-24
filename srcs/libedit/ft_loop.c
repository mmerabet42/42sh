/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_loop.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/23 11:33:35 by gdufay            #+#    #+#             */
/*   Updated: 2018/10/24 16:49:54 by sle-rest         ###   ########.fr       */
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

static void	split_gdp(char **v, char **t, char **u, char **s)
{
	*v = ft_strjoin(*s, "\n");
	*t = ft_strjoin(*v, *u);
	ft_strdel(s);
	if (*u && **u != 3 && ft_strcmp(*u, "exit"))
		*s = *t;
	else
	{
		ft_strdel(t);
		*s = ft_strdup("");
	}
	ft_strdel(u);
	ft_strdel(v);
}

static void	gain_de_place(char **s, int eol)
{
	static char	*t;
	char		*u;
	char		*v;
	int			cursor;

	ft_putchar('\n');
	ft_printf("quote>");
	ft_getcursor(&cursor, NULL);
	u = ft_loop_init(cursor, 1);
	if (eol == '\'' || eol == '`' ||
		(eol == '"' && *(*s + ft_strlen(*s) - 1) != '\\'))
		split_gdp(&v, &t, &u, s);
	else
	{
		v = ft_strn2join(*s, u, ft_strlen(*s) - 1);
		ft_strdel(s);
		*s = v;
		ft_strdel(&u);
	}
}

char		*ft_loop_init(int prompt, int retry)
{
	t_cmdedit	*cmd;
	t_cursor	cursor;
	char		*s;
	int			tmp;

	if (init_cursor(prompt, &cursor) == -1)
		return (NULL);
	if (ft_new_term() == -1)
		return (NULL);
	if (!(cmd = create_cmdedit(' ')))
		return (NULL);
	cmd = ft_main_loop(cmd, &cursor);
	if (ft_clean_term() == -1 || !(s = list_to_str(&cmd)))
		return (NULL);
	while ((tmp = ft_check_quote((s))) && !retry)
		gain_de_place(&s, tmp);
	ft_clean_term();
	return (s);
}
