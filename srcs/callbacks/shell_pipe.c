/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jraymond <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/20 19:45:28 by jraymond          #+#    #+#             */
/*   Updated: 2018/10/26 10:31:37 by jraymond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "job_control.h"
#include "ft_str.h"
#include "ft_mem.h"
#include <sys/wait.h>

#include "ft_printf.h"
#include <fcntl.h>

#include "parser.h"

static void		fork_son(t_pipe *a, t_list *elem, void *res, t_iterf *iterf)
{
	setpgid(0, a->pgrp);
	resetsign();
	if (!elem->next)
	{
		close(a->fd[0]);
		dup2(a->fd[1], 1);
	}
	else
	{
		if (elem->parent)
		{
			close(a->fd[2]);
			dup2(a->fd[3], 1);
		}
		close(a->fd[1]);
		dup2(a->fd[0], 0);
	}
	ft_astiter((t_ast *)elem->content, res, iterf);
	exit(*(int *)res);
}

static void		fork_father(t_pipe *a, t_list **elem)
{
	setpgid(a->pid, (!a->pgrp ? a->pid : a->pgrp));
	closefd(a->fd, *elem);
	if (!a->pgrp)
	{
		a->pgrp = a->pid;
		handle_bgproc(a->pid, &a->allcmmd, BG_RUN, 1);
		tcsetpgrp(0, a->pgrp);
		a->head = ft_lstend(g_shell->bgproc);
	}
	creatpushelem(&((t_inffork *)a->head->content)->pids, a->pid, *elem, 0);
	*elem = (*elem)->parent;
	if (*elem && (*elem)->next && (*elem)->next->next)
		swap1(a->fd);
}

static int		handle_res(int res, pid_t pid)
{
	t_list	*elem;

	if (WIFSTOPPED(res))
	{
		handle_bgstat(pid, BG_STOP, 1);
		handle_bgsign(ft_lstend(g_shell->bgproc), 0);
		return (WSTOPSIG(res));
	}
	else if (WIFEXITED(res) || !WIFEXITED(res))
	{
		elem = ft_lstend(g_shell->bgproc);
		if (elem == g_shell->bgproc)
			g_shell->bgproc = NULL;
		else
			elem->parent->next = NULL;
		ft_lstdelone(&elem, del);
	}
	return (WEXITSTATUS(res));
}

static int		wait_fork(t_pipe *a, void *res)
{
	t_pids	*elem;
	int		ret;

	elem = ((t_inffork *)a->head->content)->pids;
	ret = 0;
	ft_lstdel(&a->tabpipe, NULL);
	while (elem)
	{
		waitpid(elem->pid, &ret, WUNTRACED);
		elem = elem->next;
	}
	*(int *)res = handle_res(ret, ((t_inffork *)a->head->content)->pids->pid);
	tcsetpgrp(0, getpgrp());
	signal(SIGCHLD, sign_child);
	ft_lstdelone(&a->tabpipe, NULL);
	g_shell->bits &= ~(1 << 1);
	return (0);
}

int				shell_pipe_cb(t_ast *ast, void *res, t_iterf *iterf)
{
	t_pipe	a;
	t_list	*elem;
	int		ret;

	if ((ret = init_struct(&a, ast)) != 0)
		return (ret);
	elem = ft_lstend(a.tabpipe);
	signal(SIGCHLD, SIG_DFL);
	while (elem)
	{
		if (!a.pgrp && pipe(a.fd) == -1)
			return (SH_PIPFAIL);
		else if (elem->parent && elem->next)
			if (pipe((a.fd + 2)) == -1)
				return (SH_PIPFAIL);
		if ((a.pid = fork()) == -1)
			return (SH_FORKFAIL);
		else if (!a.pid)
			fork_son(&a, elem, res, iterf);
		fork_father(&a, &elem);
	}
	return (wait_fork(&a, res));
}
