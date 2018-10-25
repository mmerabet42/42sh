/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_hdoc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmerabet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/24 17:16:11 by mmerabet          #+#    #+#             */
/*   Updated: 2018/10/25 16:44:51 by mmerabet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "ft_io.h"
#include "ft_str.h"
#include "ft_mem.h"
#include "ft_types.h"
#include <fcntl.h>
#include "libedit.h"

static int	interpret_line(char *line, char *eof, t_expf *expf)
{
	char	*res;
	t_list	*lst;
	int		ret;

	ret = 0;
	lst = NULL;
	ft_strexpand(line, &lst, -1, expf);
	res = line;
	if (lst && lst->content)
		res = lst->content;
	if (ft_strequ(res, eof))
		ret = 1;
	ft_strcpy(line, res);
	ft_lstdel(&lst, content_delfunc);
	return (ret);
}

static int	in_loophdoc(char **line, t_list **lst, t_ast *ast)
{
	int	cursor;

	ft_putstr("heredoc> ");
	ft_getcursor(&cursor, NULL);
	if (!(*line = ft_loop_init(cursor, 0)))
		return (0);
	ft_putchar('\n');
	if (*line[0] == 3)
	{
		ft_strdel(line);
		ft_lstdel(lst, content_delfunc);
		while (ast->parent)
			if (ft_strmatch((ast = ast->parent)->name, "*<<"))
				return (2);
		return (!ast->parent ? 1 : 2);
	}
	return (0);
}

static int	go_hdoc(t_ast *ast, int fd, t_expf *expf)
{
	char	*line;
	char	*eof;
	t_list	*lst;
	int		ret;

	lst = NULL;
	ft_strexpand(ast->right->name, &lst, -1, expf);
	eof = ast->right->name;
	if (lst && lst->content)
		eof = lst->content;
	while (1)
	{
		if ((ret = in_loophdoc(&line, &lst, ast)) != 0)
			return (ret);
		if (interpret_line(line, eof, expf) || !ft_strcmp(line, "exit"))
			break ;
		ft_putendl_fd(line, fd);
		ft_strdel(&line);
	}
	ft_strdel(&line);
	ft_lstdel(&lst, content_delfunc);
	return (0);
}

static int	shell_create_file(t_ast *ast, void **op, void *res)
{
	int	fd;

	(void)op;
	(void)res;
	if (ft_strchr(ast->name, '>'))
	{
		fd = open(ast->right->name, O_CREAT, 0666);
		close(fd);
		return (1);
	}
	return (0);
}

int			shell_hdoc_cb(t_ast *ast, void **op, void *res, t_iterf *iterf)
{
	static int	n;
	int			fd;
	int			ret;
	char		*hdoc_file;

	*(t_ast **)res = ast;
	if (!ft_astvalid(ast->right))
		return (SH_BADEXPR);
	if (shell_create_file(ast, op, res))
		return (0);
	if (!(hdoc_file = ft_strjoin_clr(HDOC_TMP_FILE, ft_itoa(n++), 1)))
		return (SH_MALLOC);
	if ((fd = open(hdoc_file, O_WRONLY | O_TRUNC | O_CREAT, 0666)) == -1)
		return (SH_OPENFILE);
	if (!(ret = go_hdoc(ast, fd, ((t_allf *)iterf->data)->expf)))
	{
		ft_strstr(ast->name, "<<")[1] = '\0';
		*ft_memdel((void **)&ast->right->name) = hdoc_file;
		ast->right->args->argv[0] = ast->right->name;
	}
	else
		free(hdoc_file);
	close(fd);
	return (!ret || ret == 2 ? 0 : SH_HDOCSTOP);
}
