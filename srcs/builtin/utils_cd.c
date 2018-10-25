/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_cd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/03 12:53:55 by gdufay            #+#    #+#             */
/*   Updated: 2018/10/25 17:41:32 by sle-rest         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "ft_str.h"
#include "ft_printf.h"
#include "ft_mem.h"
#include <sys/stat.h>
#include <limits.h>

char		*remove_prec_component(char *canon)
{
	size_t	len;

	len = ft_strlen(canon) - 1;
	if (len && canon[len] == '/')
		canon[len] = 0;
	while (len && canon[len] != '/')
		canon[len--] = 0;
	return (canon);
}

char		*check_and_move_split(char **curpath, char *path)
{
	struct stat		buf;

	if (stat((*curpath), &buf) == -1)
	{
		ft_strdel(curpath);
		return ((void*)(size_t)!ft_printf_fd(2, "42sh: cd: %s: No such file or "
				"directory\n", path ? path : ft_getenv("HOME", g_shell->envp)));
	}
	else if ((buf.st_mode & S_IFMT) != S_IFDIR)
	{
		ft_strdel(curpath);
		return ((void*)(size_t)!ft_printf_fd(2, "42sh: cd: %s: Not a "
				"directory\n", path ? path : ft_getenv("HOME", g_shell->envp)));
	}
	else if (chdir(*curpath) == -1)
	{
		ft_strdel(curpath);
		return ((void*)(size_t)!ft_printf_fd(2, "42sh: cd: %s: Permission "
				"denied\n", path ? path : ft_getenv("HOME", g_shell->envp)));
	}
	return (*curpath);
}
