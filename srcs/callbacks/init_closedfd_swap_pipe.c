/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_closedfd_swap_pipe.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jraymond <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/24 12:01:19 by jraymond          #+#    #+#             */
/*   Updated: 2018/10/24 13:15:38 by jraymond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "job_control.h"
#include "ft_str.h"
#include "ft_mem.h"
#include <sys/wait.h>

#include "ft_printf.h"
#include <fcntl.h>

#include "parser.h"

void		swap1(int *fd)
{
	fd[0] = dup(fd[2]);
	fd[1] = dup(fd[3]);
	close(fd[2]);
	close(fd[3]);
	fd[2] = -1;
	fd[3] = -1;
}

void		closefd(int *fd, t_list *elem)
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

int			init_struct(t_pipe *pipe, t_ast *ast)
{
	int		x;
	int		ret;

	x = -1;
	g_shell->bits |= (1 << 1);
	pipe->all_cmd = NULL;
	ft_bzero(pipe, sizeof(t_pipe));
	while (++x < 4)
		pipe->fd[x] = -1;
	if ((ret = handle_ast_pipe(ast, &pipe->tabpipe)))
		return (ret);
	ret_pipecmd(pipe->tabpipe, &pipe->allcmmd);
	return (0);
}

int			init_struct_bq(t_pipe *pipe, t_ast *ast)
{
	int		x;
	int		ret;

	x = -1;
	g_shell->bits |= (1 << 1);
	pipe->all_cmd = NULL;
	ft_bzero(pipe, sizeof(t_pipe));
	while (++x < 4)
		pipe->fd[x] = -1;
	if ((ret = handle_ast_pipe(ast, &pipe->tabpipe)))
		return (ret);
	return (0);
}
