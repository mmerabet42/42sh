/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_equal.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmerabet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/14 21:09:46 by mmerabet          #+#    #+#             */
/*   Updated: 2018/09/24 17:59:35 by mmerabet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "ft_printf.h"
#include "ft_str.h"
#include "ft_types.h"

t_func		*get_function(char *name)
{
	t_list	*it;
	t_func	*func;

	it = g_shell->funcs;
	while (it && (func = (t_func *)it->content))
	{
		if (ft_strequ(name, func->name))
			return (func);
		it = it->next;
	}
	return (NULL);
}

static void	shell_equal_cb2(t_ast *astr, char *lstr, t_allf *allf, t_list *lst)
{
	t_func	*funcp;
	t_func	func;

	if (*astr->name == '{')
	{
		if ((funcp = get_function(lstr)))
		{
			free(funcp->name);
			free(funcp->src);
			ft_astdel(&funcp->ast);
		}
		else
			funcp = (t_func *)ft_lstpush_p(&g_shell->funcs,
					ft_lstnew(&func, sizeof(t_func)))->content;
		funcp->name = ft_strdup(lstr);
		funcp->src = ft_strndup(astr->name + 1, ft_strlen(astr->name) - 2);
		funcp->ast = ft_lexer(funcp->src, allf->lexerf);
	}
	else
	{
		ft_strexpand(astr->name, &lst, -1, allf->expf);
		if (lst && lst->content)
			ft_setenv(lstr, (char *)lst->content, &g_shell->envp);
		ft_lstdel(&lst, content_delfunc);
	}
}

int			shell_equal_cb(t_ast *ast, void **op, void *res, t_iterf *iterf)
{
	t_list	*lst;
	t_list	*lst2;

	(void)op;
	if (!ft_astvalid(ast->left) && (*(int *)res = 1))
		return (SH_BADEXPR);
	lst = NULL;
	ft_strexpand(ast->left->name, &lst, -1, ((t_allf *)iterf->data)->expf);
	lst2 = NULL;
	if (lst && lst->content)
	{
		if (!ft_astvalid(ast->right))
			ft_setenv(lst->content, "", &g_shell->envp);
		else
			shell_equal_cb2(ast->right, lst->content, (t_allf *)iterf->data,
					lst2);
	}
	ft_lstdel(&lst, content_delfunc);
	*(int *)res = 0;
	return (0);
}

int			check_function(t_ast *ast, void *res, t_iterf *iterf)
{
	t_func	*func;
	t_args	*args;
	int		ret;

	if (!(func = get_function(ast->cname)))
		return (-1);
	args = g_shell->curargs;
	g_shell->curargs = ft_argscopy(ast->cargs);
	ret = ft_astiter(func->ast, res, iterf);
	ft_argsdel(g_shell->curargs);
	free(g_shell->curargs);
	g_shell->curargs = args;
	return (ret);
}
