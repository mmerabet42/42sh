/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_expansions1_v.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmerabet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/16 20:01:35 by mmerabet          #+#    #+#             */
/*   Updated: 2018/10/12 11:05:10 by jraymond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmerabet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/14 17:21:45 by mmerabet          #+#    #+#             */
/*   Updated: 2018/10/11 20:13:51 by jraymond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "ft_io.h"
#include "ft_str.h"
#include <sys/wait.h>

static int		exp_cmd1(int fd[2], t_list **res, int mode, pid_t pid)
{
	char	*line;
	char	*end;
	int		ret;

	(void)pid;
	close(fd[1]);
	waitpid(pid, &ret, WUNTRACED);
	if (mode == -1)
	{
		get_next_delimstr(fd[0], EOF_NEVER_REACH, &line);
		end = ft_strend(line);
		while (end != line && *end == '\n')
			*end-- = '\0';
		*res = ft_lstcreate(line, 0);
		line = NULL;
	}
	else
	{
		while (get_next_line(fd[0], &line) >= 0)
		{
			ft_lstpush_p(res, ft_strsplitpbrk_lst(line, " \t"));
			free(line);
		}
	}
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

int				exp_cmd(t_strid *sid, t_list **res, t_expf *expf)
{
	int		fd[2];
	int		efail;
	t_ast	*head;
	pid_t	pid;

	if (pipe(fd) == -1)
		return (SH_PIPFAIL);
	if ((pid = fork()) == -1 && !close(fd[0]) && !close(fd[1]))
		return (SH_FORKFAIL);
	else if (!pid && !(sid->str[sid->len - 1] = '\0') && !close(fd[0]))
	{
		dup2(fd[1], 1);
		resetsign();
		g_shell->bits |= (1 << 4);
		head = lexer_iter(sid->str, expf, &efail);
		ft_astdel(&head);
		close(fd[1]);
		exit(efail ? 1 : 0);
	}
	g_shell->curpid = pid;
	return (exp_cmd1(fd, res, sid->i, pid));
}

int				exp_dvar(t_strid *sid, t_list **res, t_expf *expf)
{
	char	*str;
	char	*final;
	int		efail;
	t_list	*lst;

	str = sid->str + 2;
	lst = NULL;
	sid->str[sid->len - 1] = '\0';
	efail = ft_strexpand(str, &lst, -1, expf);
	sid->str[sid->len - 1] = '}';
	if (efail || !lst || !lst->content)
	{
		ft_lstdel(&lst, content_delfunc);
		return (efail);
	}
	final = ft_strnew(ft_strlen(lst->content) + 1);
	ft_strcpy(final + 1, lst->content);
	final[0] = '$';
	*res = NULL;
	efail = ft_strexpand(final, res, sid->j, expf);
	ft_lstdel(&lst, content_delfunc);
	free(final);
	return (efail);
}
