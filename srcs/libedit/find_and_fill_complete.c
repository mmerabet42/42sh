/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_and_fill_complete.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/09 16:01:07 by gdufay            #+#    #+#             */
/*   Updated: 2018/09/25 17:24:49 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libedit.h"

char			*find_cmp(t_cmdedit *cmd)
{
	char			*cmp;
	register int	i;
	int				j;

	if (!cmd)
		return (NULL);
	i = 0;
	while (cmd->prev && !is_sep(cmd->prev->c) && cmd->prev->c != '/'
			&& !ft_isws(cmd->prev->c) && cmd->prev->c != '$')
	{
		i++;
		cmd = cmd->prev;
	}
	j = -1;
	if (!(cmp = ft_strnew(i)))
		return (NULL);
	while (++j < i)
	{
		cmp[j] = cmd->c;
		cmd = cmd->next;
	}
	return (cmp);
}

char			*find_path(t_cmdedit *cmd)
{
	char			*path;
	register int	i;
	register int	j;

	if ((j = -1) && !cmd)
		return (NULL);
	path = NULL;
	while (cmd && !is_sep(cmd->c) && !ft_isws(cmd->c) && cmd->c != '/')
		cmd = cmd->prev;
	if (cmd && (is_sep(cmd->c) || ft_isws(cmd->c)))
		return (ft_strdup("."));
	i = 0;
	while (cmd && cmd->prev && !is_sep(cmd->prev->c)
			&& !ft_isws(cmd->prev->c) && ++i)
		cmd = cmd->prev;
	if (!cmd || (!i && cmd->c != '/') || !(path = ft_strnew(i + 1)))
		return (NULL);
	while (cmd && ++j <= i)
	{
		path[j] = cmd->c;
		cmd = cmd->next;
	}
	return (path);
}

t_complete		*fill_complete_var(char *cmp)
{
	char		**tmp;
	size_t		len;
	t_complete	*head;
	t_complete	*complete;
	char		*s;

	if (!g_shell || !g_shell->envp || !cmp)
		return (NULL);
	tmp = g_shell->envp;
	len = ft_strlen(cmp);
	head = NULL;
	while (tmp && *tmp)
	{
		if (!(s = ft_strjoin("$", *tmp)))
			return (head);
		s[ft_strchr(*tmp, '=') - *tmp + 1] = 0;
		if (!ft_strncmp(cmp, *tmp, len))
			add_t_complete(&head, &complete, s);
		ft_strdel(&s);
		tmp += 1;
	}
	return (head);
}

t_complete		*fill_complete_exec(char *path, char *cmp)
{
	char			**paths;
	register int	i;
	t_complete		*head;
	char			*tmp;

	paths = NULL;
	if (!path)
		if (!g_shell->envp || !cmp || !(tmp = ft_getenv("PATH", g_shell->envp))
			|| !(paths = ft_strsplit(tmp, ':')))
			return (NULL);
	i = -1;
	head = NULL;
	if (path)
		head = fill_complete_other(path, cmp);
	else
		while (paths[++i])
		{
			if (!head)
				head = fill_complete_other(paths[i], cmp);
			else
				t_completejoin(&head, fill_complete_other(paths[i], cmp));
		}
	if (paths)
		free(paths);
	return (head);
}

t_complete		*fill_complete_other(char *path, char *cmp)
{
	size_t			len;
	DIR				*dirp;
	struct dirent	*dp;
	t_complete		*head;
	t_complete		*complete;

	if (!path || !cmp)
		return (NULL);
	len = ft_strlen(cmp);
	head = NULL;
	if (!(dirp = opendir(path)))
	{
		ft_strdel(&path);
		return (NULL);
	}
	while ((dp = readdir(dirp)))
		if ((!ft_strncmp(cmp, dp->d_name, len) && len)
				|| (!len && *cmp == '.' && *dp->d_name == '.'))
		{
			add_t_complete(&head, &complete, (dp->d_type == DT_DIR
						? ft_strcat(dp->d_name, "/") : dp->d_name));
		}
	closedir(dirp);
	ft_strdel(&path);
	return (head);
}
