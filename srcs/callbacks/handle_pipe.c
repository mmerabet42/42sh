#include "shell.h"
#include "ft_list.h"

int				check_redir(t_list **begin, t_ast *ast)
{
	t_list	*elem;

	elem = NULL;
	while (ast->parent && ast->parent->type == TK_REDIR)
	{
		if (!(elem = ft_lstnew(NULL, 0)))
			return (SH_MALLOC);
		elem->content = ast;
		if (!*begin)
			*begin = elem;
		else
			ft_lstpush(*begin, elem);
		ast = ast->parent;
	}
	return (0);
}

static int		son_left_right(t_ast *ast, t_list **pipe)
{
	int		x;
	t_list	*elem;
	t_ast	*ast1;

	x = 0;
	while (++x <= 2)
	{
		(ast1 = (x == 1) ? ast->right : ast->left);
		if (ast1)
		{
			if (!(elem = ft_lstnew(NULL, 0)))
				return (SH_MALLOC);
			elem->content = ast1;
			log_debug("name-> %s\n", ((t_ast *)elem->content)->name);
			if (!*pipe)
				*pipe = elem;
			else
				ft_lstpush(*pipe, elem);
		}
	}
	return (0);
}

int				handle_ast_pipe(t_ast *ast, t_list **pipe)
{
	t_list	*elem;

	while (ast && ast->left && ast->left->type == TK_PIPE + ast->cmd_offset)
	{
		if (ast->right)
		{
			if (!(elem = ft_lstnew(NULL, 0)))
				return (SH_MALLOC);
			elem->content = ast->right;
			log_debug("name-> %s\n", ((t_ast *)elem->content)->name);
			if (!*pipe)
				*pipe = elem;
			else
				ft_lstpush(*pipe, elem);
		}
		ast = ast->left;
	}
	if (ast)
		return (son_left_right(ast, pipe));
	return (0);
}
