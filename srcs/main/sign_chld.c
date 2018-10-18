/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sign_chld.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jraymond <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/06 19:24:16 by jraymond          #+#    #+#             */
/*   Updated: 2018/10/18 20:12:02 by jraymond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "ft_str.h"
#include "job_control.h"
#include <sys/signal.h>
#include <sys/wait.h>

static void		handle_retwait(int ret, pid_t pid)
{
	if (WIFCONTINUED(ret))
		handle_bgstat(pid, BG_RUN, 0);
	else if (WIFSTOPPED(ret))
		handle_bgstat(pid, BG_STOP, 0);
	else if (!WIFEXITED(ret))
		handle_bgstat(pid, BG_KILL, 0);
	else if (WIFEXITED(ret))
		handle_bgstat(pid, BG_END, 0);
}

static void		retwait_negpid(t_pids *pids, int ret, pid_t pid)
{
	(void)pid;
	if (WIFCONTINUED(ret) || WIFSTOPPED(ret))
	{
		if (!pids->next)
		{
			if (WIFCONTINUED(ret))
			{
				log_debug("pid: %d", pids->pid);
				handle_bgstat(pids->pid, BG_RUN, 1);
			}
			else
			{
				log_debug("pid: %d", pids->pid);
				handle_bgstat(pids->pid, BG_STOP, 1);
			}
		}
	}
	else if (!WIFEXITED(ret) || WIFEXITED(ret))
	{
		if (!pids->next)
		{
			if (!WIFEXITED(ret))
			{
				log_debug("pid: %d", pids->pid);
				handle_bgstat(pids->pid, BG_KILL, 1);
			}
			else
			{
				log_debug("pid: %d", pids->pid);
				handle_bgstat(pids->pid, BG_END, 1);
			}
		}
	}
}

static int		neg_pid(t_list *elem)
{
	t_pids	*pipe;
	int		ret;

	log_debug("toto\n");
	pipe = ((t_inffork *)elem->content)->pids;
	while (pipe)
	{
		if (waitpid(pipe->pid, &ret, WNOHANG | WUNTRACED) == pipe->pid)
		{
			log_debug("lol\n");
			retwait_negpid(pipe, ret, getpgid(pipe->pid));
			break ;
		}
		pipe = pipe->next;
	}
	return (0);
}

void			sign_child(int sign)
{
	t_list	*elem;
	pid_t	pid;
	int		ret;

	elem = g_shell->bgproc;
	while (elem)
	{
		pid = ((t_inffork *)elem->content)->pid;
		if (sign == SIGCHLD && pid > 0
				&& waitpid(pid, &ret, WNOHANG | WUNTRACED) == pid)
			handle_retwait(ret, pid);
		else if (sign == SIGCHLD && pid == -1)
			neg_pid(elem);
		elem = elem->next;
	}
}
