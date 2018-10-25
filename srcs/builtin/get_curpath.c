/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_curpath.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-rest <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/25 17:38:55 by sle-rest          #+#    #+#             */
/*   Updated: 2018/10/25 17:41:16 by sle-rest         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "ft_str.h"
#include "ft_printf.h"
#include "ft_mem.h"
#include <sys/stat.h>
#include <limits.h>

static char	*get_curpath_cdpath(char **paths, char *path)
{
	char	*tmp;
	size_t	len;

	tmp = NULL;
	while (paths && *paths)
	{
		len = ft_strlen(*paths);
		if ((*paths)[len - 1] != '/')
			tmp = ft_multi_strjoin(3, *paths, "/", path);
		else
			tmp = ft_multi_strjoin(2, *paths, path);
		if (!access(tmp, F_OK))
			return (tmp);
		paths++;
		ft_strdel(&tmp);
	}
	return (NULL);
}

static char	*cd_back(char *path, int *pathno)
{
	char	*p;
	char	*pwd;
	char	tmp[PATH_MAX];
	int		i;

	(void)pathno;
	i = -1;
	pwd = g_shell->pwd;
	p = ft_strrchr(pwd, '/');
	tmp[0] = '\0';
	while (pwd && p && pwd != p)
	{
		tmp[++i] = *pwd;
		pwd++;
	}
	p = path + 2;
	while (p && *p)
	{
		tmp[++i] = *p;
		p++;
	}
	tmp[++i] = '\0';
	!tmp[0] ? ft_strcat(tmp, "/") : (void)tmp;
	return (ft_strdup(tmp));
}

char		*get_curpath(char *path, int *pathno)
{
	char	*tmp;

	tmp = ft_getenv("HOME", g_shell->envp);
	if (!path && !tmp && (*pathno = 1))
	{
		return ((void*)(size_t)!ft_printf_fd(2, "42sh: cd: HOME not set\n"));
	}
	if (!path && tmp)
		return (ft_strdup(tmp));
	if (!ft_strncmp(path, "..", 2))
		return (cd_back(path, pathno));
	if (path[0] == '/' || !ft_strncmp(path, ".", 1))
		return (ft_strdup(path));
	tmp = ft_getenv("OLDPWD", g_shell->envp);
	if (!ft_strcmp(path, "-") && !tmp && (*pathno = 1))
		return ((void*)(size_t)!ft_printf_fd(2, "42sh: cd: OLDPWD not set\n"));
	if (!ft_strcmp(path, "-") && (*pathno = 1))
		return (ft_strdup(tmp));
	return (get_curpath_extends(path, pathno));
}

char		*get_curpath_extends(char *path, int *pathno)
{
	char		*tmp;
	char		*curpath;
	char		**paths;
	static char *last[2] = {"./", NULL};

	tmp = ft_getenv("CDPATH", g_shell->envp);
	if (tmp)
	{
		paths = ft_strsplit(tmp, ':');
		curpath = get_curpath_cdpath(paths, path);
		ft_free_tab(&paths);
		*pathno = (curpath ? 1 : 0);
		return (curpath ? curpath : get_curpath_cdpath(last, path));
	}
	curpath = get_curpath_cdpath(last, path);
	return (curpath);
}
