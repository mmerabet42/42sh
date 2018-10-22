/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_astresolver.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-rest <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/22 17:45:21 by sle-rest          #+#    #+#             */
/*   Updated: 2018/10/22 17:45:23 by sle-rest         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "ft_mem.h"
#include "ft_list.h"

static int	listtotab(t_list *it, char ***av, int *ac)
{
	int		size;
	int		x;

	x = -1;
	if (!(size = ft_lstsize(it)))
		return (0);
	if (!(*av = (char **)ft_memalloc(sizeof(char *) * (size + 1))))
		return (SH_MALLOC);
	*ac = size;
	while (++x < size && it && !g_shell->kill_builtin)
	{
		(*av)[x] = (char *)it->content;
		it = it->next;
	}
	(*av)[x] = NULL;
	if (g_shell->kill_builtin)
	{
		ft_lstdel(&it, NULL);
		g_shell->kill_builtin = 0;
		return (-1);
	}
	return (0);
}

int			ft_astcresolver(t_ast *ast, t_expf *expf)
{
	int		efail;
	t_list	*lst;

	if ((efail = ft_resolver(ast->args, &lst, expf)))
		return (efail);
	ft_argsdel(ast->cargs);
	free(ast->cargs);
	if (!(ast->cargs = (t_args *)ft_memalloc(sizeof(t_args))))
	{
		ft_lstdel(&lst, NULL);
		return (SH_MALLOC);
	}
	if (listtotab(lst, &ast->cargs->argv, &ast->cargs->argc) == -1)
		return (1);
	if (ast->cargs->argv)
		ast->cname = ast->cargs->argv[0];
	ft_lstdel(&lst, NULL);
	return (0);
}

int			ft_astresolver(t_ast *ast, t_expf *expf)
{
	int		efail;
	t_list	*lst;

	if ((efail = ft_resolver(ast->cargs, &lst, expf)))
		return (efail);
	ft_argsdel(ast->cargs);
	free(ast->cargs);
	if (!(ast->cargs = (t_args *)ft_memalloc(sizeof(t_args))))
	{
		ft_lstdel(&lst, NULL);
		return (SH_MALLOC);
	}
	if (listtotab(lst, &ast->cargs->argv, &ast->cargs->argc) == -1)
		return (1);
	if (ast->cargs->argv)
		ast->cname = ast->cargs->argv[0];
	ft_lstdel(&lst, NULL);
	return (0);
}
