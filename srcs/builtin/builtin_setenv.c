/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_setenv.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmerabet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/15 21:25:03 by mmerabet          #+#    #+#             */
/*   Updated: 2018/10/05 11:01:28 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "ft_str.h"
#include "ft_mem.h"
#include "ft_io.h"

int	builtin_printenv(int argc, char **argv)
{
	char	*value;
	char	**it;

	if (argc > 1)
	{
		if (!(value = ft_getenv(argv[1], g_shell->envp)))
			return (1);
		ft_putendl(value);
	}
	else if ((it = g_shell->envp))
		while (*it)
			ft_putendl(*it++);
	return (0);
}

int	builtin_setenv(int argc, char **argv)
{
	int		i;
	char	*c;
	char	*nava[2];

	if (!(i = 0) && argc == 1)
		builtin_printenv(0, NULL);
	while (++i < argc)
	{
		if (ft_bzero(nava, sizeof(char *) * 2) && (c = ft_strchr(argv[i], '=')))
		{
			*c = '\0';
			nava[1] = c + 1;
			nava[0] = argv[i];
		}
		else if (i + 1 < argc)
			ft_memcpy(nava, &argv[i++], sizeof(char *) * 2);
		update_export(nava[0], nava[1]);
		if (ft_strequ(nava[0], "HOME"))
			g_shell->homepwd = ft_getenv("HOME", g_shell->envp);
		else if (ft_strequ(nava[0], "USER"))
			g_shell->user = ft_getenv("USER", g_shell->envp);
		else if (ft_strequ(nava[0], "PATH"))
			resetpath();
	}
	return (0);
}

int	builtin_unsetenv(int argc, char **argv)
{
	int	i;

	if (argc == 1)
	{
		ft_putstr_fd("unsetenv: not enough arguments\n", 2);
		return (1);
	}
	i = 0;
	while (++i < argc)
	{
		ft_unset(argv[i]);
		if (ft_strequ(argv[i], "HOME"))
			g_shell->homepwd = NULL;
		else if (ft_strequ(argv[i], "USER"))
			g_shell->user = NULL;
		else if (ft_strequ(argv[i], "PATH"))
			resetpath();
	}
	return (0);
}
