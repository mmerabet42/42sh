/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_resolver.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmerabet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/15 16:37:58 by mmerabet          #+#    #+#             */
/*   Updated: 2018/10/13 11:40:14 by jraymond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "ft_str.h"
#include "ft_mem.h"
#include "ft_printf.h"

static int	strexpand1(t_strid *s, t_expf *expf, t_list **res)
{
	t_list	*end;
	t_list	*cur;
	int		efail;

	end = ft_lstend(*res);
	if (s->ifound == -1 || !expf->expansions[s->ifound].func)
	{
		if (!*res)
			*res = ft_lstcreate(s->str, 0);
		else if (end)
			end->content = ft_strjoin_clr(end->content, s->str, 2);
	}
	else if (expf->expansions[s->ifound].func)
	{
		s->prev = end;
		cur = NULL;
		if ((efail = expf->expansions[s->ifound].func(s, &cur, expf)))
		{
			free(s->str);
			ft_lstdel(res, content_delfunc);
			ft_lstdel(&cur, content_delfunc);
			return (efail);
		}
		free(s->str);
		if (!*res)
			*res = cur;
		else if (cur)
		{
			end->content = ft_strjoin_clr(end->content, cur->content, 0);
			if (cur->next)
				ft_lstpush(end, cur->next);
			ft_lstdelone(&cur, content_delfunc);
		}
	}
	return (0);
}

int	ft_strexpand(const char *origin, t_list **res, int i, t_expf *expf)
{
	t_strid	strid;
	t_list	*lres;
	int		pos;

	ft_bzero(&strid, sizeof(t_strid));
	strid.i = i;
	lres = NULL;
	while (*origin)
	{
		pos = ft_strpbrkstrp_pos(origin, 0, expf->len, expf->expansions,
				sizeof(t_exp), 0);
		strid.ifound = (pos == 0 ? g_ifound : -1);
		if (pos == 0)
			strid.len = g_iread;
		else
			strid.len = (pos == -1 ? (int)ft_strlen(origin) : pos);
		strid.str = ft_strndup(origin, strid.len);
		strid.next_str = origin + strid.len;
		strid.jump = 0;
		if ((pos = strexpand1(&strid, expf, &lres)))
			return (pos);
		origin += strid.len + strid.jump;
		++strid.j;
	}
	ft_lstpush_p(res, lres);
	return (0);
}

int			ft_resolver(t_args *args, t_list **lst, t_expf *expf)
{
	int		i;
	int		err;

	i = 0;
	*lst = NULL;
	while (i < args->argc && (!i || !expf->onlyfirst))
	{
		if ((err = ft_strexpand(args->argv[i], lst, i, expf)))
		{
			ft_lstdel(lst, content_delfunc);
			return (err);
		}
		++i;
	}
	return (0);
}

static int	listtotab(t_list *it, char ***av, int *ac)
{
	int		size;
	int		x;
	int		c;

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
	c = it ? 1 : 0;
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
	ast->cname = ast->cargs->argv[0];
	ft_lstdel(&lst, NULL);
	return (0);
}
