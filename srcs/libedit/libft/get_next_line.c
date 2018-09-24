/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/26 10:24:21 by gdufay            #+#    #+#             */
/*   Updated: 2018/01/26 13:38:13 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_free_fd(char **s)
{
	char *tmp;

	if (!s || !*s)
		return ;
	tmp = ft_strchr(*s, '\n');
	if (s && *s)
		ft_strdel(s);
	if (tmp && (tmp + 1))
		*s = ft_strdup(tmp + 1);
}

static int	ft_nl(char *s)
{
	int i;

	i = -1;
	if (!s)
		return (0);
	while (s[++i] && s[i] != '\n')
		;
	return (i);
}

int			get_next_line(const int fd, char **line)
{
	char		buf[BUFF_SIZE + 1];
	char		*tmp;
	static char	*tab[INT_MAX];
	int			ret;

	if (!line)
		return (-1);
	while ((ret = read(fd, buf, BUFF_SIZE)) > 0)
	{
		buf[ret] = 0;
		tmp = (tab[fd] ? ft_strdup(tab[fd]) : NULL);
		(tab[fd] && *tab[fd] ? ft_strdel(&tab[fd]) : 0);
		tab[fd] = (tmp ? ft_strjoin(tmp, buf) : ft_strdup(buf));
		(tmp ? ft_strdel(&tmp) : 0);
		if (ft_strchr(buf, '\n'))
			break ;
	}
	if (ret == -1)
		return (-1);
	(line && *line ? ft_strdel(line) : 0);
	if (ft_nl(tab[fd]))
		*line = ft_strncpy(ft_strnew(ft_nl(tab[fd])), tab[fd], ft_nl(tab[fd]));
	ft_free_fd(&tab[fd]);
	return (tab[fd] || *line ? 1 : 0);
}
