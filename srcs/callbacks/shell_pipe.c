/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jraymond <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/20 19:45:28 by jraymond          #+#    #+#             */
/*   Updated: 2018/10/06 17:08:04 by jraymond         ###   ########.fr       */
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

/*static void		print_tab_pipe(t_list *begin)
{
	t_list	*elem;

	elem = ft_lstend(begin);
	while (elem)
	{
		if (!elem->content)
			log_debug("NULL NULL\n");
		else
			log_debug("%s\n", ((t_ast *)elem->content)->name);
		elem = elem->parent;
	}
}*/


/*static void			printfd(int *fd)
{
	int	x;

	x = -1;
	while (++x < 4)
		log_debug("fd[%d]: %d\n", x, fd[x]);
}*/

/*static int		son_action(int *fd, t_list *elem)
{
	if (!elem->next)
	{
		close(fd[0]);
		dup2(fd[1], 1);
	}
	else if (elem->next && elem->parent)
	{
		close(fd[2]);
		close(fd[1]);
		dup2(fd[3], 1);
		dup2(fd[0], 0);
	}
	else
	{
		close(fd[1]);
		dup2(fd[0], 0);
	}
	return (0);
}

static int		save_pid(t_list *elem, pid_t pid)
{
	t_pids		*pids;

	if (!(elem1 = ft_lstnew(NULL, 0)))
		return (SH_MALLOC);
	elem1->content_size = pid;
	if (!((t_inffork *)elem->content)->pids)
		((t_inffork *)elem->content)->pids = elem1;
	else
		ft_lstpush(((t_inffork *)elem->content)->p_pid, elem1);
	return (0);
}*/

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
	ft_bzero(pipe, sizeof(t_pipe));
	while (++x < 4)
		pipe->fd[x] = -1;
	if ((ret = handle_ast_pipe(ast, &pipe->tabpipe)))
		return (ret);
	return (0);
}

static void		fork_son(t_pipe *a, t_list *elem, void *res, t_iterf *iterf)
{
	setpgid(0, a->pgrp);
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
	setpgid(a->pid, (!a->pgrp ? a->pid : a->pgrp));
	closefd(a->fd, *elem);
	if (!a->pgrp)
	{
		a->pgrp = a->pid;
		handle_bgproc(a->pid, ((t_ast *)(*elem)->content)->args->argv, BG_RUN, 1);
		tcsetpgrp(0, a->pgrp);
		a->head = ft_lstend(g_shell->bgproc);
	}
	creatpushelem(&((t_inffork *)a->head->content)->pids, a->pid);
	*elem = (*elem)->parent;
		if (*elem && (*elem)->next && (*elem)->next->next)
			swap1(a->fd);
}

static int		handle_res(int res, pid_t pid)
{
	t_list	*elem;

	if (WIFEXITED(res))
	{
		elem = ft_lstend(g_shell->bgproc);
		if (elem == g_shell->bgproc)
			g_shell->bgproc = NULL;
		else
			elem->parent->next = NULL;
		ft_lstdelone(&elem, del);
	}
	else if (WIFSTOPPED(res))
	{
		handle_bgstat(pid, BG_STOP, 1);
		handle_bgsign(ft_lstend(g_shell->bgproc), 0);
		return (WSTOPSIG(res));
	}
	return (WEXITSTATUS(res));
}

static int		wait_fork(t_pipe *a, void *res)
{
	t_pids	*elem;
	int		ret;

	elem = ((t_inffork *)a->head->content)->pids;
	ret = 0;
	while (elem)
	{
		if (waitpid(elem->pid, res, WUNTRACED) == -1)
			log_debug("waitpid == -1\n");
		log_debug("waitpid: %d", elem->pid);
		elem = elem->next;
	}
	*(int *)res = handle_res(*(int *)res, ((t_inffork *)a->head->content)->pids->pid);
	tcsetpgrp(0, getpgrp());
	signal(SIGCHLD, sign_child);
	ft_lstdelone(&a->tabpipe, NULL);
	g_shell->bits &= ~(1 << 1);
	return (0);
}

int				shell_pipe_cb(t_ast *ast, void **op, void *res, t_iterf *iterf)
{
	t_pipe	a;
	t_list	*elem;
	int		ret;

	(void)op;
	if (g_shell->bits & (1 << 2))
		return (shell_pipe_bg(ast, op, res, iterf));
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
/*
int				shell_pipe_cb(t_ast *ast, void **op, void *res, t_iterf *iterf)
{
	t_list	*tabpipe;
	t_list	*elem;
	pid_t	pid;
	int		fd[4];
	int		pgrp;
	t_list	*head;

	(void)op;
	g_shell->bits |= (1 << 1);
	tabpipe = NULL;
	head = NULL;
	if ((pgrp = handle_ast_pipe(ast, &tabpipe)))
		return (pgrp);
	print_tab_pipe(tabpipe);
	elem = ft_lstend(tabpipe);
	pgrp = 0;
	fd[0] = -1;
	fd[1] = -1;
	fd[2] = -1;
	fd[3] = -1;
	while (elem)
	{
		if (!pgrp && pipe(fd) == -1)
			return (SH_PIPFAIL);
		else if (elem->parent && elem->next)
			if (pipe((fd + 2)) == -1)
				return (SH_PIPFAIL);
		if (!(pid = fork()))
		{
			if (!(g_shell->bits & (1 << 2)))
				setpgid(0, pgrp);
//			printfd(fd);
			son_action(fd, elem);
			ft_astiter((t_ast *)elem->content, res, iterf);
			exit(*(int *)res);
		}
		else if (pid == -1)
			return (SH_FORKFAIL);
		if (!(g_shell->bits & (1 << 2)))
			setpgid(pid, (!pgrp ? pid : pgrp));
		closefd(fd, elem);
		if (!pgrp && !(g_shell->bits & (1 << 2)))
		{
			pgrp = pid;
			handle_bgproc(pid, ((t_ast *)elem->content)->args->argv, BG_RUN, 1);
			tcsetpgrp(0, pgrp);
			head = ft_lstend(g_shell->bgproc);
		}
		else if (!pgrp && g_shell->bits & (1 << 2))
			pgrp = -1;
		save_pid(head, pid);
		elem->content_size = pid;
		elem = elem->parent;
		if (elem && elem->next && elem->next->next)
			swap1(fd);
	}
	elem = ft_lstend(tabpipe);
	signal(SIGCHLD, SIG_DFL);
	int	i;

	i = 0;
	while (elem)
	{
		log_debug("WAITING...: %s pid: %d\n", ((t_ast *)elem->content)->name, elem->content_size);
		if (waitpid((pid_t)elem->content_size, &i, WUNTRACED) == -1)
			log_debug("waitpid == -1: %s\n", strerror(errno));
		log_debug("NAME_END: %s\n", ((t_ast *)elem->content)->name);
		elem = elem->parent;
	}
	if (WIFEXITED(i))
	{
		log_debug("Retour normal du process\n");
		elem = ft_lstend(g_shell->bgproc);
		if (elem == g_shell->bgproc)
			g_shell->bgproc = NULL;
		ft_lstdelone(&elem, del);
	}
	else
		log_debug("bgproc-> %s\n", ((t_inffork *)g_shell->bgproc->content)->cmd[0]);
	if (!(g_shell->bits & (1 << 2)))
		tcsetpgrp(0, getpgrp());
	signal(SIGCHLD, sign_child);
	ft_lstdelone(&tabpipe, NULL);
	g_shell->bits &= ~(1 << 1);
	return (0);
}*/
