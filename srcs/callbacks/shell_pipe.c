/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_pipe2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jraymond <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/20 19:45:28 by jraymond          #+#    #+#             */
/*   Updated: 2018/10/02 19:35:39 by jraymond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "ft_str.h"
#include "ft_mem.h"
#include <sys/wait.h>

#include "ft_printf.h"
#include <fcntl.h>

#include "parser.h"

/*static int		check_cond(t_ast *ast, int type)
{
	if (type)
	{
		if (!ast->parent || !ast->parent->name
				|| !ft_strequ(ast->parent->name, "|"))
			return (1);
		else if (ft_strequ(ast->left->name, "|"))
			return (1);
		return (0);
	}
	if (!ast)
		return (0);
	if (ast->type == TK_REDIR && ft_strmatch(ast->name, "*<*"))
		return (1);
	else if (check_cond(ast->left, 0) || check_cond(ast->right, 0))
		return (1);
	return (0);
}

static pid_t	son_fork(pid_t pgrp, int fd[3], t_ast *ast, t_iterf *iterf)
{
	int	ir;
	int	pid;
	int	res;

	ir = (ast == ast->parent->right ? 1 : 0);
	if (!(pid = fork()))
	{
		setpgid(0, (!pgrp ? 0 : pgrp));
		close(fd[ir]);
		if (!fd[2])
			dup2(fd[!ir], !ir);
		g_shell->bits |= (1 << 1);
		ft_astiter(ast, &res, iterf);
		close(fd[!ir]);
		exit(res);
	}
	return (pid);
}*/

/*static pid_t	new_fork(t_ast *ast, t_iterf *iterf, int fd[3])
{
	pid_t			pid;
	int				ir;
	short			a;
	static pid_t	pgrp;

	ir = (ast == ast->parent->right ? 1 : 0);
	a = !(pgrp) ? 1 : 0;
	if (ir && check_cond(ast, 0))
		fd[2] = 1;
	if ((g_shell->bits & (1 << 2)))
		pgrp = getpgrp();
	pid = son_fork(pgrp, fd, ast, iterf);
	setpgid(pid, (!pgrp ? (pgrp = pid) : pgrp));
	if (!(g_shell->bits & (1 << 2)) && a)
		tcsetpgrp(0, pgrp);
	g_shell->curpid = pgrp;
	if (!ir && ast->type != TK_PIPE)
		pgrp = 0;
	return (pid);
}*/

/*static int		exec_left(int fd[3], pid_t pids[2], t_ast *ast, t_iterf *iterf)
{
	int	m;
	int	res;

	if (!fd[2] && check_cond(ast, 1))
	{
		if ((pids[0] = new_fork(ast->left, iterf, fd)) == -1)
			if (!close(fd[0]) && !close(fd[1]))
				return (SH_FORKFAIL);
		if (!fd[2])
			waitpid(pids[0], NULL, WUNTRACED);
	}
	else if (!fd[2])
	{
		m = dup(1);
		close(fd[0]);
		dup2(fd[1], 1);
		g_shell->bits |= (1 << 1);
		ft_astiter(ast->left, &res, iterf);
		dup2(m, 1);
		g_shell->bits &= ~(1 << 1);
	}
	return (0);
}

static void		print_tab_pipe(t_list *begin)
{
	t_list	*elem;

	elem = begin;
	while (elem)
	{
		if (!elem->content)
			log_debug("NULL NULL\n");
		else
			log_debug("%s\n", ((t_ast *)elem->content)->name);
		elem = elem->next;
	}
}*/

/*static pid_t	new_fork(t_ast *ast, t_iterf *iterf, int fd[3], int *a)
{
	pid_t			pid;
	int				ir;
	short			b;
	static pid_t	pgrp;

	b = !(pgrp) ? 1 : 0;
	if (!pgrp)
		pgrp = getpgrp();
	son_fork(pgrp, fd, ast, iterf);
	setpgid(pid, (!pgrp ? (pgrp = pid) : pgrp));
	if (!(g_shell->bits & (1 << 2)) && b)
		tcsetpgrp(0, pgrp);
	g_shell->curpid = pgrp;
	return (pid);
}*/

static void		swap1(int *fd)
{
	fd[0] = fd[2];
	fd[1] = fd[3];
	fd[2] = -1;
	fd[3] = -1;
}

static int		son_action(int *fd, int *a, t_list *elem)
{
	if (!a && fd[2] == -1)
	{
		close(fd[0]);
		dup2(fd[1], 0);
	}
	else
	{
		if (fd[2] != -1 && elem->next)
		{
			close(fd[2]);
			close(fd[1]);
			dup2(fd[3], 0);
			dup2(fd[0], 1);
		}
		else
		{
			close(fd[1]);
			dup2(fd[0], 1);
		}
	}
	(*a)++;
	return (0);
}

int				shell_pipe_cb(t_ast *ast, void **op, void *res, t_iterf *iterf)
{
	t_list	*tabpipe;
	t_list	*elem;
	pid_t	pid;
	int		fd[4];
	int		a;
	static int	first;

	(void)op;
	g_shell->bits |= (1 << 1);
	if ((a = handle_ast_pipe(ast, &tabpipe)))
		return (a);
	elem = tabpipe;
	a = 0;
	while (elem)
	{
		a = a == 2 ? 0 : a;
		if (!a && pipe(fd) == -1)
			return (SH_PIPFAIL);
		else if (elem->next)
			if (pipe((fd + 2)) == -1)
				return (SH_PIPFAIL);
		if (!(pid = fork()))
		{
			son_action(fd, &a, elem);
			ft_astiter((t_ast *)elem->content, res, iterf);
			exit(*(int *)res);
		}
		else if (pid == -1)
			return (SH_FORKFAIL);
//		setpgid(pid, (!pgrp ? (pgrp = pid) : pgrp));
		if (!first && ++first)
			waitpid(pid, res, WUNTRACED);
		if (a == 2 && elem->next)
			swap1(fd);
		elem = elem->next;
	}
	return (0);
}

/*int				shell_pipe_cb(t_ast *ast, void **op, void *res, t_iterf *iterf)
{
	int		fd[3];
	pid_t	pids[2];
	t_list	*tab_pipe;
	int		ret;
	static int	tutu;

	(void)op;
	tab_pipe = NULL;
	ft_bzero(fd, sizeof(int) * 3);
	if ((ret = handle_ast_pipe(ast, &tab_pipe)))
		return (ret);
	if (!tutu)
		print_tab_pipe(tab_pipe);
	tutu++;
	if (pipe(fd) == -1)
		return (SH_PIPFAIL);
	if ((pids[1] = new_fork(ast->right, iterf, fd)) == -1)
		if (!close(fd[0]) && !close(fd[1]))
			return (SH_FORKFAIL);
	if (exec_left(fd, pids, ast, iterf))
		return (SH_FORKFAIL);
	close(fd[0]);
	close(fd[1]);
	waitpid(pids[1], res, WUNTRACED);
	*(int *)res = WEXITSTATUS(*(int *)res);
	if (!(g_shell->bits & (1 << 2)))
		tcsetpgrp(0, g_shell->shellpid);
	return (0);
}*/
