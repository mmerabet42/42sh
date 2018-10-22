/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_resolver.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmerabet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/15 16:37:58 by mmerabet          #+#    #+#             */
/*   Updated: 2018/10/22 18:01:30 by sle-rest         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "ft_str.h"
#include "ft_mem.h"
#include "ft_printf.h"

static int	strexpand1_split(t_strid *s, t_list **res, t_list *end,
								t_expf *expf)
{
	int		efail;
	t_list	*cur;

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
	return (0);
}

static int	strexpand1(t_strid *s, t_expf *expf, t_list **res)
{
	t_list	*end;
	int		ret;

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
		if ((ret = strexpand1_split(s, res, end, expf)))
			return (ret);
	}
	return (0);
}

static void	ft_strexpand_split(t_strid *strid, const char *origin)
{
	strid->str = ft_strndup(origin, strid->len);
	strid->next_str = origin + strid->len;
	strid->jump = 0;
}

int			ft_strexpand(const char *origin, t_list **res, int i, t_expf *expf)
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
		ft_strexpand_split(&strid, origin);
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
