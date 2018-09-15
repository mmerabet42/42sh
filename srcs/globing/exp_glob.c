/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_glob.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-rest <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/02 22:30:11 by sle-rest          #+#    #+#             */
/*   Updated: 2018/09/15 18:04:58 by sle-rest         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "globing.h"
#include "parser.h"
# include "ft_list.h"
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
		if (!(new = ft_lstnew(splitacc[i], ft_strlen(splitacc[i]))))
			return (0);
		new->next = (*lst)->next;
		if ((*lst)->next)
			(*lst)->next->parent = new;
		(*lst)->next = new;
		new->parent = *lst;
		*lst = new;
	}
	while (*lst && (*lst)->parent && (*lst)->parent != tmp)
		*lst = (*lst)->parent;
	return (1);
}

static int		splitacc_into_lst(t_list **lst, char **splitacc)
{
	t_list	*new;

	if (!(new = ft_lstnew(splitacc[0], ft_strlen(splitacc[0]))))
		return (0);
	new->next = (*lst)->next;
	if ((*lst)->next)
		(*lst)->next->parent = new;
	(*lst)->next = new;
	new->parent = *lst;
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
		return (error_glob("t", &splitacc)); // NORMALEMENT ok
	if (!splitacc_into_lst(lst, splitacc))// NORMALEMENT ok
		return (error_glob("Tt", &splitpath, &splitacc)); // NORMALEMENT ok
	if (!(glob = get_glob(splitpath, splitacc)))
		return (error_glob("Tt", &splitpath, &splitacc)); // NORMALEMENT ok
	ft_free_tab(&splitacc);
	free_splitpath(&splitpath); // Surement pas doublon mais on sait jamais
	if (!process_glob(glob, lst)) // NORMALEMENT ok
		return (error_glob("Ttg", &splitpath, &splitacc, &glob));// NORMALEMENT ok
	free_triple_glob(&glob);
	return (1);
}

int				exp_glob(t_strid *strid, t_list **lst, t_expf *expf)
{
	t_list	*first_lst;

	(void)strid;
	(void)expf;
	first_lst = *lst; // pointe sur le premier arg ou le second ?? (*lst)->next
	while (*lst)
	{
		if (detect_globbing((*lst)->content)) // a modifier
		{
			if (!get_match(lst))
			{
				*lst = first_lst;
				return (SH_MALLOC); // ??
			}
		}
		if (*lst)
			*lst = (*lst)->next;
	}
	*lst = first_lst;
	return (1);
}
/*
typedef struct		s_list
{
	void			*content;
	size_t			content_size;
	struct s_list	*next;
	struct s_list	*parent;
}					t_list;

typedef strcut		s_args
{
	char	*arg;
	t_args	*next;
	t_args	*prev;
}					t_args;

typedef struct		s_strid
{
	char			*str;
	char			*next;
	int				len;
	int				ifound;
	int				jump;
	int				i;
	int				j;
}					t_strid;

typedef struct		s_expf
{
	t_exp			*expansions;
	size_t			len;
	void			*data;
	int				onlyfirst;
}					t_expf;

typedef struct		s_exp
{
	char			*name;
	t_expfunc		func;
}					t_exp;
*/
