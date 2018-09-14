/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmerabet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/26 17:21:51 by mmerabet          #+#    #+#             */
/*   Updated: 2018/09/12 21:23:43 by mmerabet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "ft_printf.h"
#include "ft_io.h"
#include "ft_str.h"
#include "ft_mem.h"
#include <fcntl.h>
#include <sys/wait.h>
#include "../logger/incs/logger.h"

static void	restore_fds(void *c, size_t s)
{
	t_redir	*r;

	(void)s;
	r = (t_redir *)c;
	if (r->checked && r->fdz != -2)
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
		else
		{
			dup2(r->fdz, r->fda);
			close(r->fdz);
		}
	}
	else
		close(r->fdz);
	free(c);
}

static int	heredoc(t_redir *r, t_list *it)
{
	char	*line;
	int		fd_o;

	if (!r->file)
		return (SH_HDOCWORD);
	if ((fd_o = open(SHELL_TMP_FILE, O_WRONLY | O_TRUNC | O_CREAT, 0666)) == -1)
		return (SH_HDOCWFAIL);
	while (ft_printf_fd(0, "heredoc> ") && get_next_line(0, &line) >= 0)
	{
		if (ft_strequ(line, r->file) && ft_memdel((void **)&line))
			break ;
		ft_printf_fd(fd_o, "%s\n", line);
		ft_memdel((void **)&line);
	}
	close(fd_o);
	(void)it;
	while ((it = it->next))
		if (((t_redir *)it->content)->fda == r->fda)
			return (0);
	if ((fd_o = open(SHELL_TMP_FILE, O_RDONLY | O_CREAT, 0666)) == -1)
		return (SH_HDOCRFAIL);
//	r->fdz = dup(r->fda);
	dup2(fd_o, r->fda);
	close(fd_o);
	return (0);
}
/*
static void	restore_closed(t_redir *r, t_list *redirs, int *closed_fd, int fdo)
{
	t_redir *rb;

	if (r->fdz == -1)
	{
		while (redirs && (rb = redirs->content) != r)
		{
			if (rb->rep && (rb->fdb == -1 || (*closed_fd & (1 << rb->fdb))))
			{
			//	*closed_fd &= ~(1 << r->fda);
				dup2(rb->fdz, rb->fda);
				close(rb->fdz);
				rb->fdz = -2;
				r->fdz = dup(rb->fda);
				dup2(fdo, r->fda);
				break ;
			}
			redirs = redirs->next;
		}
	}
	close(fdo);
}
*/
static int	dup_file(t_redir *r, t_list *redirs, int *closed_fd)
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
	(void)redirs;
	(void)closed_fd;
//	if ((r->fdz = dup(r->fda)) != -1)
		dup2(fd_o, r->fda);
	close(fd_o);
//	restore_closed(r, redirs, closed_fd, fd_o);
	return (0);
}

static void	store_fds(t_list *elem)
{
	t_redir	*r;

	r = (t_redir *)elem->content;
	r->fdz = dup(r->fda);
}

int			shell_redir_cb(t_ast *ast, void **op, void *res, t_iterf *iterf)
{
	t_list	*redirs;
	t_redir	*r;
	t_list	*it;
	int		i[2];

	redirs = list_redirections(&ast, ((t_allf *)iterf->data)->expf);
	ft_lstiter((it = redirs), store_fds);
	ft_bzero(i, sizeof(int) * 2);
	while (it && (r = (t_redir *)it->content))
	{
		if ((r->rep && (i[1] = replace_fd(r, &i[0]))) || (!r->rep
				&& ((r->type == TK_LLEFT && (i[1] = heredoc(r, it)))
					|| (r->type != TK_LLEFT && (i[1] = dup_file(r, redirs, &i[0]))))))
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
