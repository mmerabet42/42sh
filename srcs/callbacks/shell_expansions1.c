/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_expansions1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmerabet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/16 14:52:54 by mmerabet          #+#    #+#             */
/*   Updated: 2018/09/14 16:38:16 by mmerabet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "ft_io.h"
#include "ft_str.h"
#include <sys/wait.h>
#include <fcntl.h>

static int		exp_cmd2(int fd[2], char **res)
{
	char	*line;
	char	*all;

	wait(NULL);
	close(fd[1]);
	all = NULL;
	while (get_next_line(fd[0], &line) >= 0)
	{
		if (all)
			all = ft_strjoinc_clr(all, '\n');
		all = ft_strjoin_clr(all, line, 2);
	}
	*res = all;
	close(fd[0]);
	return (0);
}

static t_ast	*lexer_iter(char *str, t_expf *ef, int *efail)
{
	t_ast	*ast;

	str += (*str == '$' ? 2 : 1);
	ast = ft_lexer(str, ((t_allf *)ef->data)->lexerf);
	if ((*efail = ft_astiter(ast, efail, ((t_allf *)ef->data)->iterf)))
		ft_printf_fd(2, "%s: error in command substitution: %s: [%s]\n",
			g_shell->name, ft_strshret(*efail), str);
	return (ast);
}

int				exp_cmd(t_strid *sid, char **res, t_expf *expf)
{
	int		fd[2];
	int		efail;
	t_ast	*head;
	pid_t	pid;
	pid_t	save;

	save = getpgrp();
	if (pipe(fd) == -1)
		return (SH_PIPFAIL);
	if ((pid = fork()) == -1 && !close(fd[0]) && !close(fd[1]))
		return (SH_FORKFAIL);
	else if (!pid && !(sid->str[sid->len - 1] = '\0') && !close(fd[0]))
	{
		dup2(fd[1], 1);
		handle_pgid();
		head = lexer_iter(sid->str, expf, &efail);
		ft_astdel(&head);
		tcsetpgrp(0, save);
		close(fd[1]);
		exit(efail ? 1 : 0);
	}
	return (exp_cmd2(fd, res));
}

int				exp_dvar(t_strid *sid, char **res, t_expf *expf)
{
	char	*str;
	char	*final;
	char	*ores;
	int		efail;

	str = sid->str + 2;
	ores = NULL;
	sid->str[sid->len - 1] = '\0';
	efail = ft_strexpand(str, &ores, 0, expf);
	sid->str[sid->len - 1] = '}';
	if (efail)
		return (efail);
	final = ft_strnew(ft_strlen(ores) + 1);
	ft_strcpy(final + 1, ores);
	final[0] = '$';
	free(ores);
	*res = NULL;
	efail = ft_strexpand(final, res, sid->j, expf);
	free(final);
	return (efail);
}
#include "../../logger/incs/logger.h"
static int		exp_cmd3(int fd[2], t_list **res)
{
	char	*line;

	wait(NULL);
	close(fd[1]);
	while (get_next_line(fd[0], &line) >= 0)
		ft_lstpush_p(res, ft_lstcreate(line, 0));
	close(fd[0]);
	return (0);
}

static t_ast	*lexer_iter2(char *str, t_expf2 *ef, int *efail)
{
	t_ast	*ast;

	str += (*str == '$' ? 2 : 1);
	ast = ft_lexer(str, ((t_allf *)ef->data)->lexerf);
	if ((*efail = ft_astiter(ast, efail, ((t_allf *)ef->data)->iterf)))
		ft_printf_fd(2, "%s: error in command substitution: %s: [%s]\n",
			g_shell->name, ft_strshret(*efail), str);
	return (ast);
}

int				exp_cmd1(t_strid2 *sid, t_list **res, t_expf2 *expf)
{
	int		fd[2];
	int		efail;
	t_ast	*head;
	pid_t	pid;
	pid_t	save;

	save = getpgrp();
	if (pipe(fd) == -1)
		return (SH_PIPFAIL);
	if ((pid = fork()) == -1 && !close(fd[0]) && !close(fd[1]))
		return (SH_FORKFAIL);
	else if (!pid && !(sid->str[sid->len - 1] = '\0') && !close(fd[0]))
	{
		dup2(fd[1], 1);
		handle_pgid();
		head = lexer_iter2(sid->str, expf, &efail);
		ft_astdel(&head);
		tcsetpgrp(0, save);
		close(fd[1]);
		exit(efail ? 1 : 0);
	}
	return (exp_cmd3(fd, res));
}
#include "ft_types.h"
int			exp_quote2(t_strid2 *sid, t_list **res, t_expf2 *expf)
{
	char	sep;

	(void)expf;
	if (sid->len == 1)
		return ((*res = NULL) ? 0 : 0);
	if ((sep = sid->str[sid->len - 1]) == '\'' || sep == '"')
		sid->str[sid->len - 1] = '\0';
	if (*sid->str == '$')
		*res = ft_lstcreate(ft_strdupk(sid->str + 2), 0);
	else
		*res = ft_lstcreate(ft_strdup(sid->str + 1), 0);
	sid->str[sid->len - 1] = sep;
	return (0);
}

int			exp_var2(t_strid2 *sid, t_list **res, t_expf2 *expf)
{
	char	*value;
	char	tmp;

	(void)expf;
	if (*sid->str == '\\')
	{
		*res = ft_lstcreate(ft_strndup(sid->str + 1, 1), 0);
		return (0);
	}
	tmp = sid->str[sid->len];
	sid->str[sid->len] = '\0';
	if (!ft_isalpha(*(sid->str + 1)) && !ft_isdigit(*(sid->str + 1)))
	{
		if (*(sid->str + 1) == '?')
			*res = ft_lstcreate(ft_itoa(g_shell->exitcode), 0);
	}
	else if ((value = ft_getenv(sid->str + 1, g_shell->envp)))
		*res = ft_lstcreate(ft_strdup(value), 0);
	sid->str[sid->len] = tmp;
	return (0);
	ft_printf("var expansion: '%.*s' %d %d %d\n", sid->len, sid->str,
			sid->len, sid->i, sid->j);
}

int			exp_tild2(t_strid2 *sid, t_list **res, t_expf2 *expf)
{
	char	*name;

	(void)expf;
	name = NULL;
	if (!sid->next_str || *sid->next_str == '/')
		name = "HOME";
	else if (sid->next_str && (*sid->next_str == '+' || *sid->next_str == '-'))
	{
		if ((!sid->next_str[1] || sid->next_str[1] == '/') && !sid->j)
		{
			sid->jump = 1;
			name = (*sid->next_str == '+' ? "PWD" : "OLDPWD");
		}
	}
	if (name && !sid->j)
		*res = ft_lstcreate(ft_strdup(ft_getenv(name, g_shell->envp)), 0);
	else
		*res = sid->next;
	return (0);
}
