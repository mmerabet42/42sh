/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_background.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jraymond <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/10 17:42:32 by jraymond          #+#    #+#             */
/*   Updated: 2018/09/12 21:15:54 by mmerabet         ###   ########.fr       */
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

#include "../logger/incs/logger.h"
/*
static void prout(int sig)
{
	log_debug("OOOK SIGNAL: '%d'\n", sig);
}
*/
void			son_fork(t_ast *ast, void *res, t_iterf *iterf)
{
	char		**args;
	t_list		*elem;
	pid_t		pid;

	signal(SIGINT, SIG_DFL);
	signal(SIGTTIN, SIG_DFL);
	signal(SIGTTOU, SIG_IGN);
	pid = getpid();
//	setpgid(0, 0);
	args = ret_args(ast);
	if (handle_bgproc(pid, args, BG_RUN) == -1)
		exit(126);
	elem = ft_lstend(g_shell->bgproc);
	ft_printf("[%d] %d\n", ((t_inffork *)elem->content)->x, pid);
	if (ast->left->type != ast->left->cmd_offset
			|| ast->left->args->argv[0][0] == '('
			|| ast->left->args->argv[0][0] == '{')
	{
		ft_astiter(ast->left, res, iterf);
		exit(0);
	}
	exec_btin_bin(ret_astargs(ast), res, iterf);
	exit(0);
}

int				exec_cmd_background(t_ast *ast, void *res, t_iterf *iterf)
{
	pid_t		pid;
	t_inffork	inf;

	ft_bzero(&inf, sizeof(t_inffork));
	signal(SIGTTOU, SIG_IGN);
	tcsetpgrp(0, getpgrp());
	signal(SIGTTOU, SIG_DFL);
	if ((pid = fork()) == -1)
		return (SH_FORKFAIL);
	if (!pid)
		son_fork(ast, res, iterf);
	else
	{
		tcsetpgrp(0, getpgrp());
		handle_bgproc(pid, ret_args(ast), BG_RUN);
		ft_astiter(ast->right, res, iterf);
	}
	return (0);
}
