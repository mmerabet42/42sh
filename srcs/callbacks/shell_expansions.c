/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_expansions_v.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmerabet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/16 18:29:15 by mmerabet          #+#    #+#             */
/*   Updated: 2018/10/24 15:13:12 by jraymond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "ft_types.h"
#include "ft_str.h"
#include "ft_printf.h"

int			exp_var(t_strid *sid, t_list **res, t_expf *expf)
{
	char	*value;
	char	tmp;

	(void)expf;
	if (*sid->str == '\\')
	{
		*res = ft_lstcreate(ft_strndup(sid->str + 1, 1), 0);
		return (0);
	}
	tmp = sid->str[sid->len];
	sid->str[sid->len] = '\0';
	if (!ft_isalpha(*(sid->str + 1)) && !ft_isdigit(*(sid->str + 1)))
	{
		if (*(sid->str + 1) == '?')
			*res = ft_lstcreate(ft_itoa(g_shell->exitcode), 0);
	}
	else if ((value = ft_getenv(sid->str + 1, g_shell->envp)))
		*res = ft_lstcreate(ft_strdup(value), 0);
	else if ((value = ft_getenv(sid->str + 1, g_shell->localp)))
		*res = ft_lstcreate(ft_strdup(value), 0);
	sid->str[sid->len] = tmp;
	return (0);
}

int			exp_tild(t_strid *sid, t_list **res, t_expf *expf)
{
	char	*name;

	(void)expf;
	name = NULL;
	if (!*sid->next_str || *sid->next_str == '/')
		name = "HOME";
	else if (sid->next_str && (*sid->next_str == '+' || *sid->next_str == '-'))
	{
		if ((!sid->next_str[1] || sid->next_str[1] == '/') && !sid->j)
		{
			sid->jump = 1;
			name = (*sid->next_str == '+' ? "PWD" : "OLDPWD");
		}
	}
	if (name && !sid->j)
		*res = ft_lstcreate(ft_strdup(ft_getenv(name, g_shell->envp)), 0);
	else
		*res = ft_lstcreate(ft_strdup("~"), 0);
	return (0);
}

static t_exp	g_exps[] = {
	{"\\\\*[$`\"@=1]", exp_var},
	{"$*[aA0_-zZ9_]:$?", exp_var},
	{"$*[0-9]:$#:$@:$@*[0-9]", exp_arg},
	{"*[$((?));(?);\"*\";'*'@b]", exp_arth},
	{"*[${?};\"*\";'*'@b]", exp_dvar},
	{"*[`?`;${?};\"*\";'*'@b]", exp_cmd},
	{"*[$\\[*\\];\"*\";'*'@b]", exp_cond}
};

static t_expf	g_expf = {
	g_exps, sizeof(g_exps), NULL, 0
};

int			exp_quote(t_strid *sid, t_list **res, t_expf *expf)
{
	char	sep;
	t_list	*lst;

	(void)expf;
	g_expf.data = expf->data;
	if (sid->len == 1)
		return ((*res = NULL) ? 0 : 0);
	if ((sep = sid->str[sid->len - 1]) == '\'' || sep == '"')
		sid->str[sid->len - 1] = '\0';
	if (*sid->str == '$')
		*res = ft_lstcreate(ft_strdupk(sid->str + 2), 0);
	else if (*sid->str == '\'')
		*res = ft_lstcreate(ft_strdup(sid->str + 1), 0);
	else
	{
		lst = NULL;
		ft_strexpand(sid->str + 1, &lst, -1, &g_expf);
		*res = ft_lstcreate((!lst ? ft_strnew(0) : lst->content), 0);
		ft_lstdel(&lst, NULL);
	}
	sid->str[sid->len - 1] = sep;
	return (0);
}

static int	get_arguments1(t_strid *sid, t_list **res)
{
	int		n;
	char	**argv;

	argv = g_shell->curargs->argv;
	if ((n = ft_atoi(sid->str + 2)) >= g_shell->curargs->argc)
		return (1);
	if (sid->i == -1)
	{
		*res = ft_lstcreate(NULL, 0);
		while (n < g_shell->curargs->argc)
		{
			(*res)->content = ft_strjoin_clr((*res)->content, argv[n++], 0);
			if (n < g_shell->curargs->argc)
				(*res)->content = ft_strjoinc_clr((*res)->content, ' ');
		}
	}
	else
		while (n < g_shell->curargs->argc)
			ft_lstpush_p(res, ft_lstcreate(ft_strdup(argv[n++]), 0));
	return (0);
}

int			exp_arg(t_strid *sid, t_list **res, t_expf *expf)
{
	int	n;

	(void)expf;
	if (g_shell->curargs)
	{
		if (sid->str[1] == '#')
			*res = ft_lstcreate(ft_itoa(g_shell->curargs->argc), 0);
		else if (sid->str[1] == '@')
		{
			if (get_arguments1(sid, res))
				return (0);
		}
		else if ((n = ft_atoi(sid->str + 1)) < g_shell->curargs->argc)
			*res = ft_lstcreate(ft_strdup(g_shell->curargs->argv[n]), 0);
	}
	return (0);
}
