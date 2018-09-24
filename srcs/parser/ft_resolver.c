/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_resolver.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmerabet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/15 16:37:58 by mmerabet          #+#    #+#             */
/*   Updated: 2018/09/19 13:57:57 by mmerabet         ###   ########.fr       */
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
			*res = ft_lstcreate(ft_strdup(s->str), 0);
		else if (end)
			end->content = ft_strjoin_clr(end->content, s->str, 0);
	}
	else if (expf->expansions[s->ifound].func)
	{
		s->prev = end;
		cur = NULL;
		if ((efail = expf->expansions[s->ifound].func(s, &cur, expf)))
		{
			ft_lstdel(res, content_delfunc);
			ft_lstdel(&cur, content_delfunc);
			return (efail);
		}
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
		ft_strncpy(strid.str, origin, strid.len)[strid.len] = '\0';
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
/*
static int	strexpand1(t_list *it, t_expf *expf, t_list **res)
{
	t_strid		*s;
	t_list		*cur;
	t_list		*end;
	t_list		*tmp_lst;
	int			efail;

	while (it && (s = (t_strid *)it->content))
	{
		cur = NULL;
		s->jump = 0;
		if (s->ifound == -1 || !expf->expansions[s->ifound].func)
		{
			if (!*res)
				*res = ft_lstcreate(ft_strndup(s->str, s->len), 0);
			else
				(*res)->content = ft_strn2join_clr(s->str, (*res)->content, s->len, 1);
		}
		else if (expf->expansions[s->ifound].func)
		{
			s->next_str = (*res ? (*res)->content : NULL);
			s->next = *res;
			if ((efail = expf->expansions[s->ifound].func(s, &cur, expf)))
			{
				ft_lstdel(res, content_delfunc);
				ft_lstdel(&cur, content_delfunc);
				return (efail);
			}
			if (!*res)
			{
				if (!cur)
					*res = ft_lstcreate(ft_strdup(s->str), 0);
				else
					*res = cur;
			}
			else if (cur && cur != *res && (tmp_lst = *res))
			{
				if ((end = ft_lstend(cur)) != cur)
				{
					end->parent->next = NULL;
					ft_lstpushfront(res, cur);
				}
				tmp_lst->content = ft_strjoin(end->content, s->next_str + s->jump);
				ft_lstdelone(&end, content_delfunc);
			}
			else if (cur == s->next)
				(*res)->content = ft_strn2join(s->str, s->next_str + s->jump, s->len);
			if (cur)
				ft_memdel((void **)&s->next_str);
		}
		it = it->next;
	}
	return (0);
}

int			ft_strexpand(const char *origin, t_list **res, int i, t_expf *expf)
{
	t_list		*head;
	t_list		*lres;
	t_strid		strid;
	int			pos;
	int			efail;

	head = NULL;
	strid.i = i;
	strid.j = 0;
	strid.next = NULL;
	strid.next_str = NULL;
	while (*origin)
	{
		pos = ft_strpbrkstrp_pos(origin, 0, expf->len, expf->expansions,
				sizeof(t_exp), 0);
		strid.ifound = (pos == 0 ? g_ifound : -1);
		if (pos == 0)
			strid.len = g_iread;
		else
			strid.len = (pos == -1 ? (int)ft_strlen(origin) : pos);
		ft_strncpy(strid.str, origin, strid.len)[strid.len] = '\0';
		origin += strid.len;
		ft_lstpushfront(&head, ft_lstnew(&strid, sizeof(t_strid)));
		++strid.j;
	}
	lres = NULL;
	efail = strexpand1(head, expf, &lres);
	ft_lstpush_p(res, lres);
	ft_lstdel(&head, content_delfunc);
	return (efail);
}
*/
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

int			ft_astcresolver(t_ast *ast, t_expf *expf)
{
	int		efail;
	t_list	*lst;
	t_list	*it;

	if ((efail = ft_resolver(ast->args, &lst, expf)))
		return (efail);
	ft_argsdel(ast->cargs);
	free(ast->cargs);
	if (!(ast->cargs = (t_args *)ft_memalloc(sizeof(t_args))))
		return (-1);
	it = lst;
	while (it)
	{
		ast->cargs->argv = ft_memjoin_clr(ast->cargs->argv,
				sizeof(char *) * ast->cargs->argc++, &it->content, sizeof(char *));
		it = it->next;
	}
	ast->cargs->argv = ft_memjoin_clr(ast->cargs->argv,
			sizeof(char *) * ast->cargs->argc, &it, sizeof(char *));
	ast->cname = ast->cargs->argv[0];
	ft_lstdel(&lst, NULL);
	return (0);
}
