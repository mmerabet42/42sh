/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jraymond <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/20 19:45:28 by jraymond          #+#    #+#             */
/*   Updated: 2018/10/04 21:17:58 by jraymond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "ft_str.h"
#include "ft_mem.h"
#include <sys/wait.h>

#include "ft_printf.h"
#include <fcntl.h>

#include "parser.h"

static void		print_tab_pipe(t_list *begin)
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
}

static void		swap1(int *fd)
{
	fd[0] = dup(fd[2]);
	fd[1] = dup(fd[3]);
	close(fd[2]);
	close(fd[3]);
	fd[2] = -1;
	fd[3] = -1;
}

static void			printfd(int *fd)
{
	int	x;

	x = -1;
	while (++x < 4)
		log_debug("fd[%d]: %d\n", x, fd[x]);
}

static int		son_action(int *fd, t_list *elem)
{
	if (!elem->next)
	{
		log_trace("debut -> pid: %d pgrp -> %d\n", getpid(), getpgrp());
		close(fd[0]);
		dup2(fd[1], 1);
	}
	else if (elem->next && elem->parent)
	{
		log_trace("milieu -> pid: %d pgrp -> %d\n", getpid(), getpgrp());
		close(fd[2]);
		close(fd[1]);
		dup2(fd[3], 1);
		dup2(fd[0], 0);
	}
	else
	{
		log_trace("fin -> pid: %d pgrp -> %d\n", getpid(), getpgrp());
		close(fd[1]);
		dup2(fd[0], 0);
	}
	return (0);
}

void			closefd(int *fd, t_list *elem)
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

int				shell_pipe_cb(t_ast *ast, void **op, void *res, t_iterf *iterf)
{
	t_list	*tabpipe;
	t_list	*elem;
	pid_t	pid;
	int		fd[4];
	int		pgrp;

	(void)op;
	g_shell->bits |= (1 << 1);
	tabpipe = NULL;
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
			signal(SIGTSTP, SIG_DFL);
			printfd(fd);
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
		}
		else if (!pgrp && g_shell->bits & (1 << 2))
			pgrp = -1;
		elem->content_size = pid;
		elem = elem->parent;
		if (elem && elem->next && elem->next->next)
			swap1(fd);
	}
	elem = ft_lstend(tabpipe);
	while (elem)
	{
		log_debug("WAITING...: %s pid: %d\n", ((t_ast *)elem->content)->name, elem->content_size);
		if (waitpid((pid_t)elem->content_size, res, WUNTRACED) == -1)
			log_debug("waitpid == -1\n");
		log_debug("NAME_END: %s\n", ((t_ast *)elem->content)->name);
		elem = elem->parent;
	}
	log_debug("ouiiiiiiiiii\n");
	if (!(g_shell->bits & (1 << 2)))
		tcsetpgrp(0, getpgrp());
	g_shell->bits &= ~(1 << 1);
	return (0);
}
