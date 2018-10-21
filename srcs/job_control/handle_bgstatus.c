/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_bgstatus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jraymond <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/17 13:31:39 by jraymond          #+#    #+#             */
/*   Updated: 2018/10/20 17:53:07 by ouralgan         ###   ########.fr       */
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

static int			lookpid_pipe(t_list *elem, pid_t pid, int opt)
{
	t_pids	*pids;

	pids = ((t_inffork *)elem->content)->pids;
	if (!pids)
		return (1);
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

	elem = g_shell->bgproc;
	while (elem && ((t_inffork *)elem->content)->pid != pid)
	{
		if (((t_inffork *)elem->content)->pid == -1 &&
				lookpid_pipe(elem, pid, opt))
			break ;
		elem = elem->next;
	}
	if (((t_inffork *)elem->content)->status)
		((t_inffork *)elem->content)->modif |= (1 << 0);
	((t_inffork *)elem->content)->status = status;
	return (0);
}
