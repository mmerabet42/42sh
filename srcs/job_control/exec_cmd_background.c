/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_background.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jraymond <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/10 17:42:32 by jraymond          #+#    #+#             */
/*   Updated: 2018/10/26 19:44:18 by sle-rest         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "ft_io.h"
#include "ft_str.h"
#include "ft_mem.h"
#include "ft_list.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <signal.h>
#include "job_control.h"
#include "parser.h"

static void		son_fork(t_ast *ast, void *res, t_iterf *iterf)
{
	char		**args;
	t_list		*elem;
	pid_t		pid;

	resetsign();
	pid = getpid();
	args = ret_args(ast);
	if (handle_bgproc(pid, args, BG_RUN, 1) == -1)
		exit(126);
	elem = ft_lstend(g_shell->bgproc);
	ft_printf("[%d] %d\n", ((t_inffork *)elem->content)->x, pid);
	g_shell->bits |= (1 << 1);
	g_shell->bits |= (1 << 2);
	ft_astiter(ast->left, res, iterf);
	exit(0);
}

static int		is_pipe1(t_ast *ast, void *res, t_iterf *iterf)
{
	g_shell->bits |= (1 << 1);
	g_shell->bits |= (1 << 2);
	ft_astiter(ast->left, res, iterf);
	ft_astiter(ast->right, res, iterf);
	return (0);
}

static int		father_forkbg(pid_t pid, t_ast *ast, void *res, t_iterf *iterf)
{
	t_list	*lstargs;
	char	*allcmmd;

	if (ast->left->type == TK_PIPE)
	{
		if (handle_ast_pipe(ast, &lstargs))
			return (SH_MALLOC);
		ret_pipecmd(lstargs, &allcmmd);
		if (handle_bgproc(pid, &allcmmd, BG_RUN, 1) != 0)
			return (SH_MALLOC);
	}
	else
	{
		if (handle_bgproc(pid, ret_args(ast), BG_RUN, 1) != 0)
			return (SH_MALLOC);
	}
	ft_astiter(ast->right, res, iterf);
	return (0);
}

int				exec_cmd_background(t_ast *ast, void *res, t_iterf *iterf)
{
	pid_t		pid;
	t_inffork	inf;

	ft_bzero(&inf, sizeof(t_inffork));
	if (ast->left->type == TK_PIPE)
		return (is_pipe1(ast, res, iterf));
	else
	{
		if ((pid = fork()) == -1)
			return (SH_FORKFAIL);
		if (!pid)
		{
			setpgid(0, 0);
			son_fork(ast, res, iterf);
		}
		else
		{
			setpgid(pid, 0);
			if (father_forkbg(pid, ast, res, iterf) != 0)
				return (SH_MALLOC);
		}
	}
	return (0);
}
