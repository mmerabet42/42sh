/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   info_bg.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jraymond <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/30 16:40:15 by jraymond          #+#    #+#             */
/*   Updated: 2018/10/26 19:51:50 by sle-rest         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "ft_list.h"
#include "ft_io.h"
#include "ft_mem.h"
#include "ft_str.h"
#include "job_control.h"

static int			tabptrtotab(char **cmd, t_inffork *info, pid_t pid)
{
	int		all_size;
	int		x;

	all_size = 0;
	x = -1;
	if (pid == -1)
	{
		info->cmmd = *cmd;
		return (0);
	}
	while (cmd[++x])
		all_size += (ft_strlen(cmd[x]) + 1);
	if (!(info->cmmd = (char *)ft_memalloc(sizeof(char) * (all_size + 1))))
		return (SH_MALLOC);
	x = -1;
	while (cmd[++x])
	{
		ft_memcpy(&info->cmmd[ft_strlen(info->cmmd)],
					cmd[x], ft_strlen(cmd[x]));
		if (cmd[x + 1])
			info->cmmd[ft_strlen(info->cmmd)] = ' ';
	}
	return (0);
}

static	t_inffork	*init_infproc(int x, pid_t pid, char **cmd)
{
	t_inffork	*new;

	if (!(new = (t_inffork *)ft_memalloc(sizeof(t_inffork))))
		ft_exitf(EXIT_FAILURE, "42sh: %s\n", ft_strshret(SH_MALLOC));
	new->x = x;
	new->pid = (g_shell->bits & (1 << 1)) ? -1 : pid;
	if (tabptrtotab(cmd, new, new->pid) == SH_MALLOC)
		ft_exitf(EXIT_FAILURE, "42sh: %s\n", ft_strshret(SH_MALLOC));
	new->sign = ' ';
	return (new);
}

static	t_list		*init_t_list(t_inffork *content, size_t size)
{
	t_list	*elem;

	if (!(elem = (t_list *)ft_memalloc(sizeof(t_list))))
		ft_exitf(EXIT_FAILURE, "42sh: %s\n", ft_strshret(SH_MALLOC));
	elem->content = content;
	elem->content_size = size;
	return (elem);
}

int					nbproc_inbg(t_list *b_list)
{
	int		x;

	x = 0;
	while (b_list && ++x)
		b_list = b_list->next;
	if (x >= MAX_BGPROC)
	{
		ft_printf_fd(2, "42sh: %s (%d)", ft_strshret(SH_MAXBGPROC),
						MAX_BGPROC);
		return (-1);
	}
	return (x);
}

int					handle_bgproc(pid_t pid_fork, char **cmd,
									int status, int opt)
{
	t_list		*elem;
	t_inffork	*new;
	int			ret;

	if ((ret = nbproc_inbg(g_shell->bgproc)) == -1)
		return (-1);
	if (ret == 0)
		g_shell->numproc = 1;
	else
		++g_shell->numproc;
	new = init_infproc(g_shell->numproc, pid_fork, cmd);
	elem = init_t_list(new, sizeof(t_inffork));
	ft_lstpush_p(&g_shell->bgproc, elem);
	if (getpid() != pid_fork && opt)
		handle_bgsign(elem, 0);
	ret = ((t_inffork *)elem->content)->pid == -1 ? 1 : 0;
	handle_bgstat(pid_fork, status, ret);
	return (0);
}
