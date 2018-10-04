/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_pipe2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jraymond <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/20 19:45:28 by jraymond          #+#    #+#             */
/*   Updated: 2018/09/24 18:12:42 by mmerabet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "ft_str.h"
#include "ft_mem.h"
#include <sys/wait.h>

#include "ft_printf.h"
#include <fcntl.h>

t_list	*list_pipes(t_ast *ast)
{
	t_list	*head;

	head = NULL;
	while (ast->left)
	{
		ft_lstpushfront(&head, ft_lstcreate((void *)ast->right, 0));
		if (ast->left->type != TK_PIPE)
		{
			ft_lstpushfront(&head, ft_lstcreate((void *)ast->left, 0));
			break ;
		}
		ast = ast->left;
	}
	return (head);
}

void	start_fork(t_ast *ast, t_iterf *iterf, void *res, t_pipe *p)
{
	pid_t	pid;

	pid = getpid();
	if (!p->pgid)
		p->pgid = pid;
	setpgid(pid, p->pgid);
	tcsetpgrp(0, p->pgid);
	if (p->in != 0)
	{
		dup2(p->in, 0);
		close(p->in);
	}
	if (p->out != 1)
	{
		dup2(p->out, 1);
		close(p->out);
	}
	g_shell->bits |= (1 << 1);
	ft_astiter(ast, res, iterf);
	exit(0);
}

int		wait_pids(t_list *head, int *res, pid_t pgid)
{
	t_list	*it;

	tcsetpgrp(0, pgid);
	it = head;
	*(int *)res = 0;
	while (it)
	{
		waitpid((pid_t)it->content_size, res, WUNTRACED);
		*res = WEXITSTATUS(*res);
		it = it->next;
	}
	tcsetpgrp(0, g_shell->shellpid);
	ft_lstdel(&head, NULL);
	return (0);
}

int		process_pipe(t_list *it, t_pipe *p, void *res, t_iterf *iterf)
{
	pid_t	pid;
	int		fd[2];

	if (it->next && pipe(fd) == -1)
		return (SH_PIPFAIL);
	p->out = (it->next ? fd[1] : 1);
	if ((pid = fork()) == -1)
		return (SH_FORKFAIL);
	else if (!pid)
		start_fork((t_ast *)it->content, iterf, res, p);
	it->content_size = pid;
	setpgid(pid, (!p->pgid ? p->pgid = pid : p->pgid));
	if (p->in != 0)
		close(p->in);
	if (p->out != 1)
		close(p->out);
	p->in = fd[0];
	return (0);
}

int		shell_pipe_cb(t_ast *ast, void **op, void *res, t_iterf *iterf)
{
	t_list	*head;
	t_list	*it;
	t_pipe	p;
	int		err;

	(void)op;
	head = list_pipes(ast);
	it = head;
	p.in = 0;
	p.pgid = 0;
	while (it)
	{
		if ((err = process_pipe(it, &p, res, iterf)))
		{
			ft_lstdel(&head, NULL);
			return (err);
		}
		it = it->next;
	}
	return (wait_pids(head, (int *)res, p.pgid));
}
