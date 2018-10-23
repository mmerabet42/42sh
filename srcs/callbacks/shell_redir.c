/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmerabet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/26 17:21:51 by mmerabet          #+#    #+#             */
/*   Updated: 2018/10/23 19:38:20 by mmerabet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "ft_printf.h"
#include "ft_io.h"
#include "ft_str.h"
#include "ft_mem.h"
#include <fcntl.h>
#include <sys/wait.h>

static int	g_max_fd;

static void	restore_fds(void *c, size_t s)
{
	t_redir	*r;

	(void)s;
	r = (t_redir *)c;
	if (!(g_max_fd = 0) && r->fdz == -2)
		close(r->fda);
	else if (r->checked)
	{
		if (r->rep)
		{
			if (r->fdb == -1)
			{
				if (r->fdz != -1)
					dup2(r->fdz, r->fda);
				close(r->fdz == -1 ? r->fda : r->fdz);
			}
			else if (r->fda != r->fdb && dup2(r->fdz, r->fda) != -2)
				close(r->fdz);
		}
		else if (dup2(r->fdz, r->fda) != -1)
			close(r->fdz);
	}
	else
		close(r->fdz);
	free(c);
}

static int	dup_file(t_redir *r)
{
	int	fd_o;
	int	fd_k;
	int	bits;

	bits = 0;
	if (!r->file)
		return (SH_BADEXPR);
	if (r->type == TK_LRIGHT)
		bits = (O_APPEND | O_WRONLY | O_CREAT);
	else if (r->type == TK_RIGHT)
		bits = (O_WRONLY | O_TRUNC | O_CREAT);
	else if (r->type == TK_LEFT)
		bits = (O_RDONLY);
	if ((fd_o = open(r->file, bits, 0666)) == -1)
		return (SH_OPENFILE);
	if ((fd_k = fd_o) == r->fda)
	{
		fd_o = dup(fd_o);
		close(fd_k);
	}
	dup2(fd_o, r->fda);
	close(fd_o);
	return (0);
}

static void	store_fds(t_list *elem)
{
	t_list	*begin;
	t_redir	*r;

	begin = elem;
	while (elem)
	{
		r = (t_redir *)elem->content;
		if (g_max_fd < r->fda + 100)
			g_max_fd = r->fda + 100;
		elem = elem->next;
	}
	elem = begin;
	while (elem)
	{
		r = (t_redir *)elem->content;
		if ((r->fdz = dup2(r->fda, g_max_fd)) == -1)
			r->fdz = -2;
		g_max_fd++;
		elem = elem->next;
	}
}

static int	heredoc(t_redir *r)
{
	if (!r->file)
		return (SH_HDOCWORD);
	return (0);
}

int			shell_redir_cb(t_ast *ast, void **op, void *res, t_iterf *iterf)
{
	t_list	*redirs;
	t_redir	*r;
	t_list	*it;
	int		i[2];

	redirs = list_redirections(&ast, ((t_allf *)iterf->data)->expf);
	store_fds(redirs);
	it = redirs;
	ft_bzero(i, sizeof(int) * 2);
	while (it && (r = (t_redir *)it->content))
	{
		if ((r->rep && (i[1] = replace_fd(r, &i[0]))) || (!r->rep
				&& ((r->type == TK_LLEFT && (i[1] = heredoc(r)))
					|| (r->type != TK_LLEFT && (i[1] = dup_file(r))))))
			break ;
		r->checked = 1;
		it = it->next;
	}
	if (i[1] && i[1] != SH_BADEXPR && (*(int *)res = 1))
		redir_printerror(r, i[1], op);
	else if (!i[1])
		ft_astiter(ast, res, iterf);
	redirs = ft_lstend(redirs);
	ft_lstdelv(&redirs, restore_fds);
	return (i[1] == SH_BADEXPR ? i[1] : 0);
}
