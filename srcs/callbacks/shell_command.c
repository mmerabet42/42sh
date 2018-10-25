/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jraymond <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/30 18:18:40 by mmerabet          #+#    #+#             */
/*   Updated: 2018/10/25 18:05:12 by mmerabet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "globing.h"
#include "ft_str.h"
#include "ft_printf.h"
#include "get_next_line.h"
#include "ft_mem.h"
#include <sys/stat.h>
#include <sys/wait.h>

static int	shell_cmd_cb2(char *buff, void *res, t_ast *ast, t_args *args)
{
	struct stat	st;
	int			i;

	if ((i = ft_getfullpath(ast->cname, g_shell->paths, buff, 1024)) != SH_OK)
	{
		ft_printshret(i, ast->cname);
		*(int *)res = (i == SH_ADENIED ? 126 : 127);
		g_shell->curargs = args;
		return (0);
	}
	if (!(i = (stat(buff, &st) ? 0 : S_ISREG(st.st_mode)))
			&& (*(int *)res = 128))
		ft_printshret(SH_NFILE, ast->cname);
	else if ((*(int *)res = ft_exec(buff, ast->cargs->argv, g_shell->envp,
					&g_shell->curpid)) == -1)
	{
		g_shell->curargs = args;
		return (SH_FORKFAIL);
	}
	if (*(int *)res == 126)
		ft_printshret(SH_EXECERR, ast->cname);
	g_shell->curargs = args;
	return (0);
}

static t_exp		g_exps[] = {
	{"*[$(?);(?);`?`;${?};\"*\";'*'@b]", exp_cmd},
	{"*[`?`;$(?);${?};\"*\";'*'@b]", exp_cmd},
	{"*[\"*\"@b]:'*':$'*'", exp_quote},
	{"*[\"'@=1]*[@>0]:$'*[@>0]:\":':$'", exp_quote},
	{"", exp_glob}
};

static t_expf		g_expf = {
	g_exps, sizeof(g_exps), NULL, 0
};

int			shell_cmd_cb(t_ast *ast, void **op, void *res, t_iterf *iterf)
{
	int		ret;
	char	buff[2048];
	t_args	*args;

	(void)op;
	if ((ret = ft_astcresolver(ast, ((t_allf *)iterf->data)->expf)))
		return ((*(int *)res = 1) ? ret : ret);
	g_expf.data = iterf->data;
	if ((ret = ft_astresolver(ast, &g_expf)))
		return ((*(int *)res = 1) ? ret : ret);
	if (!ast || !ast->cname || !*ast->cname)
		return (0);
	args = g_shell->curargs;
	g_shell->curargs = ast->cargs;
	if ((ret = check_function(ast, res, iterf)) != -1
			&& (g_shell->curargs = args))
		return (ret);
	if (!execbuiltin(ast->cname, ast->cargs, (int *)res)
			&& (g_shell->curargs = args))
		return (g_shell->running ? 0 : SH_EXIT);
	return (shell_cmd_cb2(buff, res, ast, args));
}

static int	in_parent(int fd[2], void *res)
{
	int		efail;
	char	*mem;

	efail = 0;
	wait(res);
	tcsetpgrp(0, g_shell->shellpid);
	close(fd[1]);
	get_next_line(fd[0], &mem);
	if (mem)
	{
		ft_memcpy(&efail, mem, sizeof(int));
		ft_memdel((void **)&mem);
	}
	get_next_line(fd[0], &mem);
	if (mem)
	{
		ft_memcpy(res, mem, sizeof(int));
		ft_memdel((void **)&mem);
	}
	close(fd[0]);
	return (efail);
}

static int	lists_subshell(t_ast *head, void *res, t_iterf *iterf)
{
	pid_t	pid;
	int		fd[2];
	int		efail;

	efail = 0;
	if (pipe(fd) == -1)
		return (SH_PIPFAIL);
	if ((pid = fork()) == -1)
		return (SH_FORKFAIL);
	else if (!pid)
	{
		setpgid(0, 0);
		tcsetpgrp(0, getpgrp());
		efail = ft_astiter(head, res, iterf);
		close(fd[0]);
		ft_printf_fd(fd[1], "%.*r\n%.*r", sizeof(int), &efail,
				sizeof(int), res);
		close(fd[1]);
		ft_astdel(&head);
		exit(0);
	}
	setpgid(pid, pid);
	g_shell->curpid = pid;
	return (in_parent(fd, res));
}

int			shell_lists_cb(t_ast *ast, void **op, void *res, t_iterf *iterf)
{
	char	*ptr;
	char	tmp;
	int		efail;
	t_ast	*head;

	(void)op;
	g_shell->bits &= ~(1 << 1);
	ptr = ft_strend(ast->name);
	tmp = *ptr;
	*ptr = '\0';
	g_shell->subshell = ast->name + 1;
	head = ft_lexer(ast->name + 1, ast->lexerf);
	*ptr = tmp;
	efail = 0;
	if (!head)
		return (0);
	if (*ast->name == '{')
		efail = ft_astiter(head, res, iterf);
	else if ((efail = lists_subshell(head, res, iterf)))
		efail = (efail == SH_EXIT ? 0 : efail);
	g_shell->subshell = NULL;
	ft_astdel(&head);
	return (efail);
}
