/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_bgstatus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jraymond <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/17 13:31:39 by jraymond          #+#    #+#             */
/*   Updated: 2018/10/10 11:42:55 by jraymond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "ft_str.h"
#include "ft_io.h"
#include "ft_mem.h"

/*
** x de end_status a 1 direct pour sauter le Running car c est pas un
** fin de proc; if !opt new process comming else just change.
*/

static	t_bgstats	g_bgstat[] = {
	{BG_RUN, "Running"}, {BG_KILL, "Killed"},
	{BG_END, "Done"}, {BG_STOP, "Suspended"},
};

static	size_t		g_bgstats_size = sizeof(g_bgstat) / sizeof(t_bgstats);

int					end_status(char *str)
{
	size_t x;

	x = 0;
	while (++x < g_bgstats_size)
	{
		if (*str == 'K' || *str == 'D')
			return (x);
	}
	return (-1);
}

static int			lookpid_pipe(t_list *elem, pid_t pid, int opt)
{
	t_pids	*pids;

	pids = ((t_inffork *)elem->content)->pids;
	log_debug("first\n");
	if (opt)
	{
		while (pids && pids->pid != pid)
			pids = pids->next;
		if (pids)
			return (1);
	}
	return (0);
}

int					handle_bgstat(pid_t pid, int status, int opt)
{
	t_list	*elem;
	size_t	i;

	i = -1;
	elem = g_shell->bgproc;
	while (elem && ((t_inffork *)elem->content)->pid != pid)
	{
		if (((t_inffork *)elem->content)->pid == -1 &&
				lookpid_pipe(elem, pid, opt))
			break ;
		elem = elem->next;
	}
	log_debug("pid: %d opt: %d\n", pid, opt);
	if (((t_inffork *)elem->content)->status[0])
		((t_inffork *)elem->content)->modif |= (1 << 0);
	while (++i < g_bgstats_size)
	{
		if (status == g_bgstat[i].status)
		{
			ft_strcpy(((t_inffork *)elem->content)->status,
						g_bgstat[i].message);
			break ;
		}
	}
	return (0);
}
