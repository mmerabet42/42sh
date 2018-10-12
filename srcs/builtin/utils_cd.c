/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_cd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/03 12:53:55 by gdufay            #+#    #+#             */
/*   Updated: 2018/10/12 13:27:11 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "ft_str.h"
#include "ft_printf.h"
#include "ft_mem.h"

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

static char	*get_curpath_cdpath(char **paths, char *path)
{
	char	*tmp;
	size_t	len;

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
	return (curpath ? curpath :
			(void*)(size_t)!ft_printf("42sh: cd: %s: Not a directory\n", path));
}
