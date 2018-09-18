/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ultimate_glob_process.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-rest <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/29 21:08:20 by sle-rest          #+#    #+#             */
/*   Updated: 2018/09/18 16:42:25 by sle-rest         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "globing.h"
#include "ft_str.h"
#include "ft_list.h"
#include "sys/stat.h"

static int	add_new_lst_glob(t_list **lst, struct dirent *dp, char **dir)
{
	int		len;
	char	*str;
	t_list	*new;

	len = ft_strlen(dp->d_name) + ft_strlen(*dir);
	if (!(str = (char *)malloc(sizeof(char) * (len + 1))))
		return (0);
	*str = '\0';
	if (!ft_strncmp(*dir, "./", 2))
		ft_strcat(str, *dir + 2);
	else
		ft_strcat(str, *dir + 1);
	if (ft_strcmp(*dir, "."))
		ft_strcat(str, "/");
	ft_strcat(str, dp->d_name);
	if (!(new = ft_lstcreate(str, ft_strlen(str) + 1)))
		return (0);
	new->next = (*lst)->next;
	if ((*lst)->next)
		(*lst)->next->parent = new;
	(*lst)->next = new;
	new->parent = *lst;
	*lst = new;
	return (1);
}

static char	*get_fullpath(char **dir, struct dirent *dp)
{
	char	*fullpath;
	int		len;

	len = ft_strlen(*dir) + ft_strlen(dp->d_name) + 1;
	if (!(fullpath = (char *)malloc(sizeof(char) * (len + 1))))
		return (NULL);
	*fullpath = '\0';
	ft_strcat(fullpath, *dir);
	ft_strcat(fullpath, "/");
	ft_strcat(fullpath, dp->d_name);
	return (fullpath);
}

static int	split_ultimate_glob_process(t_glob **glob, t_list **lst,
											struct dirent *dp, char **dir)
{
	struct stat		buf;
	char			*fullpath;

	if (!(fullpath = get_fullpath(dir, dp)))
		return (-1);
	if (stat(fullpath, &buf) == -1)
		return (error_glob("s", &fullpath));
	if ((!(*glob)->slash[G_END] && !(*(glob + 1))) ||
			((*glob)->slash[G_END] && !(*(glob + 1)) && S_ISDIR(buf.st_mode)))
	{
		if (!add_new_lst_glob(lst, dp, dir))
			return (-1);
		ft_strdel(&fullpath);
		return (1);
	}
	if (((*glob)->slash[G_END] || (*glob)->next) && !S_ISDIR(buf.st_mode))
		return (error_glob("s", &fullpath));
	if (ultimate_glob_process(glob + 1, lst, &fullpath) == -1)
	{
		ft_strdel(&fullpath);
		return (-1);
	}
	ft_strdel(&fullpath);
	return (1);
}

int			ultimate_glob_process(t_glob **glob, t_list **lst, char **dir)
{
	DIR				*dirp;
	struct dirent	*dp;

	if (!glob || !(*glob))
		return (1);
	if (!(dirp = opendir(*dir)))
		return (error_glob("s", dir));
	while ((dp = readdir(dirp)))
	{
		if (is_match_glob(glob, dp))
		{
			if (split_ultimate_glob_process(glob, lst, dp, dir) == -1)
				return (-1);
		}
	}
	ft_strdel(dir);
	if (closedir(dirp) == -1)
		return (0);
	return (1);
}
