/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/01 14:42:10 by gdufay            #+#    #+#             */
/*   Updated: 2018/10/15 11:27:27 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "ft_mem.h"
#include "ft_str.h"
#include "ft_io.h"

static int	print_export(void)
{
	char	**it;
	char	name[1024];
	char	*content;
	size_t	len;

	if (!(it = g_shell->expor))
		return (0);
	while (*it)
	{
		content = ft_strchr(*it, '=');
		len = content - *it;
		ft_strncpy(name, *it, len < 1024 ? len : 1023);
		name[len < 1024 ? len : 1023] = 0;
		ft_printf("export %s", name);
		if (content && *(content + 1))
			ft_printf("=\"%s\"\n", content + 1);
		else
			ft_putchar('\n');
		it += 1;
	}
	return (0);
}

static int	handle_opt(char ***argv, int *argc, char *p)
{
	t_opt	opt;

	ft_bzero(&opt, sizeof(t_opt));
	while (--(*argc) && ft_getopt(argv, "p", &opt) != OPT_END)
	{
		if (opt.c == 'p')
			*p = 1;
		else if (opt.c == '\0' && (*argv = opt.ptr - 1))
			break ;
		else if (ft_printf_fd(2, "export: bad option: %c\n", opt.c))
			return (1);
	}
	return (0);
}

void		update_export(char *name, char *value)
{
	char	tmp;

	if (!name)
		return ;
	tmp = 0;
	if (!value && ft_getenv(name, g_shell->localp))
	{
		value = ft_strdup(ft_getenv(name, g_shell->localp));
		tmp = 1;
	}
	ft_setenv(name, value ? value : "", &g_shell->expor);
	if (value || ft_getenv(name, g_shell->envp))
		ft_setenv(name, value ? value : "", &g_shell->envp);
	if (value || ft_getenv(name, g_shell->localp))
		ft_setenv(name, value ? value : "", &g_shell->localp);
	if (tmp)
		ft_strdel(&value);
}

int			builtin_export(int argc, char **argv)
{
	char	p;
	char	**s;

	p = 0;
	if (!(argv++) || argc == 1)
		return (print_export());
	if (handle_opt(&argv, &argc, &p))
		return (1);
	if (!argc && p)
		return (print_export());
	while (argc-- > 0 && *argv)
	{
		s = ft_strsplit(*argv, '=');
		if (ft_strchr(*argv, '='))
			update_export(s[0], s[1] ? s[1] : "");
		else
			update_export(s[0], NULL);
		ft_strdel(&(s)[0]);
		ft_strdel(&(s)[1]);
		free(s);
		argv++;
	}
	return (0);
}
