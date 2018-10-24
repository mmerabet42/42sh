/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_glob.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-rest <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/02 22:30:11 by sle-rest          #+#    #+#             */
/*   Updated: 2018/10/24 15:27:50 by sle-rest         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "globing.h"
#include "parser.h"
#include "ft_list.h"
#include "ft_str.h"
#include "shell.h"

static t_glob	***get_glob(char ***splitpath, char **splitacc)
{
	t_glob	***glob;
	int		len;
	int		i;

	len = 0;
	while (splitpath[len])
		len++;
	if (!(glob = (t_glob ***)malloc(sizeof(t_glob **) * (len + 1))))
		return (NULL);
	i = -1;
	while (splitpath[++i])
	{
		if (!loop_get_glob(splitpath, glob, splitacc, i))
		{
			error_glob("g", &glob);
			return (NULL);
		}
	}
	glob[i] = NULL;
	return (glob);
}

static int		split_splitacc_into_lst(t_list **lst, t_list *tmp,
											char **splitacc)
{
	int		i;
	t_list	*new;

	i = 0;
	while (splitacc[++i])
	{
		if (!(new = ft_lstnew(splitacc[i], ft_strlen(splitacc[i]) + 1)))
			return (0);
		new->next = (*lst)->next;
		if ((*lst)->next)
			(*lst)->next->parent = new;
		(*lst)->next = new;
		new->parent = *lst;
		*lst = new;
	}
	while (*lst && (*lst)->parent && *lst != tmp)
		*lst = (*lst)->parent;
	return (1);
}

static int		splitacc_into_lst(t_list **lst, char **splitacc)
{
	t_list	*new;

	if (!(new = ft_lstnew(splitacc[0], ft_strlen(splitacc[0]) + 1)))
		return (0);
	new->next = (*lst)->next;
	if ((*lst)->next)
		(*lst)->next->parent = new;
	(*lst)->next = new;
	free((*lst)->content);
	free(*lst);
	*lst = new;
	if (!split_splitacc_into_lst(lst, *lst, splitacc))
		return (0);
	return (1);
}

static int		get_match(t_list **lst)
{
	char	**splitacc;
	char	***splitpath;
	t_glob	***glob;

	if (!(splitacc = split_acc((*lst)->content)))
		return (0);
	if (!(splitpath = split_path(splitacc)))
		return (error_glob("t", &splitacc));
	if (!splitacc_into_lst(lst, splitacc))
		return (error_glob("Tt", &splitpath, &splitacc));
	if (!(glob = get_glob(splitpath, splitacc)))
		return (error_glob("Tt", &splitpath, &splitacc));
	ft_free_tab(&splitacc);
	free_splitpath(&splitpath);
	if (!process_glob(glob, lst))
		return (error_glob("Ttg", &splitpath, &splitacc, &glob));
	free_triple_glob(&glob);
	return (1);
}

static t_exp		g_exps[] = {
	{"*[\"*\"@b]:'*':$'*'", exp_quote},
	{"*[\"'@=1]*[@>0]:$'*[@>0]:\":':$'", exp_quote},
};

static t_expf		g_expf = {
	g_exps, sizeof(g_exps), NULL, 0
};

int				exp_glob(t_strid *strid, t_list **lst, t_expf *expf)
{
	char	*str;

	(void)expf;
	ft_strexpand(strid->str, lst, strid->i, &g_expf);
	if (*lst && (*lst)->content)
		str = ft_strdup((*lst)->content);
	else
		str = ft_strdup(strid->str);
	ft_lstdel(lst, content_delfunc);
	if (!(*lst = ft_lstcreate(str, ft_strlen(str))))
		return (SH_MALLOC);
	if (!strid->i)
		return (0);
	if (detect_globbing((*lst)->content) && !get_match(lst))
		return (SH_MALLOC);
	while (lst && *lst && (*lst)->parent)
		*lst = (*lst)->parent;
	return (0);
}
