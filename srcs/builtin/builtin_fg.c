/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_fg.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jraymond <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/25 16:03:52 by jraymond          #+#    #+#             */
/*   Updated: 2018/10/18 20:12:00 by jraymond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "ft_io.h"
#include "ft_mem.h"
#include "ft_types.h"
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>

static	t_list		*error_fg(char **argv, int i)
{
	if (!i)
		ft_printf_fd(2, "21sh: fg: %s: no such job\n", argv[1]);
	else
		ft_putstr_fd("21sh: fg: current: no such job\n", 2);
	return (NULL);
}

static	t_list		*check_args(int argc, char **argv, int numprocbg)
{
	int		x;
	t_list	*elem;

	x = -1;
	if (!(elem = g_shell->bgproc))
		return (error_fg(argv, 1));
	if (argc > 1)
	{
		while (argv[1][++x] && ft_isdigit(argv[1][x]))
			;
		if (argv[1][x] || numprocbg < 1)
			return (error_fg(argv, 0));
		while (elem && ((t_inffork *)elem->content)->x != numprocbg)
			elem = elem->next;
		handle_bgsign(elem, 0);
	}
	else
	{
		while (elem && ((t_inffork *)elem->content)->sign != '+')
			elem = elem->next;
	}
	if (elem && (((t_inffork *)elem->content)->status != BG_KILL ||
				((t_inffork *)elem->content)->status != BG_END))
		return (elem);
	return (error_fg(argv, 0));
}

static void			check_retfork(int ret, t_list *elem)
{
	int		p;
	pid_t	pid;
	t_pids	*pids;

	pid = 0;
	p = ((t_inffork *)elem->content)->pid == -1 ? 1 : 0;
	if (p)
	{
		pids = ((t_inffork *)elem->content)->pids;
		while (pids->next)
			pids = pids->next;
		pid = pids->pid;
	}
	pid = pid ? pid : ((t_inffork *)elem->content)->pid;
	if (WIFCONTINUED(ret))
		handle_bgstat(pid, BG_RUN, p);
	else if (WIFSTOPPED(ret))
	{
		if (WSTOPSIG(ret) == SIGTTIN)
			handle_bgstat(pid, BG_STOP, p);
	}
	else if (!WIFEXITED(ret))
		handle_bgstat(pid, BG_KILL, p);
	else if (WIFEXITED(ret))
		handle_bgstat(pid, BG_END, p);
}

static int			my_wait(t_list *elem)
{
	int		status;
	t_pids	*pids;

	if (((t_inffork *)elem->content)->pid == -1)
	{
		pids = ((t_inffork *)elem->content)->pids;
		while (pids)
		{
			waitpid(pids->pid, &status, WUNTRACED);
			pids = pids->next;
		}
	}
	else
	{
		waitpid(((t_inffork *)elem->content)->pid, &status, WUNTRACED);
	}
	return (status);
}

int					builtin_fg(int argc, char **argv)
{
	t_list	*elem;
	int		status;

	if (!(elem = check_args(argc, argv, ft_atoi(argv[1]))))
		return (1);
	else
	{
		print_cmd_args2(((t_inffork *)elem->content)->cmmd);
		signal(SIGCHLD, SIG_DFL);
		if (((t_inffork *)elem->content)->status == BG_STOP)
		{
			if (((t_inffork *)elem->content)->pid != -1)
			{
				kill(((t_inffork *)elem->content)->pid, SIGCONT);
				tcsetpgrp(0, ((t_inffork *)elem->content)->pid);
			}
			else
			{
				kill(-((t_inffork *)elem->content)->pids->pid, SIGCONT);
				tcsetpgrp(0, ((t_inffork *)elem->content)->pids->pid);
			}
		}
		status = my_wait(elem);
		check_retfork(status, elem);
		tcsetpgrp(0, getpgrp());
		signal(SIGCHLD, sign_child);
	}
	return (0);
}
