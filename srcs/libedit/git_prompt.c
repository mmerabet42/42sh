/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   git_prompt.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-rest <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/04 16:52:56 by sle-rest          #+#    #+#             */
/*   Updated: 2018/10/08 10:18:37 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "ft_str.h"
#include "ft_io.h"
#include <fcntl.h>
#include <limits.h>
#include <unistd.h>

static void	get_path_git(char *path_git, char *path_git_head)
{
	char	*pos;

	if (!path_git[0])
	{
		ft_strcat(path_git, g_shell->pwd);
		if (ft_strcmp(g_shell->pwd, "/"))
			ft_strcat(path_git, "/.git");
		else
			ft_strcat(path_git, ".git");
		ft_strcat(path_git_head, path_git);
		ft_strcat(path_git_head, "/HEAD");
	}
	else if ((pos = ft_strstr(path_git, "/.git")))
	{
		while (pos && pos != path_git && *pos == '/')
			pos--;
		while (pos != path_git && *pos != '/')
			pos--;
		*pos = '\0';
		ft_strcat(path_git, "/.git");
		path_git_head[0] = '\0';
		ft_strcat(path_git_head, path_git);
		ft_strcat(path_git_head, "/HEAD");
	}
}

int			free_git_prompt(char ***git_prompt, char **buf)
{
	int	i;

	i = -1;
	ft_strdel(buf);
	if (git_prompt && *git_prompt)
	{
		while (++i < 3)
			ft_strdel(&((*git_prompt)[i]));
		free(*git_prompt);
		*git_prompt = NULL;
	}
	return (-1);
}

static int	loop_git_prompt(char *path_git_head, char ***git_prompt)
{
	int		fd;
	char	*tmp;
	char	*buf;

	tmp = NULL;
	if ((fd = open(path_git_head, O_RDONLY)) == -1)
		return (-1);
	if (get_next_line(fd, &buf) == -2)
		return (-1);
	close(fd);
	if (!(*git_prompt = (char **)malloc(sizeof(char *) * 3)))
		return (-1);
	(*git_prompt)[0] = NULL;
	(*git_prompt)[1] = NULL;
	(*git_prompt)[2] = NULL;
	if (!((*git_prompt)[0] = ft_strdup("git:(")))
		return (free_git_prompt(git_prompt, &buf));
	if (!(tmp = ft_strrchr(buf, '/')) || !(*(++tmp)))
		return (free_git_prompt(git_prompt, &buf));
	if (!((*git_prompt)[1] = ft_strdup(tmp)))
		return (free_git_prompt(git_prompt, &buf));
	if (!((*git_prompt)[2] = ft_strdup(")")))
		return (free_git_prompt(git_prompt, &buf));
	ft_strdel(&buf);
	return (1);
}

char		**git_prompt(void)
{
	char	path_git[PATH_MAX];
	char	path_git_head[PATH_MAX];
	char	**git_prompt;
	int		res;

	path_git[0] = '\0';
	path_git_head[0] = '\0';
	git_prompt = NULL;
	while (1)
	{
		get_path_git(path_git, path_git_head);
		if (!access(path_git, R_OK) && !access(path_git_head, R_OK))
		{
			if ((res = loop_git_prompt(path_git_head, &git_prompt)) == -1)
				return (NULL);
			if (res)
				return (git_prompt);
		}
		if (!ft_strcmp(path_git, "/.git"))
			return (NULL);
	}
	return (NULL);
}
