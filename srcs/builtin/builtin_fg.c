/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_fg.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jraymond <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/25 16:03:52 by jraymond          #+#    #+#             */
/*   Updated: 2018/10/09 14:51:04 by jraymond         ###   ########.fr       */
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
	if (elem && (((t_inffork *)elem->content)->status[0] != 'K' ||
				((t_inffork *)elem->content)->status[0] != 'D'))
		return (elem);
	return (error_fg(argv, 0));
}

static void			check_retfork(int ret, pid_t pid)
{
	if (WIFCONTINUED(ret))
	{
		log_debug("RUNNING\n");
		handle_bgstat(pid, BG_RUN, 1);
	}
	else if (WIFSTOPPED(ret))
	{
		if (WSTOPSIG(ret) == SIGTTIN)
		{
			log_debug("STOPPED\n");
			handle_bgstat(pid, BG_STOP, 1);
		}
	}
	else if (!WIFEXITED(ret))
	{
		log_debug("KILLED\n");
		handle_bgstat(pid, BG_KILL, 1);
	}
	else if (WIFEXITED(ret))
	{
		log_debug("FINISH\n");
		handle_bgstat(pid, BG_END, 1);
	}
}

static int			my_wait(t_list *elem)
{
	int		status;
	t_pids	*pids;

	if (((t_inffork *)elem->content)->pid == -1)
	{
		tcsetpgrp(0, ((t_inffork *)elem->content)->pids->pid);
		pids = ((t_inffork *)elem->content)->pids;
		while (pids)
		{
			waitpid(pids->pid, &status, WUNTRACED);
			pids = pids->next;
		}
	}
	else
	{
		tcsetpgrp(0, ((t_inffork *)elem->content)->pid);
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
		print_cmd_args2(((t_inffork *)elem->content)->cmd);
		signal(SIGCHLD, SIG_DFL);
		if (((t_inffork *)elem->content)->status[0] == 'S')
		{
			if (((t_inffork *)elem->content)->pid != -1)
			{
				log_debug("SIGCONT: %d\n", ((t_inffork *)elem->content)->pid);
				kill(((t_inffork *)elem->content)->pid, SIGCONT);
			}
			else
			{
				log_debug(" - SIGCONT: %d\n", ((t_inffork *)elem->content)->pid);
				kill(-((t_inffork *)elem->content)->pids->pid, SIGCONT);
			}
		}
		status = my_wait(elem);
		check_retfork(status, ((t_inffork *)elem->content)->pid);
		tcsetpgrp(0, getpgrp());
		signal(SIGCHLD, sign_child);
	}
	return (0);
}
