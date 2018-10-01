/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/01 14:42:10 by gdufay            #+#    #+#             */
/*   Updated: 2018/10/01 16:22:02 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "ft_mem.h"
#include "ft_str.h"
#include "ft_io.h"

static void	display_and_free(char ***s, char p)
{
	if (p)
	{
		if ((*s)[1])
			ft_printf("%s=%s\n", (*s)[0], (*s)[1]);
		else
			ft_printf("%s\n", (*s)[0]);
	}
	ft_strdel(&(*s)[0]);
	ft_strdel(&(*s)[1]);
	free(*s);
}

static int	print_export(void)
{
	char	**it;

	if (!(it = g_shell->envp))
		return (1);
	while (*it)
		ft_printf("export %s\n", *it++);
	return (0);
}

int			builtin_export(int argc, char **argv)
{
	char	p;
	t_opt	opt;
	char	**s;

	p = 0;
	ft_bzero(&opt, sizeof(t_opt));
	if (!(argv++) || argc == 1)
		return (print_export());
	while (--argc && ft_getopt(&argv, "p", &opt) != OPT_END)
	{
		if (opt.c == 'p')
			p = 1;
		else if (opt.c == '\0' && (argv = opt.ptr - 1))
			break ;
		else if (ft_printf_fd(2, "export: bad option: %c\n", opt.c))
			return (1);
	}
	while (argc-- > 0 && *argv)
	{
		s = ft_strsplit(*argv, '=');
		ft_setenv(s[0], s[1] ? s[1] : "", &g_shell->envp);
		display_and_free(&s, p);
		argv++;
	}
	return (0);
}
