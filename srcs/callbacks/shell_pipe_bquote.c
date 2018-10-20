/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_pipe_bquote.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jraymond <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/16 15:36:45 by jraymond          #+#    #+#             */
/*   Updated: 2018/10/20 18:02:03 by ouralgan         ###   ########.fr       */
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

static void		swap1(int *fd)
{
	fd[0] = dup(fd[2]);
	fd[1] = dup(fd[3]);
	close(fd[2]);
	close(fd[3]);
	fd[2] = -1;
	fd[3] = -1;
}

static void		closefd(int *fd, t_list *elem)
{
	if (!elem->next)
		close(fd[1]);
	else if (elem->next && elem->parent)
	{
		close(fd[3]);
		close(fd[0]);
	}
	else
		close(fd[0]);
}

static int		init_struct(t_pipe *pipe, t_ast *ast)
{
	int		x;
	int		ret;

	x = -1;
	g_shell->bits |= (1 << 1);
	pipe->all_cmd = NULL;
	ft_bzero(pipe, sizeof(t_pipe));
	while (++x < 4)
		pipe->fd[x] = -1;
	if ((ret = handle_ast_pipe(ast, &pipe->tabpipe)))
		return (ret);
	return (0);
}

static void		fork_son(t_pipe *a, t_list *elem, void *res, t_iterf *iterf)
{
	resetsign();
	if (!elem->next)
	{
		close(a->fd[0]);
		dup2(a->fd[1], 1);
	}
	else if (elem->next && elem->parent)
	{
		close(a->fd[2]);
		close(a->fd[1]);
		dup2(a->fd[3], 1);
		dup2(a->fd[0], 0);
	}
	else
	{
		close(a->fd[1]);
		dup2(a->fd[0], 0);
	}
	ft_astiter((t_ast *)elem->content, res, iterf);
	exit(*(int *)res);
}

static void		fork_father(t_pipe *a, t_list **elem)
{
	closefd(a->fd, *elem);
	if (!a->pgrp)
		a->pgrp = a->pid;
	creatpushelem(&a->bquote, a->pid, *elem, 1);
	*elem = (*elem)->parent;
	if (*elem && (*elem)->next && (*elem)->next->next)
		swap1(a->fd);
}

static int		wait_fork(t_pipe *a, void *res)
{
	t_pids	*elem;

	elem = a->bquote;
	ft_lstdel(&a->tabpipe, NULL);
	while (elem)
	{
		waitpid(elem->pid, res, WUNTRACED);
		elem = elem->next;
	}
	*(int *)res = WEXITSTATUS(*(int *)res);
	signal(SIGCHLD, sign_child);
	freetpids(&a->bquote);
	ft_lstdelone(&a->tabpipe, NULL);
	g_shell->bits &= ~(1 << 1);
	return (0);
}

int				shell_pipe_bquote(t_ast *ast, void **op, void *res, t_iterf *iterf)
{
	t_pipe	a;
	t_list	*elem;
	int		ret;

	(void)op;
	g_shell->bits |= (1 << 1);
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
