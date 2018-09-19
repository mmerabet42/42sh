/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmerabet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/09 20:36:55 by mmerabet          #+#    #+#             */
/*   Updated: 2018/09/18 18:08:42 by mmerabet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include <unistd.h>
#include <sys/wait.h>
#include "ft_printf.h"
#include "ft_str.h"

static int		has_left_redir(t_ast *ast)
{
	if (!ast)
		return (0);
	if (ast->type == TK_REDIR && ft_strmatch(ast->name, "*<*"))
		return (1);
	else if (has_left_redir(ast->left) || has_left_redir(ast->right))
		return (1);
	return (0);
}

static pid_t	new_fork(t_ast *ast, void *res, t_iterf *iterf, int fd[3])
{
	pid_t	pid;
	pid_t	save;
	int		ir;

	if (!ast || !ast->name)
		return (-1);
	ir = (ast == ast->parent->right ? 1 : 0);
	if (ir && has_left_redir(ast))
		fd[2] = 1;
	if (!(pid = fork()))
	{
		save = getpgrp();
		close(fd[ir]);
		if (!fd[2])
			dup2(fd[!ir], !ir);
		handle_pgid();
		setpgid(0, 0);
		ft_astiter(ast, res, iterf);
		tcsetpgrp(0, save);
		close(fd[!ir]);
		exit(*(int *)res);
	}
	return (pid);
}

int				shell_pipe_cb(t_ast *ast, void **op, void *res, t_iterf *iterf)
{
	int		fd[3];
	pid_t	pidl;
	pid_t	pidr;
	int		leftres;
	int		rightres;

	(void)op;
	fd[2] = 0;
	if (pipe(fd) == -1)
		return (SH_PIPFAIL);
	if ((pidr = new_fork(ast->right, &rightres, iterf, fd)) == -1
			&& close(fd[1]) != 1 && close(fd[0]) != 1)
		return (SH_FORKFAIL);
	if (!fd[2] && (pidl = new_fork(ast->left, &leftres, iterf, fd)) == -1
			&& close(fd[1]) != 1 && close(fd[0]) != 1)
		return (SH_FORKFAIL);
	close(fd[1]);
	close(fd[0]);
	if (!fd[2])
		waitpid(pidl, NULL, 0);
	waitpid(pidr, res, 0);
	*(int *)res = WEXITSTATUS(*(int *)res);
	return (0);
}

int				shell_andor_seco_cb(t_ast *ast, void **op, void *res,
								t_iterf *iterf)
{
	int	efail;

	if (*ast->name == '|' || *ast->name == '&')
	{
		if ((*ast->name == '|' && *(int *)op[0] != 0)
				|| (*ast->name == '&' && *(int *)op[0] == 0))
			return (ft_astiter(ast->right, res, iterf));
		else
			*(int *)res = *(int *)op[0];
	}
	else if (*ast->name == ';' || *ast->name == ',' || *ast->name == '\n')
	{
		if ((efail = ft_astiter(ast->left, res, iterf)))
			return (efail);
		else if (ast->right && ast->right->name)
			return (ft_astiter(ast->right, res, iterf));
	}
	return (0);
}

int				shell_bckgrnd_cb(t_ast *ast, void **op, void *res,
							t_iterf *iterf)
{
	(void)iterf;
	(void)op;
	if (*ast->name == '&' && exec_cmd_background(ast, res, iterf) != 0)
		return (SH_FORKFAIL);
	return (0);
}
