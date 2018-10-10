/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_hdoc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmerabet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/24 17:16:11 by mmerabet          #+#    #+#             */
/*   Updated: 2018/09/28 15:39:40 by sle-rest         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "ft_io.h"
#include "ft_str.h"
#include "ft_mem.h"
#include "ft_types.h"
#include <fcntl.h>
#include "libedit.h"

static int	end_sig(t_list *lst, int c, t_ast *ast)
{
	ft_lstdel(&lst, content_delfunc);
	while (c == 3 && ast->parent)
		if (ft_strmatch((ast = ast->parent)->name, "*<<"))
			return (2);
	return (c == 4 || !ast->parent ? 1 : 2);
}

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

static int	go_hdoc(t_ast *ast, int fd, t_expf *expf)
{
	char	*line;
	char	*eof;
	t_list	*lst;
	int		cursor;

	lst = NULL;
	ft_strexpand(ast->right->name, &lst, -1, expf);
	eof = ast->right->name;
	if (lst && lst->content)
		eof = lst->content;
	while (1)
	{
		ft_putstr("heredoc> ");
		ft_getcursor(&cursor, NULL);
		if (!(line = ft_loop_init(cursor, 0)))
			break ;
		ft_putchar('\n');
		if (*line == 3 ||  !ft_strcmp(line, "exit"))
			return (end_sig(lst, *line == 3 ? 3 : 4, ast));
		if (interpret_line(line, eof, expf))
			break ;
		ft_putendl_fd(line, fd);
	}
	ft_lstdel(&lst, content_delfunc);
	return (0);
}

int			shell_hdoc_cb(t_ast *ast, void **op, void *res, t_iterf *iterf)
{
	static int	n;
	int			fd;
	int			ret;
	char		*hdoc_file;

	(void)op;
	(void)res;
	*(t_ast **)res = ast;
	if (!ft_astvalid(ast->right))
		return (SH_BADEXPR);
	if (!(hdoc_file = ft_strjoin_clr(HDOC_TMP_FILE, ft_itoa(n++), 1)))
		return (SH_MALLOC);
	if ((fd = open(hdoc_file, O_WRONLY | O_TRUNC | O_CREAT, 0666)) == -1)
		return (SH_OPENFILE);
	if (!(ret = go_hdoc(ast, fd, (t_expf *)iterf->data)))
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
