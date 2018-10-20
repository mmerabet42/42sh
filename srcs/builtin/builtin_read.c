/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_read.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/08 10:24:29 by gdufay            #+#    #+#             */
/*   Updated: 2018/10/20 17:54:47 by ouralgan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "ft_printf.h"
#include "ft_str.h"
#include "ft_mem.h"
#include "get_next_line.h"
#include <signal.h>

static int	handle_opt(char ***argv, char *r)
{
	t_opt	opt;

	ft_bzero(&opt, sizeof(t_opt));
	while (ft_getopt(argv, "r", &opt) != OPT_END)
	{
		if (opt.c == 'r')
			*r = 1;
		else if (opt.c == '\0' && (*argv = opt.ptr - 1))
			break ;
		else if (ft_printf_fd(2, "read: bad option: %c\n", opt.c))
			return (1);
	}
	return (0);
}

static int	count_backslash(char *s)
{
	int		count;
	char	*tmp;

	if (!s || !*s)
		return (0);
	tmp = s;
	while (*s)
		s++;
	count = 0;
	while (*(--s) == '\\' && s != tmp)
		count++;
	if (*s == '\\')
		count++;
	return (count);
}

static int	handle_read(char **sp, char **argv)
{
	char	*s;
	char	*tmp;

	if (!sp || !argv || (s = NULL))
		return (0);
	while (*sp || *argv)
	{
		if (*argv && *sp)
			manage_export(*argv++, *sp++);
		else if (!*argv && *(argv - 1) && *sp)
		{
			tmp = ft_multi_strjoin(3,
					ft_getenv(*(argv - 1), g_shell->localp), " ", *sp++);
			manage_export(*(argv - 1), tmp);
			ft_strdel(&tmp);
		}
		else if (*argv && !*sp)
			manage_export(*argv++, "");
	}
	return (0);
}

static char	*expands_builtin(char *s1, char *s2)
{
	char	*tmp;

	tmp = ft_strjoin(s1, s2);
	ft_strdel(&s1);
	return (tmp);
}

int			builtin_read(int argc, char **argv)
{
	char	*s;
	char	*tmp;
	char	**sp;
	char	r;

	argv++;
	r = 0;
	if (!argc || handle_opt(&argv, &r) || (tmp = NULL))
		return (1);
	signal(SIGINT, signal_read);
	while (get_next_line(0, &s) > -1)
	{
		tmp = expands_builtin(tmp, s);
		if (!(count_backslash(s) % 2) || r)
			break ;
		ft_strdel(&s);
	}
	ft_strdel(&s);
	if (!r)
		ft_strremove(tmp);
	sp = ft_split_whitespaces(tmp);
	handle_read(sp, argv);
	ft_free_tab(&sp);
	ft_strdel(&tmp);
	return (0);
}
