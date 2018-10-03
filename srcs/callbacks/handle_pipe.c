#include "shell.h"
#include "ft_list.h"

int		check_redir(t_list **begin, t_ast *ast)
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

int		handle_ast_pipe(t_ast *ast, t_list **pipe)
{
	t_list	*elem;
	t_ast	*ast1;
	t_ast	*tmp;
	int		x;

	x = 0;
	tmp = ast;
	while (ast && ast->left && ast->left->type == TK_PIPE + 1)
	{
		if (ast->right)
		{
			if (!(elem = ft_lstnew(NULL, 0)))
				return (SH_MALLOC);
			elem->content = ast->right;
			if (!*pipe)
				*pipe = elem;
			else
				ft_lstpush(*pipe, elem);
		}
		ast = ast->left;
	}
	while (++x <= 2)
	{
		if (ast && (ast1 = (x == 1) ? ast->right : ast->left))
		{
			if (!(elem = ft_lstnew(NULL, 0)))
				return (SH_MALLOC);
			elem->content = ast1;
			if (!*pipe)
				*pipe = elem;
			else
				ft_lstpush(*pipe, elem);
		}
	}
	return (0);
}
