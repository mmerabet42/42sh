/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redparser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmerabet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/26 16:10:49 by mmerabet          #+#    #+#             */
/*   Updated: 2018/09/15 23:19:01 by mmerabet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "ft_str.h"
#include "ft_types.h"
#include "ft_mem.h"
#include "ft_io.h"

#include "../../logger/incs/logger.h"

static t_list	*implement_args(t_ast *cmd, t_ast *red, t_redir *r,
							t_expf *expf)
{
	char	**ptr;
	char	*s;
	t_args	*args;
	t_ast	*right;

	r->names = NULL;
	r->file = NULL;
	r->str = red->name;
	if (!(right = red->right))
		return (ft_lstnew(r, sizeof(t_redir)));
	ft_astcresolver(right, expf);
	r->names = right->cargs;
	r->file = (right->cargs->argv ? right->cargs->argv[0] : NULL);
	if (right->args && (ptr = right->args->argv) && right->args->argc > 0)
	{
		args = cmd->args;
		if (!r->rep && *ptr)
			++ptr;
		else if (!r->rep)
			return (ft_lstnew(r, sizeof(t_redir)));
		while (*ptr && (s = ft_strdup(*ptr)) && ++ptr)
			args->argv = ft_memjoin_clr(args->argv,
				sizeof(char *) * args->argc++, &s, sizeof(char *));
		args->argv = ft_memjoin_clr(args->argv,
			sizeof(char *) * args->argc, ptr, sizeof(char *));
	}
	return (ft_lstnew(r, sizeof(t_redir)));
}

static t_list	*getrdrctn(t_ast *red, t_ast *cmd, t_expf *expf)
{
	t_redir		r;
	char		*ptr;
	int			def;

	ft_bzero(&r, sizeof(t_redir));
	if (ft_strmatch(red->name, "*>>*"))
		r.type = TK_LRIGHT;
	else if (ft_strmatch(red->name, "*<<*"))
		r.type = TK_LLEFT;
	else if (ft_strmatch(red->name, "*>*"))
		r.type = TK_RIGHT;
	else if (ft_strmatch(red->name, "*<*"))
		r.type = TK_LEFT;
	def = (r.type == TK_LEFT || r.type == TK_LLEFT ? 0 : 1);
	r.fda = (ft_isdigit(*red->name) ? ft_atoi(red->name) : def);
	ptr = red->name;
	while (ft_isdigit(*ptr))
		++ptr;
	ptr += ((r.type == TK_LLEFT || r.type == TK_LRIGHT) ? 2 : 1);
	r.rep = (*ptr == '&' ? 1 : 0);
	if (r.rep && *(ptr + 1) == '-')
		r.fdb = -1;
	else if (r.rep)
		r.fdb = (ft_isdigit(*(ptr + 1)) ? ft_atoi(ptr + 1) : def);
	return (implement_args(cmd, red, &r, expf));
}

t_list			*list_redirections(t_ast **ast, t_expf *expf)
{
	t_list	*lst;
	t_ast	*red;
	t_ast	*parent;

	lst = NULL;
	if (ast && *ast && (*ast)->type == TK_REDIR)
	{
		parent = (*ast)->parent;
		while ((*ast)->left)
			*ast = (*ast)->left;
		red = (*ast)->parent;
		while (red != parent)
		{
			ft_lstpush_p(&lst, getrdrctn(red, *ast, expf));
			red = red->parent;
		}
	}
	return (lst);
}

static int		read_heredoc(int fd, char *hdoc_file, t_ast *ast, char *ptr)
{
	char	line[500];
	int		c;

	while (ft_strclr(line))
	{
		ft_printf("heredoc> ");
		c = ft_readraw(line, 500);
		ft_putchar('\n');
		if (c == 3 || c == 4)
		{
			free(hdoc_file);
			close(fd);
			return (c);
		}
		if (ft_strequ(line, ast->right->name))
			break ;
		ft_putendl_fd(line, fd);
	}
	ptr[1] = '\0';
	free(ast->right->name);
	ast->right->name = hdoc_file;
	ast->right->args->argv[0] = ast->right->name;
	close(fd);
	return (0);
}

int				repair_hdoc(t_ast *ast, int n)
{
	char		*ptr;
	char		*hdoc_file;
	int			ret[2];

	if (!ast || ast->type == ast->cmd_offset)
		return (0);
	if ((ret[0] = repair_hdoc(ast->left, n + 1)) == 3)
		return (ret[0]);
	if ((ret[1] = repair_hdoc(ast->right, n + 2)) == 3)
		return (ret[1]);
	if (ast->type == TK_REDIR && (ptr = ft_strstr(ast->name, "<<")))
	{
		if (ast->right && ast->right->name)
		{
			if (!(hdoc_file = ft_strjoin_clr(HDOC_TMP_FILE, ft_itoa(n), 1)))
				return (0);
			if ((n = open(hdoc_file, O_WRONLY | O_TRUNC | O_CREAT, 0666)) != -1)
				return (read_heredoc(n, hdoc_file, ast, ptr));
			free(hdoc_file);
		}
	}
	if (ret[0])
		return (ret[0]);
	return (ret[1]);
}
